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
    int empty; //TODO newton ha trovato un modo per cui non serve empty
};

HASH_INDEX SAXhashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //SAX hash function
{
    HASH_INDEX index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++)
        index ^= (index << 5) + (index >> 2) + ((INDEX_TYPE)(keyArray[i]));
    index %= MAX_CHILD*2; //TODO lento
    return index;
}

HASH_INDEX hashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //Bernstein hash function
{
    HASH_INDEX index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++)
        index = 33 * index + ((INDEX_TYPE)(keyArray[i]));
    index %= MAX_CHILD*2; //TODO lento
    return index;
}

HASH_INDEX ELFhashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //ELF hash function
{
    HASH_INDEX index = 0;
    HASH_INDEX g = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++){
        index = ( index << 4 ) + ((INDEX_TYPE)(keyArray[i]));
    g = index & 0xf0000000L;
    if ( g != 0 )
        index ^= g >> 24;
    index &= ~g;
    }
    index %= MAX_CHILD*2; //TODO lento
    return index;
}

HASH_INDEX JSWhashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //JSW hash function
{
    HASH_INDEX* tab = malloc(256*sizeof(HASH_INDEX));
    int j = 0;
    for(;j<256;j++){
    tab[j]=j;
    }
    HASH_INDEX index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++)
        index = ( index << 1 | index >> 31 ) ^ tab[((INDEX_TYPE)(keyArray[i]))];
    index %= MAX_CHILD*2; //TODO lento
    free(tab);
    return index;
}

int hashInsert
(
    struct LZ78HashTableEntry* table,
    INDEX_TYPE fatherIndex,
    uint8_t* childValue,
    INDEX_TYPE childIndex,
    int* collision
)
{
    if(table == NULL) return -1;
    struct LZ78HashTableEntry* current;
    HASH_INDEX index = hashFunction(fatherIndex, (INDEX_TYPE)(*childValue));
    //INDEX_TYPE i = 0; //useless, hashInsert it's called from the compressor at most MAX_CHILD times, then the compressor itself calls hashReset
    if(!table[index].empty) (*collision)++;//PER TESTING!!!
    while(!table[index].empty) //collision, find first empty. Slow but it's done only in case of collision
    {
        index = (index + 1)%(MAX_CHILD*2); //TODO lento
        //i++; //useless, hashInsert it's called from the compressor at most MAX_CHILD times, then the compressor itself calls hashReset
        //if(i == MAX_CHILD*2) break; //useless, hashInsert it's called from the compressor at most MAX_CHILD times, then the compressor itself calls hashReset
    }
    //if(table[index].childIndex != ROOT_INDEX) return -1; //useless, hashInsert it's called from the compressor at most MAX_CHILD times, then the compressor itself calls hashReset
    current = &(table[index]);
    current->childIndex = childIndex;
    current->fatherIndex = fatherIndex;
    current->childValue = *childValue;
    current->empty = 0;

    return 0;
}

INDEX_TYPE hashLookup
(
    struct LZ78HashTableEntry* table,
    INDEX_TYPE fatherIndex,
    uint8_t* childValue,
    int* collision
)
{
    if(table == NULL) return -1;
    HASH_INDEX index = hashFunction(fatherIndex, ((INDEX_TYPE)*childValue));
    HASH_INDEX i = 0;
    if((!table[index].empty) && (table[index].childValue  != *childValue || table[index].fatherIndex != fatherIndex)) (*collision)++; //PER TESTING!!!
    while //slow but it's done only in case of collision
    (
        (!table[index].empty) && (table[index].childValue  != *childValue || table[index].fatherIndex != fatherIndex)
    )
    {
        index = (index + 1)%(MAX_CHILD*2); //slow but it's done only in case of collision
        i++;
        if(i == MAX_CHILD*2) return ROOT_INDEX;
    }
    return (table[index].empty)? ROOT_INDEX : table[index].childIndex;
}

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry* table, int* collision)
{
    int i = MAX_CHILD*2;
    uint8_t currentValue = 0;
    if(table != NULL)
    {
        for(; i-- ;) //TODO provare bzero
        {
            table[i].empty = 1;
        }
        for(i = 0 ; i < ROOT_INDEX; i++) //TODO due cicli, peso
        {
            currentValue = (uint8_t)i;  //ascii value equals to index value
            if
            (
                hashInsert
                (
                    table,
                    ROOT_INDEX,
                    &currentValue,
                    (INDEX_TYPE)currentValue, collision
                ) == -1
            ) goto exceptionHandler;
        }
    }
    return table;

    exceptionHandler:
        hashDestroy(table);
        return NULL;
}

inline struct LZ78HashTableEntry* hashCreate(int* collision)
{
    return hashInitialize((struct LZ78HashTableEntry*)malloc(HASH_TABLE_LENGTH), collision);
}

inline struct LZ78HashTableEntry* hashReset(struct LZ78HashTableEntry* table, int* collision)
{
    return hashInitialize(table, collision);
}

void hashDestroy(struct LZ78HashTableEntry* table){
    bzero(table, HASH_TABLE_LENGTH);
    free(table);
}
