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
#include "devs.h"
#include <stdlib.h>
#include <stdbool.h>

devs_message* new_devs_message() {
  devs_message* list = (devs_message*)malloc(sizeof(devs_message));
  list->head = NULL;
  list->tail = NULL;
  return list;
}

bool is_empty_devs_message(const devs_message* msg) {
  return msg->head == NULL;
}

devs_message* push_back_devs_message(devs_message* msg, int port_id, void* value) {
  devs_node* node = (devs_node*)malloc(sizeof(devs_node));
  node->port_id = port_id;
  node->value = value;
  node->prev = NULL;
  node->next = NULL;
  if(is_empty_devs_message(msg)) {
    msg->head = msg->tail = node;
  }
  else {
    node->prev = msg->tail;
    msg->tail->next = node;
    msg->tail = node;
  }
  return msg;
}

