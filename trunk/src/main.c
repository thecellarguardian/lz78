#include "Compressor/Algorithm/LZ78Compressor.h"
#include "Decompressor/Algorithm/LZ78Decompressor.h"

int main(){
    FILE* f1 = fopen("prova","r");
    FILE* f2 = fopen("prova2","w+");
    FILE* f3 = fopen("prova3","w+");
    printf("\n%i\n",compress(f1,f2));
    fclose(f2);
    f2 = fopen("prova2","r");
    printf("\n%i\n",decompress(f2,f3));
    return 0;
}