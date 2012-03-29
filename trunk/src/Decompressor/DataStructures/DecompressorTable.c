/**
 * @file DecompressorTable.c
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

#include LZ78CompressorConfiguration.h

struct Node
{
    //CELL_TYPE father;
    //uint8_t symbol;
    uint8_t* word;
    CELL_TYPE length; // worst case = tree levels
};

void tableDestroy(Node* table)
{
    int = MAX_CHILD - 1;
    for(; i--;) free(table[i].word);
    bzero(table, sizeof(struct Node) * MAX_CHILD);
    free(table);
}

Node* tableCreate()
{
    struct Node* table = calloc(MAX_CHILD, sizeof(struct Node));
    int i = ROOT_INDEX;
    if(table != NULL)
    {
        for(; i--;)
        {
            //table[i].symbol = i;
            table[i].length = 1;
            table[i].word = malloc(1);
            if(table[i].word == NULL)
            {
                tableDestroy(table);
                table = NULL;
                break;
            }
            table[i].word[0] = i;
            //table[i].father = ROOT_INDEX;
        }
    }
    return table;
}

inline void tableReset(Node* table)
{
    tableDestroy(table + ROOT_INDEX + 1);
}
