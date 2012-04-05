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
    INDEX_TYPE fatherIndex;
    uint8_t childValue;
    //value:
    INDEX_TYPE childIndex;
};

INDEX_TYPE hashFunction(INDEX_TYPE key1, INDEX_TYPE key2){ //SAX hash function
    INDEX_TYPE index = 0;
    key1 ^= key2; //TODO migliorare
    uint8_t i = sizeof(INDEX_TYPE);
    for (; i -- ;)
      index ^= ( index << 5 ) + ( index >> 2 ) + &key1[i]; 
    return index;
}

LZ78HashTableEntry* hashInitialize(){
    LZ78HashTableEntry* tableToReturn = malloc(HASH_TABLE_LENGTH); //TODO vediamo come ottimizzare
    int i = HASH_TABLE_LENGTH - 1;
    uint8_t currentValue = ROOT_INDEX - 1;
    if(tableToReturn != NULL)
    {
	for(; i-- ;){
	    if(hashInsert(tableToReturn, ROOT_INDEX, NULL, ROOT_INDEX) == -1)
		goto exceptionHandler;
	}
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

int hashInsert(LZ78HashTableEntry* table, INDEX_TYPE fatherIndex, uint8_t* childValue, INDEX_TYPE childIndex){ //TODO inline??
    LZ78HashTableEntry* result = table[hashFunction(table,fatherIndex,*childValue)]; //TODO o si fa per valore o si passa anche fatherIndex per indirizzo
    if(result->childIndex != ROOT_INDEX)
	return -1; //collisione
    result->childIndex = childIndex;
    result->fatherIndex = fatherIndex;
    result->childValue = *childValue;
    return 0;
}

INDEX_TYPE hashLookup(LZ78HashTableEntry* table, INDEX_TYPE fatherIndex, uint8_t* childValue){ //TODO inline?
    LZ78HashTableEntry* result = table[hashFunction(table,fatherIndex,*childValue)];
    if(result->childValue == *childValue && result->fatherIndex == fatherIndex)
	return result->childIndex;
    return ROOT_INDEX; //TODO come notificare collisione != non presente ??
}

int hashReset(LZ78HashTableEntry* table){ //TODO metodo + efficiente??   
    int i = HASH_TABLE_LENGTH - 1;
    uint8_t currentValue = ROOT_INDEX - 1;
    
    for(; i-- ;){
	if(hashInsert(tableToReturn, ROOT_INDEX, NULL, ROOT_INDEX) == -1)
	    goto exceptionHandler;
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

void hashDestroy(LZ78HashTableEntry* table){
    bzero(table, HASH_TABLE_LENGTH);
    free(table);
}
