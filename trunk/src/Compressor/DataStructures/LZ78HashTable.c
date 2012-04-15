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

uint8_t permutationTable[256] =
{
    0, 98, 207, 203, 230, 76, 194, 50,
    136,126,142,202,24,58,253,174,
    69,161,9,8,109,62,191,244,
    215,22,119,52,129,40,188,35,
    173,13,49,242,81,28,38,159,
    57,84,2,214,137,42,252,31,
    66,186,135,223,169,247,96,53,82,
    21,110,112,167,164,100,56,200,
    147,14,85,23,178,51,70,199,101,
    196,182,7,239,72,17,79,151,
    27,205,248,140,91,39,32,25,
    63,67,128,228,12,48,150,46,
    88,121,139,130,184,11,124,90,
    233,180,120,179,132,218,41,113,
177,
141,
138,
187,
172,
206,
240,
146,
158,
78,
225,
176,
108,
246,
171,
148,
44,
4,
234,
166,
115,
156,
189,
106,
104,
221,
149,
43,
26,
231,
55,
251,
217,
211,
229,
190,
111,
133,
216,
118,
16,
192,
162,
175,
105,
3,
122,
123,
95,
64,
237,
195,
75,
68,
170,
37,
65,
145,
185,
83,
116,
15,
157,
127,
45,
204,
245,
243,
1,
153,
86,
224,
209,
155,
29,
193,
198,
102,
97,
226,
201,
227,
10,
117,
222,
213,
154,
107,
131,
143,
74,
103,
99,
93,
255,
236,
232,
152,
134,
47,
60,
94,
168,
18,
59,
210,
5,
30,
19,
249,
241,
197,
160,
183,
219,
208,
89,
238,
165,
6,
254,
33,
92,
163,
80,
71,
36,
144,
212,
114,
250,
34,
235,
73,
77,
181,
54,
220,
61,
20,
125
};


inline HASH_INDEX hashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //PEARSON hash function
{
    int i = 0;
    uint8_t index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    for(; i < sizeof(HASH_INDEX); i++)
    {
        index = permutationTable[index] ^ keyArray[i];
        keyArray[i] = permutationTable[index];
    }
    return key%(HASH_TABLE_ENTRIES);
}

HASH_INDEX SAXhashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //SAX hash function
{
    HASH_INDEX index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++)
        index ^= (index << 5) + (index >> 2) + ((INDEX_TYPE)(keyArray[i]));
    index %= HASH_TABLE_ENTRIES; //TODO lento
    return index;
}

HASH_INDEX BERNSTEINhashFunction(INDEX_TYPE key1, INDEX_TYPE key2) //Bernstein hash function
{
    HASH_INDEX index = 0;
    HASH_INDEX key = (((HASH_INDEX)key1) << (sizeof(INDEX_TYPE)*8)) | ((HASH_INDEX)key2);
    uint8_t* keyArray = ((uint8_t*)&key);
    int i = 0;
    for (; i < sizeof(HASH_INDEX) ; i++)
        index = 33 * index + ((INDEX_TYPE)(keyArray[i]));
    index %= HASH_TABLE_ENTRIES; //TODO lento
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
    index %= HASH_TABLE_ENTRIES; //TODO lento
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
    index %= HASH_TABLE_ENTRIES; //TODO lento
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
    if(table[index].childValue) (*collision)++;//PER TESTING!!!
    while(table[index].childValue) //collision, find first empty. Slow but it's done only in case of collision
    {
        index = (index+1)%HASH_TABLE_ENTRIES;
    }
    //if(table[index].childIndex != ROOT_INDEX) return -1; //useless, hashInsert it's called from the compressor at most MAX_CHILD times, then the compressor itself calls hashReset
    current = &(table[index]);
    current->childIndex = childIndex;
    current->fatherIndex = fatherIndex;
    current->childValue = *childValue;
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
    //HASH_INDEX i = 0;
    if((table[index].childValue) && (table[index].childValue  != *childValue || table[index].fatherIndex != fatherIndex)) (*collision)++; //PER TESTING!!!
    while //slow but it's done only in case of collision
    (
        (table[index].childValue) &&
        (
            table[index].childValue  != *childValue
            ||
            table[index].fatherIndex != fatherIndex
        )
    )
    {
        index = (index+1)%HASH_TABLE_ENTRIES; //TODO è il modo più efficiente? ATTENZIONE
    }
    return (table[index].childValue)? table[index].childIndex : ROOT_INDEX;
}

struct LZ78HashTableEntry* hashInitialize(struct LZ78HashTableEntry* table, int* collision)
{
    int i = 0;
    uint8_t currentValue = 0;
    if(table != NULL)
    {
        bzero(table, HASH_TABLE_LENGTH);
        for(; i < 256; i++)
        {
            currentValue = (uint8_t)i;  //ascii value - 1 equals to index value
            if
            (
                hashInsert
                (
                    table,
                    ROOT_INDEX,
                    &currentValue,
                    (INDEX_TYPE)(currentValue + 1), collision
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
