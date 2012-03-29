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


#include LZ78Decompressor.h
#include LZ78CompressorConfiguration.h

struct node {
    CELL_TYPE father;
    uint8_t symbol;
    uint8_t* word;
    CELL_TYPE length; // worst case = tree levels
};

int decompress(const char* inputFile, FILE* outputFile) //Attenzione: generalizzare a FILE*
{
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(inputFile, 0);
    //struct BitwiseBufferedFile* w = openBitwiseBufferedFile(outputFile, 1);//ATTENZIONE: e se fossero socket? generalizzare a descrittore di file
    CELL_TYPE currentIndex;
    CELL_TYPE childIndex = ROOT_INDEX + 1;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    uint8_t* result;
    CELL_TYPE length = 0;
    int notFirstOccurence = 0;
    struct node table [MAX_CHILD];
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
        if(currentIndex == ROOT_INDEX)
        {
            break;
        }
        result = table[currentIndex].word;
        length = table[currentIndex].length;
        if(fwrite(result, 1, length, outputFile) != length)
        {
            errno = EBADFD;
            goto exceptionHandler;
        }
        table[childIndex].father = currentIndex;
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
            table[childIndex - 1].symbol = *result;
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
