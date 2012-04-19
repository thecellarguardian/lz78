#include "Compressor/Algorithm/LZ78Compressor.h"
#include "Decompressor/Algorithm/LZ78Decompressor.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv){
    char* inputFile = NULL;
    char* outputFile = NULL;
    int opt = -1;
    int index;
    int command;
    int error = 0;
    int statError = 0;
    struct stat inputStat;
    struct stat outputStat;
    opterr = 0;
    while((command = getopt (argc, argv, "c:d:o:")) != -1)
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
                if (optopt == 'c' || optopt == 'd' || optopt == 'o')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }
    FILE* f1 = fopen(inputFile,"r");
    FILE* f2 = fopen(outputFile,"w+");
    if(f1 == NULL || f2 == NULL) return 1;
    if(opt == 0)
    {
        error = compress(f1, f2);
        statError = min(stat(inputFile, &inputStat), stat(outputFile, &outputStat));
        if(!statError && (outputStat.st_size > inputStat.st_size))
        {
            remove(outputFile);
            return 1;
        }
    }
    else if(opt == 1) error = decompress(f1, f2);
    else return 1;
    fclose(f1);
    fclose(f2);
    return error || statError;
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
