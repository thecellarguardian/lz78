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

int compress(const char* path)
{
    INDEX_TYPE child_index = ROOT_INDEX + 1;
    INDEX_TYPE search_index = ROOT_INDEX;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(path, 0);
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(path, 1);//Deve essere un altro file!
    if(r == NULL || w == NULL)
    {
        return -1;
    }

    LZ78HashTable* hash_table = hash_initialize();//TODO inizializzazione hash_table

    uint8_t read;
    ssize_t bitsRead = 0;
    while(r->emptyFile == 0)
    {
        bitsRead += readBitBuffer(r, &read, 8);//TODO gestione errori + byte per byte
        int child = hash_lookup(hash_table, search_index, read); //TODO
        if(child != -1){
            search_index = child;
        }
        else{ //read not found in table
            writeBitBuffer(w, search_index, INDEX_LENGTH); //TODO gestione errori
            hash_insert(hash_table, search_index, read, child_index);//TODO funzione
            child_index ++;
            //search_index = ROOT_INDEX;
            search_index = read;
            if (child_index == MAX_CHILD){
              hash_reset(hash_table);//TODO
              child_index = ROOT_INDEX + 1;
            }
        }
    }
    writeBitBuffer(w, search_index, INDEX_LENGTH); //TODO gestione errori
    /*if(search_index != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }*/
    writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH); //Fine file (todo Huffman di tutte le codifiche)
    closeBitwiseBufferedFile(r);

    return bitsRead;
}
