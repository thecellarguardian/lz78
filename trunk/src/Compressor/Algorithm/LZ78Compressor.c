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

int compress(FILE* inputFile, FILE* outputFile)
{
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(NULL, 1, -1, outputFile);
    uint8_t readByte[LOCAL_BYTE_BUFFER_LENGTH];
    size_t indexLength = INITIAL_INDEX_LENGTH;
    size_t bufferedBytes;
    int byteIndex = 0;
    struct LZ78HashTableEntry* hashTable;
    INDEX_TYPE childIndex = ROOT_INDEX + 1;
    INDEX_TYPE lookupIndex = ROOT_INDEX;
    INDEX_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    INDEX_TYPE child;
    if(inputFile == NULL || w == NULL)
    {
        errno = EINVAL;
        if(w != NULL) closeBitwiseBufferedFile(w);
        return -1;
    }
    hashTable = hashCreate();
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
    printf("COMPRESSORE ONLINE\n");
    while(!feof(inputFile) && !ferror(inputFile))
    {
        //printf("LA PRINTF CHE NON PRInta\n");
        bufferedBytes = fread(readByte, 1, LOCAL_BYTE_BUFFER_LENGTH, inputFile);
        //printf("Ho letto: %s\n",(char*)&readByte);
        for(byteIndex = 0; byteIndex < bufferedBytes; byteIndex++)
        {
            printf("\nCerco: %u a partire da %i\n",readByte[byteIndex],lookupIndex);
            child = hashLookup(hashTable, lookupIndex, &(readByte[byteIndex]));
            //printf("Era nell'indice: %u\n", child);
            if(child != ROOT_INDEX)
            {
                lookupIndex = child; //TODO giusto usare root_index?
                printf("Trovato qui: %i\n",lookupIndex);
            }
            else
            {
                printf("Non l'ho trovato allora ");
                if //OR short circuit evaluation exploited
                (
                    writeBitBuffer(w, lookupIndex, indexLength) == -1
                    ||
                    hashInsert
                    (
                        hashTable,
                        lookupIndex,
                        &readByte[byteIndex],
                        childIndex
                    ) == -1
                ) goto exceptionHandler;
                printf("ho scritto: %i\n", lookupIndex);
		printf("Ho inserito %s nel figlio: %i\n",&readByte[byteIndex], childIndex);
                childIndex++;
                if((childIndex & indexLengthMask) == 0) //A power of 2 is reached
                {
                    //The length of the transmitted index is incremented
                    indexLength++;
                    //The next power of 2 mask is set
                    indexLengthMask = (indexLengthMask << 1) | 1;
                }
                //readByte value is also the right index to start with next time
                lookupIndex = readByte[byteIndex]; //ROOT_INDEX??
                if (childIndex == MAX_CHILD)
                {
                    hashReset(hashTable); //TODO controllare errore
                    childIndex = ROOT_INDEX + 1;
                }
            }
        }
    }
    if(ferror(inputFile))
    {
        errno = EBADFD;
        goto exceptionHandler; //TODO: siamo sicuri???
    }
    if
    (
        writeBitBuffer(w, lookupIndex, indexLength) == -1 //TODO problema, chi mi dice che quì indexLength non sfora??
        ||
        writeBitBuffer(w, ROOT_INDEX, indexLength) == -1 //TODO sarebbe meglio INITIAL_INDEX_LENGTH
    ) goto exceptionHandler;
    printf("ho scritto: %i\n", lookupIndex);
    printf("ho scritto: %i\n", ROOT_INDEX);
    /*if(lookupIndex != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }*/
    printf("COMPRESSORE OFFLINE\n");
    return closeBitwiseBufferedFile(w);

    /**
     * "With great power there must also come great responsibility!"
     * (Stan Lee)
     **/
    exceptionHandler:
        closeBitwiseBufferedFile(w);
        hashDestroy(hashTable);
        return -1;
}
