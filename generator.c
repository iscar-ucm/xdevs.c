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
#include "generator.h"
#include "devs.h"

devs_message* lambda(const devs_state* s) {
  devs_message* msg = devs_message_new();
  job_t* job = (job_t*)malloc(sizeof(job_t));
  job->id = ((generator_state*)s->user_data)->job_next_id;
  devs_message_push_back(msg, PORT_OUT, job);
  return msg;
}

devs_state* deltint(devs_state* state) {
  generator_state* s = state->user_data;
  s->job_next_id++;
  return state;
}

devs_state *deltext(devs_state *state, const double e, const devs_message* msg) { return passivate(state); }

struct atomic_operations generator = {
                                      .ta = ta_default,
                                      .lambda = lambda,
                                      .deltint = deltint,
                                      .deltext = deltext
};

