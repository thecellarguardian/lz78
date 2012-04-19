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

#define ROOT_INDEX 0
#define INITIAL_INDEX_LENGTH 9 //log_2(ROOT_INDEX)
//#define FINAL_INDEX_LENGTH 24 //era 16 log_2(MAX_CHILD+1), provato a 20
#define INDEX_LENGTH_MASK 511 //111111111 (INITIAL_INDEX_LENGTH ones)
#define MAX_CHILD 16777215//era 65535, provato a 1048575 CONF
#define LOCAL_BYTE_BUFFER_LENGTH 128 //era 64
#define LZ78_INTERPRETER "#!/usr/bin/env lz78"
#define INDEX_TYPE uint32_t //era uint16_t, provato a 32
#define CELL_TYPE_LENGTH 32// era 16, provato a 32
#define HASH_INDEX uint32_t //era uint32_t, il numero di bit che serve per indicizzare 2*MAX_CHILD, provato a 64
#define HASH_TABLE_LENGTH (33554432)*sizeof(struct LZ78HashTableEntry) //CONF
#define HASH_TABLE_ENTRIES (33554432) //CONF
