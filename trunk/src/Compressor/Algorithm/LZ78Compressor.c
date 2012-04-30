/**
 * @file LZ78Compressor.c
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

#include "LZ78Compressor.h"
#include "../DataStructures/LZ78HashTable.h"
#include "../../Configuration/LZ78CompressorConfiguration.h"
#include "../../../lib/BitwiseBufferedFile/BitwiseBufferedFile.h"
#include <errno.h>

int compress(FILE* inputFile, FILE* outputFile, int compressionLevel)
{
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(NULL, 1, -1, outputFile);
    uint8_t readByte[LOCAL_BYTE_BUFFER_LENGTH];
    size_t indexLength = INITIAL_INDEX_LENGTH;
    size_t bufferedBytes;
    int byteIndex = 0;
    struct LZ78HashTableEntry* hashTable;
    INDEX_TYPE childIndex = 257;
    INDEX_TYPE lookupIndex = ROOT_INDEX;
    INDEX_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    INDEX_TYPE child;
    uint32_t hashTableEntries = getCompressionParameter(compressionLevel, HASH_TABLE_ENTRIES);
    uint32_t hashTableLength = hashTableEntries*sizeof(struct LZ78HashTableEntry);
    uint32_t moduloMask = getCompressionParameter(compressionLevel, HASH_TABLE_ENTRIES_MODULO_MASK);
    uint32_t maxChild = getCompressionParameter(compressionLevel, MAX_CHILD);
    int collision = 0; //TESTING
    if(!(maxChild && hashTableEntries && moduloMask) || inputFile == NULL || w == NULL)
    {
        printf("%u", HASH_TABLE_ENTRIES_MODULO_MASK);
        errno = EINVAL;
        if(w != NULL) closeBitwiseBufferedFile(w);
        return -1;
    }
    hashTable = hashCreate(hashTableLength, moduloMask, &collision);
    if(hashTable == NULL)
    {
        closeBitwiseBufferedFile(w);
        return -1;
    }
    /*TODO DECOMMENTARE!!!
    if //OR short circuit exploited to write the interpreter directive
    (
        fwrite(LZ78_INTERPRETER, 1, sizeof(LZ78_INTERPRETER), outputFile) < sizeof(LZ78_INTERPRETER)
        ||
        fflush(outputFile) == EOF
    ) goto exceptionHandler;
    */
    //  printf("COMPRESSORE ONLINE\n");
    if(writeBitBuffer(w, (CELL_TYPE)compressionLevel, 3) == -1) goto exceptionHandler;
    while(!feof(inputFile) && !ferror(inputFile))
    {
        bufferedBytes = fread(readByte, 1, LOCAL_BYTE_BUFFER_LENGTH, inputFile);
        for(byteIndex = 0; byteIndex < bufferedBytes; byteIndex++)
        {
           // printf("\nCerco: %u a partire da %i\n",readByte[byteIndex],lookupIndex);
            child = hashLookup(hashTable, lookupIndex, readByte[byteIndex], moduloMask, &collision);
            if(child != ROOT_INDEX) //ROOT_INDEX means NOT FOUND
            {
                lookupIndex = child;
              //  printf("Trovato qui: %i\n",lookupIndex);
            }
            else
            {
              //  printf("Non l'ho trovato allora ");
                if //OR short circuit evaluation exploited
                (
                    writeBitBuffer(w, lookupIndex, indexLength) == -1
                    ||
                    hashInsert
                    (
                        hashTable,
                        lookupIndex,
                        readByte[byteIndex],
                        childIndex,
                        moduloMask,
                        &collision
                    ) == -1
                ) goto exceptionHandler;
                //  printf("ho scritto: %i\n", lookupIndex);
                //printf("Ho inserito %s nel figlio: %i\n",&readByte[byteIndex], childIndex);
                childIndex++;
                if((childIndex & indexLengthMask) == 0) //A power of 2 is reached
                {
                    //The length of the transmitted index is incremented
                    indexLength++;
                    //The next power of 2 mask is set
                    indexLengthMask = (indexLengthMask << 1) | 1;
                }
                //readByte value is also the right index to start with next time
        //because you have to start from the last character recognized
                lookupIndex = readByte[byteIndex] + 1;
                if (childIndex == maxChild) //hash table is full
                {
                    if(hashReset(hashTable, hashTableLength, moduloMask, &collision) == NULL)
                        goto exceptionHandler; //hash table was not successfully created
                    childIndex = 257; //starts from the beginning
                }
            }
        }
    }
    if(ferror(inputFile))
    {
        errno = EBADFD;
        goto exceptionHandler;
    }
    if
    (
        writeBitBuffer(w, lookupIndex, indexLength) == -1
        ||
        writeBitBuffer(w, ROOT_INDEX, indexLength) == -1
    ) goto exceptionHandler;
    //printf("ho scritto: %i\n", lookupIndex);
    // printf("ho scritto: %i\n", ROOT_INDEX);
    // printf("COMPRESSORE OFFLINE\n");
    printf("COLLSIONI: %i\n",collision);
    hashDestroy(hashTable, hashTableLength);
    return closeBitwiseBufferedFile(w);

    exceptionHandler:
        closeBitwiseBufferedFile(w);
        hashDestroy(hashTable, hashTableLength);
        return -1;
}
