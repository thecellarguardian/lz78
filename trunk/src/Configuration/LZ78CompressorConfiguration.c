#include "LZ78CompressorConfiguration.h"
#include <errno.h>

const uint32_t compressionLevelMatrix [5][3] =
{
        /* ******************************************************************* *
         * MAX_CHILD  |  HASH_TABLE_ENTRIES  |  HASH_TABLE_ENTRIES_MODULO_MASK *
         * ******************************************************************* */
        {  65535      ,  131072              ,  131071  },//2^16-1, 2^17, 2^17-1
        {  262143     ,  524288              ,  524287  },//2^18-1, 2^19, 2^19-1
        {  1048575    ,  2097152             ,  2097151 },//2^20-1, 2^21, 2^21-1
        {  4194303    ,  8388608             ,  8388607 },//2^22-1, 2^23, 2^23-1
        {  16777215   ,  33554432            ,  33554431} //2^24-1, 2^25, 2^25-1
};

inline uint32_t getCompressionParameter(int compressionLevel, int parameter)
{
    return
        (
            compressionLevel >= MIN_COMPRESSION_LEVEL
            &&
            compressionLevel <= MAX_COMPRESSION_LEVEL
            &&
            (
                parameter == MAX_CHILD
                ||
                parameter == HASH_TABLE_ENTRIES
                ||
                parameter == HASH_TABLE_ENTRIES_MODULO_MASK
            )
        )?
        compressionLevelMatrix[compressionLevel - 1][parameter]
        :
        ((errno = EINVAL) && 0);
}
