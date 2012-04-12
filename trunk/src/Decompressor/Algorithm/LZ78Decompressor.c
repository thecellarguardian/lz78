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
#include "../DataStructures/DecompressorTable.h"
#include "../../../lib/BitwiseBufferedFile/BitwiseBufferedFile.h"
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int decompress(FILE* inputFile, FILE* outputFile)
{
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(NULL, O_RDONLY, -1, inputFile);
    INDEX_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    INDEX_TYPE childIndex = ROOT_INDEX + 1;
    INDEX_TYPE currentIndex = 0;
    INDEX_TYPE length = 0;
    uint8_t* result;
    struct Node* table;
    struct Node* current;
    if(r == NULL || outputFile == NULL)
    {
        errno = EINVAL;
        if(r != NULL) closeBitwiseBufferedFile(r);
        return -1;
    }
    table = tableCreate();
    if(table == NULL)
    {
        closeBitwiseBufferedFile(r);
        return -1;
    }
   // printf("INIZIO DECOMPRESSIONE\n");
    while(!emptyFile(r))
    {
    currentIndex = 0;
        if((readBitBuffer(r, &currentIndex, indexLength)) < indexLength)
        {
            //printf("sono stati letti meno di %i bit", indexLength);
            goto exceptionHandler;
        }
        // printf("\nho letto %u\n", currentIndex);
        if(currentIndex == ROOT_INDEX) break;
        current = &(table[currentIndex]);
         /**
         * The previous child has to be updated with the current leading byte,
         * but not the first time (in that case, no previous child exists).
         **/
        if(childIndex > 257)
        {
        struct Node* lastChild = &(table[childIndex - 1]);
            lastChild->word[lastChild->length] = current->word[0];
        lastChild->length++;
           // printf("aggiorno con %u il figlio %i\n",current->word[0],childIndex-1);
        }
        result = current->word;
        length = current->length;
        if(fwrite(result, 1, length, outputFile) != length)
        {
            //printf("errore in scrittura\n\n");
            errno = EBADFD;
            goto exceptionHandler;
        }
       // printf("ho scritto %s\n",result);
        current = &(table[childIndex]);
        current->length = length;
        current->word = malloc(length + 1);
        if(current->word == NULL)
        {
            //printf("fallisce la malloc\n\n");
            goto exceptionHandler;
        }
        bcopy(result,current->word, length);
        //printf("ho creato il figlio %i\n",childIndex);
        //stava quì
        childIndex++;
        if((childIndex & indexLengthMask) == 0) //A power of 2 is reached
        {
            //The length of the transmitted index is incremented
            indexLength++;
            //The next power of 2 mask is set
            indexLengthMask = (indexLengthMask << 1) | 1;
           // printf("aumento la lunghezza dell'indice \n");
        }
        if(childIndex == MAX_CHILD)
        {
            tableReset(table);
            childIndex = ROOT_INDEX + 1;
           // printf("reset della tabella\n");
        }
    }
    //printf("\nho letto FINE FILE\n");
    //printf("FINE DECOMPRESSIONE\n");
    closeBitwiseBufferedFile(r);
    tableDestroy(table);
    return 0;

    exceptionHandler:
        closeBitwiseBufferedFile(r);
        tableDestroy(table);
        return -1;
}
