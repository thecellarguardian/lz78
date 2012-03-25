**
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
    uint32_t next_index = ROOT_INDEX + 1;
    uint32_t current_index = ROOT_INDEX;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(path, 0);
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(path, 1);
    if(r == NULL || w == NULL)
    {
        return -1;
    }
    
    LZ78HashTable* hash_table = hash_initialize();//TODO inizializzazione hash_table
    
    CELL_TYPE read;
    ssize_t bitsRead;
    while(r->emptyFile == 0)
    {
         bitsRead += readBitBuffer(r, &read, CELL_TYPE_LENGTH);//TODO gestione errori
	 int child = hash_lookup(hash_table,current_index,read); //TODO
	 if(child != -1){
	    next_index = child;
	 }
	 else{ //read not found in table
	    writeBitBuffer(w, current_index, INDEX_LENGTH); //TODO gestione errori
	    hash_insert(hash_table,current_index,read,next_index);//TODO
	    next_index ++;
	    current_index = ROOT_INDEX;
	    if (next_index == MAX_CHILD){
	      hash_reset(hash_table);//TODO
	      next_index = ROOT_INDEX + 1;
	    }
	 }
    }
    writeBitBuffer(w, current_index, INDEX_LENGTH); //TODO gestione errori
    if(current_index != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto (è arrivato il fine file e non è stato riconosciuto)
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }
    closeBitwiseBufferedFile(r);

    return bitsRead;
}
