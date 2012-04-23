#include "LZ78CompressorConfiguration.h"
#include <errno.h>

const uint32_t compressionLevelMatrix [5][3] =
{
        //MAX_CHILD, HASH_TABLE_ENTRIES, HASH_TABLE_ENTRIES_MODULO_MASK
        {65535   , 131072  , 131071  },  //2^17
        {262143  , 524288  , 524287  },  //2^19
        {1048575 , 2097152 , 2097151 },  //2^21
        {4194303 , 8388608 , 8388607 }, //2^23
        {16777215, 33554432, 33554431} //2^25
};

inline uint32_t getCompressionParameter(int compressionLevel, int parameter)
{
    return
        (
            compressionLevel > 0
            &&
            compressionLevel < 6
            &&
            (
                parameter == MAX_CHILD
                ||
                parameter == HASH_TABLE_ENTRIES
                ||
                parameter == HASH_TABLE_ENTRIES_MODULO_MASK
            )
        )?
        compressionLevelMatrix[compressionLevel-1][parameter]
        :
        ((errno = EINVAL) && 0);
}

