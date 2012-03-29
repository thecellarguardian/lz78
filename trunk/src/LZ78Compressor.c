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

int compress(FILE* inputFile, const char* outputFile)//TODO: simmetrizzare i parametri, output sia un FILE*
{
    CELL_TYPE childIndex = ROOT_INDEX + 1;
    CELL_TYPE lookupIndex = ROOT_INDEX;
    CELL_TYPE indexLengthMask = INDEX_LENGTH_MASK;
    CELL_TYPE child;
    struct BitwiseBufferedFile* w = openBitwiseBufferedFile(outputFile, 1);//ATTENZIONE: e se fossero socket? generalizzare a descrittore di file
    size_t indexLength = INITIAL_INDEX_LENGTH;
    uint8_t readByte;
    //size_t readBits = 0;
    LZ78HashTable* hashTable;
    if(inputFile == NULL || w == NULL)
    {
        errno = EINVAL;
	if(w != NULL) closeBitwiseBufferedFile(w);
        return -1;
    }
    hashTable = hashInitialize();
    if(hashTable == NULL){
	closeBitwiseBufferedFile(w);
	return -1;
    }
    while(fread(&readByte, 1, 1, inputFile))
    {
        child = hash_lookup(hashTable, lookupIndex, readByte);
        if(child != -1) lookupIndex = child;
        else
        { 
	    //readByte not found in table
            if(writeBitBuffer(w, lookupIndex, indexLength) == -1){
		goto exceptionHandler;
	    }
	    if(hashInsert(hashTable, lookupIndex, readByte, childIndex) == -1){
		goto exceptionHandler;
	    }
            childIndex++;
            /**
             * If the number of children reaches the next power of 2, the
             * related index length must be incremented by one.
             * In order to index k children, log_2(k) bits are sufficient, so
             * the index transmitted to the decompressor is log_2(k) long, no
             * more.
             **/
            if(childIndex & indexLengthMask == 0) //if the next power of 2 is reached...
            {
                indexLength++; //...the length of the transmitted index is incremented...
                indexLengthMask = (indexLengthMask << 1) | 1; //...and the next power of 2 to check is set
            }
           
            lookupIndex = readByte; //ascii code of read is read's index. next lookup starts from read.
            if (childIndex == MAX_CHILD)
            {
              hashReset(hashTable);
              childIndex = ROOT_INDEX + 1;
            }
        }
    }
    //fine file o c'è stato un errore?
    if(feof(inputFile) == 0){
	errno = EBADFD;
	return -1;
    }

    if(writeBitBuffer(w, lookupIndex, indexLength) == -1){
	goto exceptionHandler;
    }//TODO #! decompressor aaa
    /*if(lookupIndex != ROOT_INDEX){ //se non era il fine file ma l'ultimo simbolo non riconosciuto
       writeBitBuffer(w, ROOT_INDEX, INDEX_LENGTH);
    }*/
    if(writeBitBuffer(w, ROOT_INDEX, INITIAL_INDEX_LENGTH) == -1){
	goto exceptionHandler;
    }//Fine file
    closeBitwiseBufferedFile(w);
    return 0;
    
    exceptionHandler:
	closeBitwiseBufferedFile(w);
	hashDestroy(hashTable);
	return -1;
}
