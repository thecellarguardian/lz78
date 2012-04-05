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

#include "LZ78HashTable.h"

struct LZ78HashTableEntry
{
    //key:
    INDEX_TYPE father;
    uint8_t childValue;
    //value:
    INDEX_TYPE childIndex;
};


/*
struct LZ78HashTableEntry* hashInitialize(){
    struct LZ78HashTableEntry* tableToReturn = calloc(HASH_TABLE_LENGTH, sizeof(struct LZ78HashTableEntry));
    uint8_t i = ROOT_INDEX-1;
    if(tableToReturn != NULL)
    {
        for(; i--;)
        {
	    if(hashInsert(tableToReturn, ROOT_INDEX, &i, i))
		goto exceptionHandler;
        }
    }
    return tableToReturn;
    
    exceptionHandler:
	hashDestroy();
	return NULL;
}

INDEX_TYPE hashLookup(struct LZ78HashTableEntry*, INDEX_TYPE, uint8_t*);

int hashInsert(struct LZ78HashTableEntry* table, INDEX_TYPE father, uint8_t* childValue, INDEX_TYPE childIndex);

int hashReset(struct LZ78HashTableEntry*);

void hashDestroy(struct LZ78HashTableEntry*);*/


LZ78HashTableEntry* hashInitialize(){
    LZ78HashTableEntry* tableToReturn = malloc(HASH_TABLE_LENGTH);
    //int i = HASH_TABLE_LENGTH - 1;
    uint8_t currentValue = ROOT_INDEX - 1;
    if(tableToReturn != NULL)
    {
        for(; currentValue--;) //i caratteri ascii coincidono con il loro indice
        {
	    if(hashInsert(tableToReturn, ROOT_INDEX, &currentValue, currentValue) == -1)
		goto exceptionHandler;
        }
    }
    return tableToReturn;
    
    exceptionHandler:
	hashDestroy();
	return NULL;
}

//TODO if table!= null ovunque??

int hashInsert(INDEX_TYPE* table, INDEX_TYPE fatherIndex, uint8_t* childValue, INDEX_TYPE childIndex){
    if(table[fHash(fatherIndex,childValue)] != ROOT_INDEX)
	return -1; //collisione
    table[fHash(fatherIndex,childValue)] = childIndex; //TODO inline??
    return 0;
}

INDEX_TYPE hashLookup(INDEX_TYPE* LZ78HashTable, INDEX_TYPE fatherIndex, uint8_t* childValue){ //TODO inline?
    return fHash(fatherIndex,childValue);
}

int hashReset(INDEX_TYPE*){ //TODO metodo + efficiente??   
    int i = HASH_TABLE_LENGTH - 1;
    uint8_t currentValue = ROOT_INDEX - 1;
    
    for(; i--;)
     {
	tableToReturn[i] = ROOT_INDEX; //inizializza a un valore di default = nodo vuoto
     }
     for(; currentValue--;) //i caratteri ascii coincidono con il loro indice
     {
	if(hashInsert(tableToReturn, ROOT_INDEX, &currentValue, currentValue) == -1)
	    goto exceptionHandler;
     }    
     
    return 0;
      
    exceptionHandler:
	hashDestroy();
	return -1;
}

void hashDestroy(INDEX_TYPE* table){
    bzero(table, HASH_TABLE_LENGTH);
    free(table);
}
