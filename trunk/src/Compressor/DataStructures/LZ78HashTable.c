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
#include <stdio.h>
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
    printf("HASH");
    INDEX_TYPE index = 0;
    uint8_t* byteKey1 = (uint8_t*)(&key1);
    key1 ^= key2; //TODO migliorare
    int i = sizeof(INDEX_TYPE);
    for (; i-- ;) index ^= ( index << 5 ) + ( index >> 2 ) + ((INDEX_TYPE)*(&byteKey1)[i]);
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
    //è tutto molto lento, inoltre gli INDEX_TYPE sono un casino, non è proprio
    //bello che siano uguali ai CELL_TYPE
    printf("father: %llu\nchildVal: %u, childIndex: %llu", fatherIndex, childValue, childIndex);
    INDEX_TYPE index = hashFunction(fatherIndex, *childValue);
    INDEX_TYPE i = 0;
    while(table[index].childIndex != ROOT_INDEX && i < MAX_CHILD*2) //lento
    {
        index = (index + 1)%(MAX_CHILD*2); //lento
        i++;
    }
    if(table[index].childIndex != ROOT_INDEX) return -1;
    table[index].childIndex = childIndex; //lento
    table[index].fatherIndex = fatherIndex; //lento
    table[index].childValue = *childValue; //lento
    return 0;
}

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry* table)
{
    int i = HASH_TABLE_LENGTH;
    uint8_t currentValue = 0;
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
    printf("aaa");
    return hashInitialize((struct LZ78HashTableEntry*)malloc(HASH_TABLE_LENGTH));
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
    INDEX_TYPE index = hashFunction(fatherIndex, *childValue);
    INDEX_TYPE i = 0;
    while
    (
        table[index].childValue  != *childValue && //lento
        table[index].fatherIndex != fatherIndex && //lento
        i < MAX_CHILD*2
    )
    {
        index = (index + 1)%(MAX_CHILD*2); //lento
        i++;
    }
    return
    (
        table[index].childValue  == *childValue && //lento
        table[index].fatherIndex == fatherIndex //lento
    )?
        table[index].childIndex : ROOT_INDEX; //lento
}

void hashDestroy(struct LZ78HashTableEntry* table){
    bzero(table, HASH_TABLE_LENGTH);
    free(table);
}
