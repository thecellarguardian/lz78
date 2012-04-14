#include "Compressor/Algorithm/LZ78Compressor.h"
#include "Decompressor/Algorithm/LZ78Decompressor.h"

int main(){
    FILE* f1 = fopen("rfc4227.txt","r");
//    FILE* f1 = fopen("prova.pdf","r");
    FILE* f2 = fopen("prova2","w+");
    FILE* f3;
    compress(f1, f2);
    fclose(f1);
    fclose(f2);
    f2 = fopen("prova2","r");
    f3 = fopen("prova3","w+");
    decompress(f2,f3);
    fclose(f2);
    fclose(f3);
    return 0;
}
