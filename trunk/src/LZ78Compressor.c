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


#include LZ78Compressor.h
#include LZ78CompressorConfiguration.h

int compress(const char* inputFile, const char* outputFile)
{
    CELL_TYPE child_index = ROOT_INDEX + 1;
    CELL_TYPE lookup_index = ROOT_INDEX;
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    CELL_TYPE child;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(inputFile, 0);
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(outputFile, 1);//ATTENZIONE: e se fossero socket? generalizzare a descrittore di file
    size_t indexLength = INITIAL_INDEX_LENGTH;
    uint8_t read;
    ssize_t bitsRead = 0;
    LZ78HashTable* hash_table;
    if(r == NULL || w == NULL)
    {
        return -1;
    }
    hash_table = hash_initialize();//TODO inizializzazione hash_table
    while(r->emptyFile == 0)
    {
        bitsRead += readBitBuffer(r, &read, 8);//TODO gestione errori + byte per byte
        child = hash_lookup(hash_table, lookup_index, read); //TODO
        if(child != -1){
            lookup_index = child;
        }
        else{ //read not found in table
            writeBitBuffer(w, lookup_index, indexLength); //TODO gestione errori
            hash_insert(hash_table, lookup_index, read, child_index);//TODO funzione
            child_index++;
            /**
             * If the number of children reaches the next power of 2, the
             * related index length must be incremented by one.
             * In order to index k children, log_2(k) bits are sufficient, so
             * the index transmitted to the decompressor is log_2(k) long, no
             * more.
             **/
            if(child_index & indexLengthMask == 0) //if the next power of 2 is reached...
            {
                indexLength++; //...the length of the transmitted index is incremented...
                indexLengthMask = (indexLengthMask << 1) | 1; //...and the next power of 2 to check is set
            }
            //lookup_index = ROOT_INDEX;
            lookup_index = read; //ascii code of read is read's index. next lookup starts from read.
            if (child_index == MAX_CHILD){
              hash_reset(hash_table);//TODO
              child_index = ROOT_INDEX + 1;
            }
        }
    }
    writeBitBuffer(w, lookup_index, indexLength); //TODO gestione errori
    /*if(lookup_index != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }*/
    writeBitBuffer(w, ROOT_INDEX, INITIAL_INDEX_LENGTH); //Fine file
    closeBitwiseBufferedFile(r);

    return bitsRead;
}
