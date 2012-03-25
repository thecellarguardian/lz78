/**
 * @file LZ78Compressor_configuration.h
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

#define ROOT_INDEX 256//TODO in realtà va messa nell'hash sta roba
#define INDEX_LENGTH 16
#define MAX_CHILD 256

