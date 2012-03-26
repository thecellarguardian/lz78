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

int decompress(const char* inputFile, const char* outputFile)
{
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(inputFile, 0);
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(outputFile, 1);//ATTENZIONE: e se fossero socket? generalizzare a descrittore di file
    CELL_TYPE current_index;
    CELL_TYPE child_index = ROOT_INDEX + 1;
    ssize_t bitsRead = 0;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    uint8_t* result;
    ssize_t index = 0;
    CELL_TYPE length = 0;
    
    struct node table [MAX_CHILD];
    
    if(r == NULL || w == NULL)
    {
        return -1;
    }
    
    //TODO inizializzazione table
   
    while(r->emptyFile == 0)
    {
	bitsRead += readBitBuffer(r, &current_index, indexLength);//TODO gestione errori + byte per byte
	if(current_index == ROOT_INDEX){
	    break;
	}
	result = table[current_index].word; //TODO gestione errori
	length = table[current_index].length;
	for(index = 0; index < length; index ++){
	    writeBitBuffer(w, result+index, 8); //TODO FUNZIONE CHE FA A BYTE
	}
	// la prima volta non va fatto
	table[child_index].father = current_index;
	table[child_index].symbol = ***;
	table[child_index].word = ***;
	table[child_index].length = ***;
	//......
	child_index ++;
	if(child_index == MAX_CHILD){
	    table_reset(table);//TODO
            child_index = ROOT_INDEX + 1;
	}
	
    }
    
    return 1;
   
}
