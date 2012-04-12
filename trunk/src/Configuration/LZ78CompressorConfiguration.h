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
#define FINAL_INDEX_LENGTH 15 //era 15 log_2(MAX_CHILD), provato a 21
#define INDEX_LENGTH_MASK 511 //111111111 (INITIAL_INDEX_LENGTH ones)
#define MAX_CHILD 65535//era 65535, provato a 1048575
#define LOCAL_BYTE_BUFFER_LENGTH 2 //era 2, provato a 8
#define LZ78_INTERPRETER "#!/usr/bin/env lz78"
#define INDEX_TYPE uint16_t //era uint16_t, provato a 32
#define CELL_TYPE_LENGTH 16// era 16, provato a 32
