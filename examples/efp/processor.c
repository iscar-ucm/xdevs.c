/*
 * Copyright (C) 2022 José Luis Risco Martín <jlrisco@ucm.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contributors:
 *  - José Luis Risco Martín
 */
#include "processor.h"

void processor_lambda(atomic *self) {
  processor_state *s = self->state.user_data;
  devs_message_push_back(&(self->output), PROCESSOR_OUT, s->current_job);
  return;
}

void processor_deltint(atomic *self) {
  processor_state *s = self->state.user_data;
  // "Free" current job:
  if (s->current_job != NULL) {
    s->current_job = NULL;
  }
  // Update clock:
  s->clock += self->state.sigma;
  // Passivate the current model:
  passivate(self);
  return;
}

void processor_deltext(atomic *self, const double e) {
  resume(self, e);
  processor_state *s = self->state.user_data;
  // Update clock:
  s->clock += e;
  // If the model is available, then take the job:
  if (s->current_job == NULL) {
    s->current_job = (job *)devs_port_get_value(&(self->input), PROCESSOR_IN);
    s->current_job->time = s->clock;
    hold_in(self, "active", s->processing_time);
  }    
}

void processor_init(atomic *self) {
  processor_state *s = self->state.user_data;
  s->clock = 0.0;
  s->current_job = NULL;
  passivate(self);
}

atomic *processor_new(double period) {
  atomic *processor = atomic_new();
  processor_state *data = (processor_state *)malloc(sizeof(processor_state));
  data->clock = 0.0;
  data->processing_time = period;
  data->current_job = NULL;
  processor->state.user_data = data;
  processor->initialize = processor_init;
  processor->ta = ta_default;
  processor->lambda = processor_lambda;
  processor->deltint = processor_deltint;
  processor->deltext = processor_deltext;
  processor->deltcon = deltcon_default;
  processor->exit = exit_default;
  return processor;
}
