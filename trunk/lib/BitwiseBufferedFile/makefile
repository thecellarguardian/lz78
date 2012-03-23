CC = gcc
CFLAGS = -O2 -Wall -Werror -pg -ggdb

testbin: BitwiseBufferedFile.o 
	$(CC) -o $@ -g $^
	
BitwiseBufferedFile.o: BitwiseBufferedFile.h ../utilities/mathUtils.h

test:
	-make && ./testbin && xxd -b provaRead && xxd -b provaWrite

clean:
	-rm *.o *.out
