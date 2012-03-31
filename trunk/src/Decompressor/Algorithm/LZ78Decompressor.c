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

int decompress(FILE* inputFile, FILE* outputFile) //Attenzione: generalizzare a FILE*
{
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(NULL, 0, -1, inputFile);
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    CELL_TYPE childIndex = ROOT_INDEX + 1;
    int notFirstOccurence = 0;
    CELL_TYPE currentIndex;
    CELL_TYPE length = 0;
    uint8_t* result;
    struct Node table[MAX_CHILD];
    if(r == NULL || outputFile == NULL)
    {
        errno = EINVAL;
        if(r != NULL) closeBitwiseBufferedFile(r);
        return -1;
    }
    tableInitialize(table);//TODO inizializzazione table, gestione errori!
    while(r->emptyFile == 0)
    {
        if(readBitBuffer(r, &currentIndex, indexLength) == -1) goto exceptionHandler;
        if(currentIndex == ROOT_INDEX) break;
        result = table[currentIndex].word;
        length = table[currentIndex].length;
        if(fwrite(result, 1, length, outputFile) != length)
        {
            errno = EBADFD;
            goto exceptionHandler;
        }
        //table[childIndex].father = currentIndex;
        table[childIndex].length = length + 1;
        table[childIndex].word = malloc(length + 1);
        if(table[childIndex].word == NULL) goto exceptionHandler;
        bcopy(result,table[childIndex].word,length); //DEPRECATED
        /**
         * The previous child has to be updated with the current leading byte,
         * but not the first time (in that case, no previous child exists).
         **/
        if(notFirstOccurence)
        {
            //table[childIndex - 1].symbol = *result;
            table[childIndex - 1].word[table[childIndex - 1].lenght - 1] = *result;
        }
        childIndex++;
        if(childIndex == MAX_CHILD)
        {
            tableReset(table);
            childIndex = ROOT_INDEX + 1;
        }
    }
    return 0;

    exceptionHandler:
        closeBitwiseBufferedFile(r);
        tableDestroy(table);
        return -1;
}
