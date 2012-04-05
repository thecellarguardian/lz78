/**
 * @file LZ78HashTable.h
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

#include <stdint.h>
#include "../../../lib/BitwiseBufferedFile/bufferConfiguration.h"

#define INDEX_TYPE CELL_TYPE
#define HASH_TABLE_LENGTH MAX_CHILD * 2 * sizeof(struct LZ78HashTableEntry)

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry*);

struct LZ78HashTableEntry* hashCreate();

INDEX_TYPE hashLookup(struct LZ78HashTableEntry*, INDEX_TYPE, uint8_t*);

int hashInsert(struct LZ78HashTableEntry*, INDEX_TYPE, uint8_t*, INDEX_TYPE);

struct LZ78HashTableEntry* hashReset(struct LZ78HashTableEntry*);

void hashDestroy(struct LZ78HashTableEntry*);
