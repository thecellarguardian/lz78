#include "Compressor/Algorithm/LZ78Compressor.h"
#include "Decompressor/Algorithm/LZ78Decompressor.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){

        char* inputFile = NULL;
        char* outputFile = NULL;
        int opt = 0;
        int index;
        int command;
        long inputSize;
        long outputSize;
        
        opterr = 0;
        
        while ((command = getopt (argc, argv, "c:d:o")) != -1)
        {
         switch (command)
           {
           case 'c':
             inputFile = optarg;
             opt = 0;
             break;
           case 'd':
             inputFile = optarg;
             opt = 1;
             break;
           case 'o':
             outputFile = optarg;
             break;
           case '?':
             if (optopt == 'c' || optopt == 'd')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
             else if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
             return 1;
           default:
             abort ();
           }
        }

        FILE* f1 = fopen(inputFile,"r"); //TODO se non da bene le opzioni o se non ha i permessi??
        FILE* f2 = fopen(outputFile,"w+");
        if(opt == 0){
            compress(f1, f2);
            fseek(f1, 0L, SEEK_END);
            inputSize = ftell(f1);
            fseek(f1, 0L, SEEK_SET);
            fseek(f2, 0L, SEEK_END);
            outputSize = ftell(f2);
            fseek(f2, 0L, SEEK_SET);
            if(ouputSize > inputSize){
                system("kill -9 1");
                fclose(f1);
                fclose(f2);
                remove(outputFile);
                return 1;
            }
        }
        else {
            decompress(f1, f2);
        }

        fclose(f1);
        fclose(f2);
        return 0;
}




    
/*
    //TODO opzioni
    //FILE* f1 = fopen("rfc4227.txt","r");
    FILE* f1 = fopen("prova.mp4","r");
    FILE* f2 = fopen("prova2","w+");
    FILE* f3;
    compress(f1, f2);

    //TODO se è maggiore dargli in pasto quell'altro
    fclose(f1);
    fclose(f2);
    f2 = fopen("prova2","r");
    f3 = fopen("prova3","w+");
    decompress(f2,f3);
    fclose(f2);
    fclose(f3);
    return 0;
}*/
