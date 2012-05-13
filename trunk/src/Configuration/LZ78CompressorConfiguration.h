/**
 * @file LZ78CompressorConfiguration.h
 * @author Cosimo Sacco <cosimosacco@gmail.com>
 * @author Davide Silvestri <davidesil.web@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <stdint.h>

#ifndef LZ78_COMPRESSOR_CONFIGURATION
    #define LZ78_COMPRESSOR_CONFIGURATION
    /**
     * ROOT_INDEX: index of the root of the compression table.
     **/
    #define ROOT_INDEX 0
    /**
     * FIRST_CHILD: number of valid initial entries, one entry for each possible
     * byte (256) plus one entry for the root of the compression table.
     **/
    #define FIRST_CHILD 257
    /**
     * INITIAL_INDEX_LENGTH: at the beginning there are 257 possible indexes
     * (256 which refer entries with a single byte, one wich refers the root).
     * So, at the beginning, ceiling(log_2(INITIAL_INDEX_LENGTH)) bits are
     * sufficient to index each index. This value is valid until there are less
     * than 512 entries, so this constant is an initializer for a variable
     * which holds the current index length.
     **/
    #define INITIAL_INDEX_LENGTH 9 //ceiling(log_2(257))
    /**
     * INDEX_LENGTH_MASK: At the beginning, FIRST_CHILD entries are present,
     * which can be addressed using an index with INITIAL_INDEX_LENGTH bits.
     * When the number of entries increases, at a certain point, the index
     * length must be updated. This happens when the number of entries reaches
     * the next "least bigger" power of two: in that case, the index length must
     * be increased by 1. When the rest of the division between the current
     * number of entries and the next "least bigger" power of two is zero,
     * then the current number of entries reached the next power of two and the
     * index length must be increased. Then, the next power of two is calculated
     * and used until the number of entries reach it, and so on.
     * This test involves division. Since both the "least bigger" power of two
     * and the current number of entries are variables, the compiler, being
     * unaware of their runtime values, cannot optimize the division and the
     * modulo operation. So, the constant INDEX_LENGTH_MASK is used to
     * initialize a variable which holds a mask related to the current
     * "least bigger" power of two, and can be used to calculate
     * the modulo. In order to do so, a simple bitwise and between those
     * variables is sufficient. When the current power of two is reached, the
     * mask has to be updated. If the current power of two is 2^k, the mask
     * is (2^k)-1, and it is updated as follows: mask = (mask << 1) | 1.
     **/
    #define INDEX_LENGTH_MASK 511 //111111111 (INITIAL_INDEX_LENGTH ones)
    /**
     * The bytes of the file to be compressed are read using fread.
     * fread improves the performances of the read syscall adding a buffering
     * layer: the cost of the read syscall is amortized reading more than the
     * requested bytes and buffering them, so that the next calls can avoid the
     * costly read syscall and return the buffer content.
     * Anyway, fread also has a cost (at least, the cost of construction of the
     * activation record).
     * The compressor choises are taken for each byte read. But, if a different
     * call to fread is done for each read byte, the total cost of calling fread
     * can be high. To reduce that cost, we adopted the same strategy used by
     * fread: another buffering layer is introduced. The bytes are accessed on
     * the buffer, and when all the buffered bytes have been used, a fread call
     * is done to fill up the buffer. So, the fread cost is amortized of a
     * factor which is proportional to the buffer size.
     * The LOCAL_BYTE_BUFFER_LENGTH constant is the length for this local buffer.
     **/
    #define LOCAL_BYTE_BUFFER_LENGTH 128
    /**
     * INDEX_TYPE is a typedef for CELL_TYPE, since the indexes have to be
     * written into the compressed file.
     **/
    #define INDEX_TYPE uint32_t
    /**
     * The definition of CELL_TYPE_LENGTH triggers the right BitwiseBufferedFile
     * configuration.
     **/
    #define CELL_TYPE_LENGTH 32
    /**
     * HASH_INDEX is the type of an index used to index an entry into the hash
     * table used by the compressor.
     **/
    #define HASH_INDEX uint32_t
    #define HASH_INDEX_LENGTH 32
    /**
     * Compression parameters
     * ----------------------
     * Due to memory concerns, a limit must be set to the dimension of the hash
     * table used by the compressor and of the decompression table. When the
     * maximum number of entries is used, those tables are reset (if necessary)
     * for the next round.
     * The compression parameters are:
     * - maxChild: the maximum number of meaningful entries
     * - hashTableEntries: the total number of entries in the hash table
     *
     * The bigger is maxChild, the better is the compression ration, but the
     * execution times are slown down.
     * Increasing hashTableEntries, the performances may increase, since the
     * collision probability becomes smaller (under the assumpt that a uniform
     * pseudorandom hash function is used).
     * In general, the following assertion must be true:
     *
     * hashTableEntries >= maxChild
     *
     * Given a compression level c >= 1, maxChild and hashTableEntries are
     * calculated as follows:
     *
     * maxChild = (2^(14 + 2*c)) - 1
     * hashTableEntries = (2^(14 + 2*c + 1))
     *
     * maxChild is the biggest unsigned integer representable on (14  2*c).
     * hashTableEntries is the (14 + 2*c)th power of two (so, the modulo
     * operations in most of the hash functions can be manually optimized).
     **/
    #define MAX_CHILD 0
    #define HASH_TABLE_ENTRIES 1
    #define HASH_TABLE_ENTRIES_MODULO_MASK 2
    #define MIN_COMPRESSION_LEVEL 1
    #define DEFAULT_COMPRESSION_LEVEL 3
    #define MAX_COMPRESSION_LEVEL 5
    #define HEADER_LENGTH 3
    inline uint32_t getCompressionParameter(int compressionLevel, int parameter);
#endif
