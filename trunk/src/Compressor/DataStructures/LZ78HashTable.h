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
#include "../../Configuration/LZ78CompressorConfiguration.h"
#include "../../../lib/BitwiseBufferedFile/bufferConfiguration.h"

struct LZ78HashTableEntry
{
    //key:
    INDEX_TYPE fatherIndex;
    INDEX_TYPE childValue; //Only one byte is used
    //value:
    INDEX_TYPE childIndex;
};

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry*, uint32_t, int*);

inline struct LZ78HashTableEntry* hashCreate(uint32_t, int*);

inline INDEX_TYPE hashLookup(struct LZ78HashTableEntry*, INDEX_TYPE, INDEX_TYPE, uint32_t, int*);

inline int hashInsert(struct LZ78HashTableEntry*, INDEX_TYPE, INDEX_TYPE, INDEX_TYPE, uint32_t, int*);

inline struct LZ78HashTableEntry* hashReset(struct LZ78HashTableEntry*, uint32_t, int*);

inline void hashDestroy(struct LZ78HashTableEntry*, uint32_t);
