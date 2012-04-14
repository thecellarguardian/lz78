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

#include "DecompressorTable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void tableDestroy(struct Node* table)
{
    if(table == NULL) return;
    int i = MAX_CHILD - 1;
    for(; i--;) free(table[i].word);
    bzero(table, sizeof(struct Node) * MAX_CHILD);
    free(table);
}

struct Node* tableCreate()
{
    struct Node* table = calloc(MAX_CHILD, sizeof(struct Node));
    int i = 1;
    struct Node* current;
    if(table != NULL)
    {
        for(; i < 257; i++)
        {
            current = &(table[i]);
            current->length = 1;
            current->word = malloc(1);
            if(current->word == NULL)
            {
                tableDestroy(table);
                table = NULL;
                break;
            }
            current->word[0] = i;
           // printf("riempio la posizione %i con %c\n", i, current->word[0]);
        }
    }
    return table;
}

inline void tableReset(struct Node* table)
{
    table = table + 257;
    int i = MAX_CHILD - 257; //TODO siamo sicuri?
    for(; i--;) free(table[i].word);
    bzero(table, sizeof(struct Node) * MAX_CHILD);
}
