CC = gcc
CFLAGS = -O2 -Wall -Werror -pg -ggdb
BUFFER_CONFIGURATION_FILE = 'EXTERN_BUFFER_CONFIGURATION_FILE="../../src/Configuration/LZ78CompressorConfiguration.h"'

test:                                             \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.o    \
./Compressor/DataStructures/LZ78HashTable.o      \
./Decompressor/DataStructures/DecompressorTable.o   \
./Compressor/Algorithm/LZ78Compressor.o            \
./Decompressor/Algorithm/LZ78Decompressor.o          \
main.o
	$(CC) -S -o $@ -g $^
	
../lib/BitwiseBufferedFile/BitwiseBufferedFile.o:    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h \
../lib/utilities/mathUtils.h
	$(CC) -D $(BUFFER_CONFIGURATION_FILE) -c
	
./Decompressor/DataStructures/DecompressorTable.o:   \
./Decompressor/DataStructures/DecompressorTable.h

./Decompressor/Algorithm/LZ78Decompressor.o:          \
./Decompressor/Algorithm/LZ78Decompressor.h       \
./Decompressor/DataStructures/DecompressorTable.h \
./Configuration/LZ78CompressorConfiguration.h     \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h
	
./Compressor/DataStructures/LZ78HashTable.o:      \
./Compressor/DataStructures/LZ78HashTable.h   \
./Configuration/LZ78CompressorConfiguration.h

./Compressor/Algorithm/LZ78Compressor.o:            \
./Compressor/Algorithm/LZ78Compressor.h          \
./Compressor/DataStructures/LZ78HashTable.h      \
./Configuration/LZ78CompressorConfiguration.h    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h
	
main.o:                                         \
./Compressor/Algorithm/LZ78Compressor.h     \
./Decompressor/Algorithm/LZ78Decompressor.h

debug:
	-make && ./testbin && xxd -b provaRead && xxd -b provaWrite

clean:
	-rm *.o *.out
