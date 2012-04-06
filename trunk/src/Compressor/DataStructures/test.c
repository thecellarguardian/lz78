#include "LZ78HashTable.h"
#include "../../Configuration/LZ78CompressorConfiguration.h"
#include <stdio.h>
#include <assert.h>

int main()
{
	struct LZ78HashTableEntry* table = hashCreate();
	INDEX_TYPE father = 0;
	uint8_t childValue = 0;
	INDEX_TYPE childIndex = 0;
	uint16_t app = 0;
	int insertReturn = 0;
	INDEX_TYPE lookupReturn = 0;
	for(; father < MAX_CHILD; father++)
	{
		for(; app < 2; app++)
		{
			childValue = (uint8_t)app;
			insertReturn = hashInsert(table, father, &childValue, childIndex);
			lookupReturn = hashLookup(table, father, &childValue);
			//assert(insertReturn != -1);
			//assert(lookupReturn == childIndex);
			printf("%lu\n", lookupReturn);
			childIndex = (childIndex == ROOT_INDEX - 1)? (childIndex + 2) : (childIndex + 1);
		}
		childValue = 0;
		app = 0;
	}
	hashDestroy(table);
	return 0;
}
