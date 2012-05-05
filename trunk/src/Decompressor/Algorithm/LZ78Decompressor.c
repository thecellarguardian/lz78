/**
 * @file LZ78Decompressor.c
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

#include "LZ78Decompressor.h"
#include "../../Configuration/LZ78CompressorConfiguration.h"
#include "../DataStructures/LZ78DecompressorTable.h"
#include "../../../lib/BitwiseBufferedFile/BitwiseBufferedFile.h"
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

inline void preappend
(
    struct LZ78DecompressorTableEntry* current,
    struct LZ78DecompressorTableEntry* ancestor
)
{
    if(current->bufferLength < (current->length + ancestor->length))
    {
        current->bufferLength = current->length + ancestor->length;
        current->word = realloc(current->word, current->bufferLength);
    }
    if(current->length > 0)
    {
        memmove(current->word + ancestor->length, current->word, current->length);
    }
    memcpy(current->word, ancestor->word, ancestor->length);
    current->length += ancestor->length;
}

int decompress(FILE* inputFile, FILE* outputFile)
{
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(NULL, O_RDONLY, -1, inputFile);
    INDEX_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    INDEX_TYPE childIndex = FIRST_CHILD;
    INDEX_TYPE currentIndex = 0;
    INDEX_TYPE length = 0;
    uint8_t* result;
    struct LZ78DecompressorTableEntry* table;
    struct LZ78DecompressorTableEntry* current;
    struct LZ78DecompressorTableEntry* auxilium;
    struct LZ78DecompressorTableEntry* lastChild;
    CELL_TYPE compressionLevel = 0;
    uint32_t maxChild = 0;
    uint8_t* word = NULL;
    if(r == NULL || outputFile == NULL)
    {
        errno = EINVAL;
        if(r != NULL) closeBitwiseBufferedFile(r);
        return -1;
    }
    if
    (
        readBitBuffer(r, &compressionLevel, HEADER_LENGTH) < HEADER_LENGTH
        ||
        !(maxChild = getCompressionParameter(compressionLevel, MAX_CHILD))
        ||
        (table = tableCreate(maxChild)) == NULL
    )
    {
        closeBitwiseBufferedFile(r);
        return -1;
    }
    for(;;)
    {
        currentIndex = 0;
        if((readBitBuffer(r, &currentIndex, indexLength)) < indexLength)
            goto exceptionHandler;
        if(currentIndex == ROOT_INDEX) break;
        //printf("Ho ricevuto %u \n",currentIndex);
        current = &(table[currentIndex]);
        /**
         * The previous child has to be updated with the current leading byte,
         * but not the first time (in that case, no previous child exists).
         **/
        if(childIndex > FIRST_CHILD) //257 is the first child index, not to be updated
        {
            lastChild = &(table[childIndex - 1]);
            if(lastChild->length > 0) lastChild->length = 0;
            if(currentIndex == childIndex - 1)
            {
                auxilium = &(table[lastChild->fatherIndex]);
                while(auxilium->length == 1 && auxilium->fatherIndex != ROOT_INDEX)
                {
                    preappend(lastChild, auxilium);
                    auxilium = &(table[auxilium->fatherIndex]);
                }//index cache
                preappend(lastChild, auxilium);
                lastChild->length++;
                if(lastChild->bufferLength < lastChild->length)
                {
                    lastChild->bufferLength = lastChild->length;
                    lastChild->word = realloc(lastChild->word, lastChild->bufferLength);
                }
                lastChild->word[lastChild->length - 1] =  lastChild->word[0];
               //C'ERA PRIMA SOLO QUESTO lastChild->word[0] = (table[lastChild->fatherIndex].word[0]);
                //printf("Ho inserito %u nel child\n",word[0]);
            }
            else
            {
                if(lastChild->bufferLength == 0)
                {
                    lastChild->bufferLength = 1;
                    lastChild->word = realloc(lastChild->word, 1);
                }
                lastChild->length = 1;
                if(lastChild->word == NULL) goto exceptionHandler;
                if(currentIndex > (FIRST_CHILD - 1) && current->length == 1)
                { //is not one of the first 257 and its word is not complete
                    auxilium = &(table[current->fatherIndex]);
                    while(auxilium->length == 1 && auxilium->fatherIndex != ROOT_INDEX) //TODO doppio ciclo
                    {
                        preappend(current, auxilium);
                        auxilium = &(table[auxilium->fatherIndex]);
                    }//index cache
                    preappend(current, auxilium);
                    lastChild->word[0] = auxilium->word[0];
                    //PRIMA C'ERA SOLO LUI ->  lastChild->word[0] = current->word[0];
                    //printf("Inserisco %c nel child %u\n",current->word[0],childIndex -1);
                }
                else lastChild->word[0] = current->word[0];
            }
        }
        result = current->word;
        length = current->length;
        if(fwrite(result, 1, length, outputFile) != length)
        {
            errno = EBADFD;
            goto exceptionHandler;
        }
        //printf("Ho scritto %c \n",result[0]);
        current = &(table[childIndex]); //Current cambia significato!
        //current->length = 1;
        //current->word = malloc(1);
        current->fatherIndex = currentIndex;
        //if(current->word == NULL) goto exceptionHandler;
        //memcpy(current->word, result, length); //once upon a time, bcopy
        childIndex++;
        if((childIndex & indexLengthMask) == 0)//A power of two is reached
        {
            indexLength++; //The length of the transmitted index is incremented
            indexLengthMask = (indexLengthMask << 1) | 1; //Next power of 2 set
        }
        if(childIndex == maxChild) /*tableReset?;*/ childIndex = FIRST_CHILD;
    }
    closeBitwiseBufferedFile(r);
    tableDestroy(table, maxChild);
    return 0;

    exceptionHandler:
        closeBitwiseBufferedFile(r);
        tableDestroy(table, maxChild);
        return -1;
}
