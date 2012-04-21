#include "Compressor/Algorithm/LZ78Compressor.h"
#include "Decompressor/Algorithm/LZ78Decompressor.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

#define MIN_COMPRESSION_LEVEL 1
#define DEFAULT_COMPRESSION_LEVEL 3
#define MAX_COMPRESSION_LEVEL 5

const char* license =
"lz78 rev 132\n\
Copyright (C) 2012 Cosimo Sacco and Davide Silvestri.\n\
This is free software.\n\
You may redistribute copies of it under the terms of\n\
the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n\
There is NO WARRANTY, to the extent permitted by law.\n";

void help()
{


}

static int fastFlag = 0;
static int bestFlag = 0;

int main(int argc, char** argv)
{
    int optionToProcess = 0;
    int outputFlag = 0;
    int compressFlag = -1;
    int compressionLevel = 0;
    char* inputFile = NULL;
    char* outputFile = NULL;
    FILE* input;
    FILE* output;
    struct stat inputStat;
    struct stat outputStat;
    int error = 0;
    while(optionToProcess != -1)
    {
        static struct option options[] =
        {
            {"best"      , no_argument      , &bestFlag,  1 },
            {"fast"      , no_argument      , &fastFlag,  1 },
            {"help"      , no_argument      , 0        , 'h'},
            {"license"   , no_argument      , 0        , 'L'},
            {"level"     , required_argument, 0        , 'l'},
            {"compress"  , required_argument, 0        , 'c'},
            {"decompress", required_argument, 0        , 'd'},
            {"output"    , required_argument, 0        , 'o'},
            {0           , 0                , 0        ,  0 }
        };
        int optionIndex = 0;
        optionToProcess = getopt_long(argc, argv, "Ll:c:d:o:", options, &optionIndex);
        switch (optionToProcess)
        {
            case 0:
            case 1:
                if(optarg)
                {
                    fprintf
                    (
                        stderr,
                        "No argument expected for option %s\n",
                        options[optionIndex].name
                    );
                    return 1;
                }
                compressionLevel = (optionToProcess == 1) ? 1 : 5;
                break;
            case 'h': help(); return 0;
            case 'L': printf(license); return 0;
            case 'l':
                compressionLevel = atoi(optarg);
                if
                (
                    compressionLevel < MIN_COMPRESSION_LEVEL
                    ||
                    compressionLevel > MAX_COMPRESSION_LEVEL
                )
                {
                    fprintf(stderr, "Invalid compression level\n");
                    return 1;
                }
                break;
            case 'c':
                inputFile = optarg;
                compressFlag = 1;
                break;
            case 'd':
                inputFile = optarg;
                compressFlag = 0;
                break;
            case 'o':
                outputFile = optarg;
                outputFlag = 1;
            case -1:
                break;
            default:
                abort();
        }
    }
    if (optind < argc)
    {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }
    if(compressFlag == -1) return 0;
    if(compressionLevel == 0) compressionLevel = 3;
    if(outputFile == NULL) outputFile = "out.lz78";
    assert(inputFile != NULL && outputFile != NULL);
    input = fopen(inputFile,"r");
    output = fopen(outputFile,"w+");
    if(input == NULL || output == NULL)
    {
        fprintf(stderr, "Invalid file passed\n");
        return 1;
    }
    error = (compressFlag)? compress(input, output, compressionLevel) : decompress(input, output, compressionLevel);
    fclose(input);
    fclose(output);
    if(compressFlag)
    {
        /*TODO ATTENZIONE! È SBAGLIATO! SE C'È STATA ESPANSIONE, BISOGNA DIRE AL
         * DECOMPRESSORE CHE IL FILE NON È STATO COMPRESSO! ALTRIMENTI LUI
         * CREDE CHE SIA COMPRESSO E CHISSÀ COSA INIZIA A FARE!
         */

        if(!(stat(inputFile, &inputStat) || stat(outputFile, &outputStat)) && (outputStat.st_size > inputStat.st_size))
        {
            //TODO remove(outputFile);
            return 1;
        }
    }
    return error;
}
