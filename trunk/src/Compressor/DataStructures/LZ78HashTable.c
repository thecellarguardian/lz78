/**
 * @file LZ78HashTable.c
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

#include <stdlib.h>
#include <strings.h>
#include "LZ78HashTable.h"
#include "../../Configuration/LZ78CompressorConfiguration.h"

struct LZ78HashTableEntry
{
    //key:
    INDEX_TYPE fatherIndex;
    uint8_t childValue;
    //value:
    INDEX_TYPE childIndex;
};

INDEX_TYPE hashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //SAX hash function
{
    INDEX_TYPE index = 0;
    key1 ^= key2; //TODO migliorare
    int i = sizeof(INDEX_TYPE);
    for (; i-- ;) index ^= ( index << 5 ) + ( index >> 2 ) + (&key1)[i];
    return index;
}

//TODO if table!= null ovunque??

int hashInsert
(
    struct LZ78HashTableEntry* table,
    INDEX_TYPE fatherIndex,
    uint8_t* childValue,
    INDEX_TYPE childIndex
) //TODO inline??
{
    struct LZ78HashTableEntry* result;
    do
    {
        result = table + hashFunction(fatherIndex, *childValue);
    }
    while(result->childIndex != ROOT_INDEX && result < table + MAX_CHILD*2);
    if(result->childIndex != ROOT_INDEX) return -1; //collisione
    result->childIndex = childIndex;
    result->fatherIndex = fatherIndex;
    result->childValue = *childValue;
    return 0;
}

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry* table)
{
    int i = HASH_TABLE_LENGTH - 1;
    uint8_t currentValue = ROOT_INDEX - 1;
    if(table != NULL)
    {
        for(; i-- ;) table[i].childIndex = ROOT_INDEX;
        for(; currentValue--;) //i caratteri ascii coincidono con il loro indice
        {
            if
            (
                hashInsert
                (
                    table,
                    ROOT_INDEX,
                    &currentValue,
                    currentValue
                ) == -1
            ) goto exceptionHandler;
        }
    }
    return table;

    exceptionHandler:
    hashDestroy(table);
    return NULL;
}

inline struct LZ78HashTableEntry* hashCreate()
{
    return hashInitialize(malloc(HASH_TABLE_LENGTH));
}

inline struct LZ78HashTableEntry* hashReset(struct LZ78HashTableEntry* table)
{
    return hashInitialize(table);
}

INDEX_TYPE hashLookup
(
    struct LZ78HashTableEntry* table,
    INDEX_TYPE fatherIndex,
    uint8_t* childValue
) //TODO inline?
{
    struct LZ78HashTableEntry* result;
    do result = table + hashFunction(fatherIndex, *childValue);
    while
    (
        result->childValue  != *childValue &&
        result->fatherIndex != fatherIndex &&
        result < table + MAX_CHILD*2
    );
    return
    (
        result->childValue  == *childValue &&
        result->fatherIndex == fatherIndex
    )?
        result->childIndex : ROOT_INDEX;
}

void hashDestroy(struct LZ78HashTableEntry* table){
    bzero(table, HASH_TABLE_LENGTH);
    free(table);
}
