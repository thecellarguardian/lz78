CC = gcc
CFLAGS = -O2 -Wall -Werror
BUFFER_CONFIGURATION_FILE = 'EXTERN_BUFFER_CONFIGURATION_FILE="../../src/Configuration/LZ78CompressorConfiguration.h"'
OBJS = ../lib/BitwiseBufferedFile/BitwiseBufferedFile.o ./Configuration/LZ78CompressorConfiguration.o ./Compressor/DataStructures/LZ78HashTable.o ./Decompressor/DataStructures/LZ78DecompressorTable.o ./Compressor/Algorithm/LZ78Compressor.o ./Decompressor/Algorithm/LZ78Decompressor.o main.o
TARGET = lz78
	
BitwiseBufferedFile.o:    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h \
../lib/utilities/mathUtils.h
	$(CC) $(CFLAGS) -D $(BUFFER_CONFIGURATION_FILE) -c ../lib/BitwiseBufferedFile/BitwiseBufferedFile.c
	
LZ78DecompressorTable.o:   \
./Decompressor/DataStructures/LZ78DecompressorTable.h

LZ78Decompressor.o:          \
./Decompressor/Algorithm/LZ78Decompressor.h       \
./Decompressor/DataStructures/LZ78DecompressorTable.h \
./Configuration/LZ78CompressorConfiguration.h     \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h
	
LZ78HashTable.o:      \
./Compressor/DataStructures/LZ78HashTable.h   \
./Configuration/LZ78CompressorConfiguration.h

LZ78Compressor.o:            \
./Compressor/Algorithm/LZ78Compressor.h          \
./Compressor/DataStructures/LZ78HashTable.h      \
./Configuration/LZ78CompressorConfiguration.h    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.h

LZ78CompressorConfiguration.o: ./Configuration/LZ78CompressorConfiguration.h
	
main.o:                                         \
./Compressor/Algorithm/LZ78Compressor.h     \
./Decompressor/Algorithm/LZ78Decompressor.h

$(TARGET):$(OBJS)                                
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

clean:
	-rm *.o $(TARGET)
