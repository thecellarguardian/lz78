#include "LZ78CompressorConfiguration.h"
#include <errno.h>

const uint32_t compressionLevelMatrix [5][2] =
{
        {65535, 131072},  //2^17
        {262143, 524288},  //2^19
        {1048575, 2097152},  //2^21
        {4194303, 8388608}, //2^23
        {16777215, 33554432} //2^25 
};

inline uint32_t getCompressionParameter(int compressionLevel, int parameter){
    return (compressionLevel > 0 && compressionLevel < 6 &&
        (parameter == MAX_CHILD || parameter == HASH_TABLE_ENTRIES)) ?
        compressionLevelMatrix[compressionLevel-1][parameter] : ((errno = EINVAL) && 0);
} 
        
