/**
 * @file LZ78CompressorConfiguration.h
 * @author Cosimo Sacco <cosimosacco@gmail.com>
 * @author Davide Silvestri <davidesil.web@gmail.com>
 *
 * @section LICENSE
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
 **/

#define ROOT_INDEX 256//TODO: DEVE essere 256, altrimenti non funziona nulla (vedi tableCreate)
#define INITIAL_INDEX_LENGTH 9 //log_2(ROOT_INDEX)
#define FINAL_INDEX_LENGTH 16 //log_2(MAX_CHILD)
#define INDEX_LENGTH_MASK 511 //111111111 (INITIAL_INDEX_LENGTH ones)
#define MAX_CHILD 65535
#define LOCAL_BYTE_BUFFER_LENGTH 8
#define LZ78_INTERPRETER "#!/usr/bin/env lz78"
#if FINAL_INDEX_LENGTH <= 16 //TODO Attenti agli include + far dipendere CELL_TYPE da MAX_CHILD
#define INDEX_TYPE CELL_TYPE 
//TODO CELL_TYPE dipende da MAX_CHILD + attenzione agli include
