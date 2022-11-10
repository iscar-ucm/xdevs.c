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
#include "simulation.h"
#include "devs.h"

simulator *simulator_new(atomic *model)
{
  simulator *sim = (simulator *)malloc(sizeof(simulator));
  sim->model = model;
  return sim;
}

void simulator_delete(simulator *sim)
{
  free(sim);
  return;
}

void simulator_initialize(simulator *sim, atomic *model)
{
  sim->model = model;
  sim->model->initialize(sim->model);
  sim->tL = 0.0;
  sim->tN = sim->tL + sim->model->ta(sim->model);
}

void simulator_exit(simulator *sim)
{
  sim->model->exit(sim->model);
}

double simulator_ta(const simulator *sim)
{
  return sim->model->ta(sim->model);
}

void simulator_deltfcn(simulator *sim)
{
  double t = 0.0;
  bool is_input_empty = devs_message_is_empty(&(sim->model->input));
  if (!is_input_empty)
  {
    double e = sim->tL;
    if (t == sim->tN)
    {
      sim->model->deltcon(sim->model, e);
    }
    else
    {
      sim->model->deltext(sim->model, e);
    }
  }
  else if (t == sim->tN)
    sim->model->deltint(sim->model);
  else
    return;
  sim->tL = t;
  sim->tN = sim->tL + sim->model->ta(sim->model);
  /* TODO: Should I free here the memory associated with msg??? */
}

void simulator_lambda(simulator *sim)
{
  if (sim->tN)
  {
    sim->model->lambda(sim->model);
  }
}

coordinator *coordinator_new(const coupled *model)
{
  coordinator *c = (coordinator *)malloc(sizeof(coordinator));
  c->model = model;
  list_node *current_node = model->components.head;
  while (current_node != NULL)
  {
    int component_type = (int)(current_node->data + 0);
    bool is_coupled = DEVS_IS_COUPLED(component_type);
    bool is_atomic = DEVS_IS_ATOMIC(component_type);
    if (is_coupled)
    {
      coordinator *current_sim = coordinator_new(current_node->data);
      list_push_back(&(c->simulators), current_sim);
    }
    else if (is_atomic)
    {
      simulator *current_sim = simulator_new(current_node->data);
      list_push_back(&(c->simulators), current_sim);
    }
    current_node = current_node->next;
  }
  return c;
}

void coordinator_initialize(coordinator *sim)
{
  list_node* n = sim->simulators.head;
  while(n!=NULL) {
    simulator* s = n->data;
    simulator_initialize(s, s->model);
    n = n->next;
  }
  sim->tL = 0.0;
  sim->tN = sim->tL + coordinator_ta(sim);
}

double coordinator_ta(const coordinator *sim)
{
  double tn = INFINITY;
  list_node* n = sim->simulators.head;
  while(n!=NULL) {
    simulator* s = n->data;
    if(s->tN < tn) tn = s->tN;
    n = n->next;
  }
  return tn;
}

devs_message *coordinator_lambda(const coordinator *sim, double t)
{
  list_node* n = sim->simulators.head;
  while(n!=NULL) {
    simulator* s = n->data;
    simulator_lambda(s);
    n = n->next;
  }
  coordinator_propagate_output(sim);
}

void coordinator_deltfcn(coordinator *sim, double t)
{
  coordinator_propagate_input(sim);
  list_node* n = sim->simulators.head;
  while(n!=NULL) {
    simulator* s = n->data;
    simulator_deltfcn(s);
    n = n->next;
  }
  sim->tL = 0;
  sim->tN = sim->tL + coordinator_ta(sim);
}

void coordinator_exit(coordinator *sim)
{
  list_node* n = sim->simulators.head;
  while(n!=NULL) {
    simulator* s = n->data;
    simulator_exit(s);
  }
}

void coordinator_simulate(coordinator* sim, unsigned long int nsteps) {
  printf("Starting simulation ...\n");
  unsigned long int steps = 0;
  while(steps < nsteps && sim->tN < INFINITY) {
    coordinator_lambda(sim, sim->tN);
    coordinator_deltfcn(sim, sim->tN);
    coordinator_clear(sim);
    steps++;
  }
}
