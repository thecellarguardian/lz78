CC = gcc
CFLAGS = -O2 -Wall -Werror
BUFFER_CONFIGURATION_FILE = 'EXTERN_BUFFER_CONFIGURATION_FILE="../../src/Configuration/LZ78CompressorConfiguration.h"'
OBJS = ../lib/BitwiseBufferedFile/BitwiseBufferedFile.o ./Configuration/LZ78CompressorConfiguration.o ./Compressor/DataStructures/LZ78HashTable.o ./Decompressor/DataStructures/LZ78DecompressorTable.o ./Compressor/Algorithm/LZ78Compressor.o ./Decompressor/Algorithm/LZ78Decompressor.o main.o
TARGET = lz78

$(TARGET):$(OBJS)                                
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
	
BitwiseBufferedFile.o:    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.c
	$(CC) $(CFLAGS) -D $(BUFFER_CONFIGURATION_FILE) -c ../lib/BitwiseBufferedFile/BitwiseBufferedFile.c
	
LZ78DecompressorTable.o:   \
./Decompressor/DataStructures/LZ78DecompressorTable.c

LZ78Decompressor.o:          \
./Decompressor/Algorithm/LZ78Decompressor.c       \
./Decompressor/DataStructures/LZ78DecompressorTable.c \
./Configuration/LZ78CompressorConfiguration.c  \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.c
	
LZ78HashTable.o:      \
./Compressor/DataStructures/LZ78HashTable.c   \
./Configuration/LZ78CompressorConfiguration.c

LZ78Compressor.o:            \
./Compressor/Algorithm/LZ78Compressor.c         \
./Compressor/DataStructures/LZ78HashTable.c      \
./Configuration/LZ78CompressorConfiguration.c    \
../lib/BitwiseBufferedFile/BitwiseBufferedFile.c

LZ78CompressorConfiguration.o: ./Configuration/LZ78CompressorConfiguration.c
	
main.o: ./Compressor/Algorithm/LZ78Compressor.c     \
./Decompressor/Algorithm/LZ78Decompressor.c main.c

clean:
	-rm *.o $(TARGET)
