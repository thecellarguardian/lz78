/**
 * @file bufferConfiguration.h
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
/**
 * CELL_TYPE_LENGTH is a configurable paramether.
 * A definition must be given before including configurableBuffer.h.
 * If no definition is found, the default value 64 is used.
 * The other paramethers are automatically derived from CELL_TYPE_LENGTH.
 **/

#ifdef EXTERN_BUFFER_CONFIGURATION_FILE
    #include EXTERN_BUFFER_CONFIGURATION_FILE
#endif

#ifndef CELL_TYPE_LENGTH
    #define CELL_TYPE_LENGTH 64 //TODO comunque non funziona la configurazione
#endif //CELL_TYPE_LENGTH

#ifndef BUFFER_CELLS
    #define BUFFER_CELLS (BUFSIZ/(CELL_TYPE_LENGTH/8))
#endif //BUFFER_CELLS

/**
 * All the other paramethers are defined properly.
 **/

#define BUFFER_BYTES (BUFFER_CELLS*sizeof(CELL_TYPE))

#if CELL_TYPE_LENGTH == 8
    #define CELL_TYPE uint8_t
    #define CELL_TYPE_BYTE_LENGTH 1
    #define BITWISE_SHIFT_FACTOR 3
    #define FULL_MASK 0xFF
    #define BITWISE_MODULO_MASK 7
#elif CELL_TYPE_LENGTH == 16
    #define CELL_TYPE uint16_t
    #define CELL_TYPE_BYTE_LENGTH 2
    #define SHIFT_FACTOR 1
    #define BITWISE_SHIFT_FACTOR 4
    #define FULL_MASK 0xFFFF
    #define MODULO_MASK 1
    #define BITWISE_MODULO_MASK 15
    #define LITTLE_ENDIAN_TO_HOST_CONVERT le16toh
    #define HOST_TO_LITTLE_ENDIAN_CONVERT htole16
#elif CELL_TYPE_LENGTH == 32
    #define CELL_TYPE uint32_t
    #define CELL_TYPE_BYTE_LENGTH 4
    #define SHIFT_FACTOR 2
    #define BITWISE_SHIFT_FACTOR 5
    #define FULL_MASK 0xFFFFFFFF
    #define MODULO_MASK 3
    #define BITWISE_MODULO_MASK 31
    #define LITTLE_ENDIAN_TO_HOST_CONVERT le32toh
    #define HOST_TO_LITTLE_ENDIAN_CONVERT htole32
#elif CELL_TYPE_LENGTH == 64
    #define CELL_TYPE uint64_t
    #define CELL_TYPE_BYTE_LENGTH 8
    #define SHIFT_FACTOR 3
    #define BITWISE_SHIFT_FACTOR 6
    #define FULL_MASK 0xFFFFFFFFFFFFFFFF
    #define MODULO_MASK 7
    #define BITWISE_MODULO_MASK 63
    #define LITTLE_ENDIAN_TO_HOST_CONVERT le64toh
    #define HOST_TO_LITTLE_ENDIAN_CONVERT htole64
#else
    #error The cell type length must be set to one of the following: 8, 16, 32, 64
#endif
