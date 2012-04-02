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

struct LZ78HashTable* hashInitialize();

INDEX_TYPE hashLookup(struct LZ78HashTable*, INDEX_TYPE, uint8_t*);

int hashInsert(struct LZ78HashTable*, INDEX_TYPE, uint8_t*, INDEX_TYPE);

int hashReset(struct LZ78HashTable*);

void hashDestroy(struct LZ78HashTable*);