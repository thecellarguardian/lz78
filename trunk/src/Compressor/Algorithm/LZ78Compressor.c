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
#include "../Configuration/LZ78CompressorConfiguration.h"
#include "../../../lib/BitwiseBufferedFile/BitwiseBufferedFile.h"

int compress(FILE* inputFile, FILE* outputFile)//TODO: simmetrizzare i parametri, output sia un FILE*
{
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(NULL, 1, -1, outputFile);
    uint8_t readByte[LOCAL_BYTE_BUFFER_LENGTH];
    size_t indexLength = INITIAL_INDEX_LENGTH;
    size_t bufferedBytes;
    int byteIndex = 0;
    LZ78HashTable* hashTable;
    CELL_TYPE childIndex = ROOT_INDEX + 1;
    CELL_TYPE lookupIndex = ROOT_INDEX;
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    CELL_TYPE child;
    if(inputFile == NULL || w == NULL)
    {
        errno = EINVAL;
        if(w != NULL) closeBitwiseBufferedFile(w);
        return -1;
    }
    hashTable = hashInitialize();
    if(hashTable == NULL)
    {
        closeBitwiseBufferedFile(w);
        return -1;
    }
    while(feof(inputFile) & !ferror(inputFile)) //TODO molte fread!
    {
        bufferedBytes = fread(readByte, 1, 8, inputFile);
        for(byteIndex = 0; byteIndex < bufferedBytes; i++)
        {
            child = hashLookup(hashTable, lookupIndex, readByte[byteIndex]);
            if(child != -1) lookupIndex = child;
            else
            {
                //OR short circuit evaluation exploited in the next statement
                if
                (
                    writeBitBuffer(w, lookupIndex, indexLength) == -1
                    ||
                    hashInsert
                    (
                        hashTable,
                        lookupIndex,
                        readByte[byteIndex],
                        childIndex
                    ) == -1
                ) goto exceptionHandler;
                childIndex++;
                /**
                 * If the number of children reaches the next power of 2, the
                 * related index length must be incremented by one.
                 * In order to index k children, log_2(k) bits are sufficient,
                 * so the index transmitted to the decompressor is log_2(k)
                 * long, no more.
                 **/
                if(childIndex & indexLengthMask == 0) //if the next power of 2 is reached...
                {
                    indexLength++; //...the length of the transmitted index is incremented...
                    indexLengthMask = (indexLengthMask << 1) | 1; //...and the next power of 2 to check is set
                }
                lookupIndex = readByte; //ascii code of readByte is readByte's index. The next lookup starts from readByte.
                if (childIndex == MAX_CHILD)
                {
                    hashReset(hashTable);
                    childIndex = ROOT_INDEX + 1;
                }
            }
        }
    }
    //fine file o c'è stato un errore?
    if(ferror(inputFile))
    {
        errno = EBADFD;
        return -1;
    }
    if
    (
        writeBitBuffer(w, lookupIndex, indexLength) == -1
        ||
        writeBitBuffer(w, ROOT_INDEX, INITIAL_INDEX_LENGTH) == -1
    ) goto exceptionHandler;
    //TODO #! decompressor aaa
    /*if(lookupIndex != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }*/
    closeBitwiseBufferedFile(w);
    return 0;

    /**
     * "With great power there must also come -- great responsibility!" (Stan Lee)
     **/
    exceptionHandler:
        closeBitwiseBufferedFile(w);
        hashDestroy(hashTable);
        return -1;
}
