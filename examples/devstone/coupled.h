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
#include "../../core/modeling.h"
#include "atomic.h"

#define DSCOUPLED_IN 0
#define DSCOUPLED_IN_AUX 1
#define DSCOUPLED_OUT 2
#define DSCOUPLED_OUT_AUX 3

coupled* coupled_li_new( int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time);
coupled* coupled_hi_new( int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time);
coupled* coupled_ho_new( int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time);
coupled* coupled_homod_new( int width, int depth, double preparation_time, double int_delay_time, double ext_delay_time);
