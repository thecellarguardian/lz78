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

int decompress(const char* inputFile, FILE* outputFile)
{
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    struct BitwiseBufferedFile* r = openBitwiseBufferedFile(inputFile, 0);
    //struct BitwiseBufferedFile* w = openBitwiseBufferedFile(outputFile, 1);//ATTENZIONE: e se fossero socket? generalizzare a descrittore di file
    CELL_TYPE currentIndex;
    CELL_TYPE childIndex = ROOT_INDEX + 1;
    ssize_t bitsRead = 0;
    size_t indexLength = INITIAL_INDEX_LENGTH;
    uint8_t* result;
    ssize_t index = 0;
    CELL_TYPE length = 0;
    int notFirstOccurence = 0;
    
    struct node table [MAX_CHILD];
    
    if(r == NULL || w == NULL)
    {
        return -1;
    }
    
    tableInitialize(table);//TODO inizializzazione table
   
    while(r->emptyFile == 0)
    {
	bitsRead += readBitBuffer(r, &currentIndex, indexLength);//TODO gestione errori + byte per byte
	if(currentIndex == ROOT_INDEX){
	    break;
	}
	result = table[currentIndex].word; //TODO gestione errori
	length = table[currentIndex].length;
	for(index = 0; index < length; index ++){
	    fwrite(w, result+index, 8); //TODO FUNZIONE CHE FA A BYTE
	}

	table[childIndex].father = currentIndex;
	table[childIndex].length = length + 1;
	table[childIndex].word = malloc(length + 1); //TODO gestire errore
	bcopy(result,table[childIndex].word,length);
	
	//aggiornamento del precedente, la prima volta non va fatto
	if(notFirstOccurence){
	    table[childIndex - 1].symbol = *result;
	    table[childIndex - 1].word[table[childIndex - 1].lenght - 1] = *result;
	}

	childIndex ++;
	if(childIndex == MAX_CHILD){
	    tableReset(table);//TODO
            childIndex = ROOT_INDEX + 1;
	}
	
    }
    
    return 0;
   
}
