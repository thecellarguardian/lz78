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
	for(; father < MAX_CHILD; father++)
	{
		for(; app < 256; app++)
		{
			childValue = (uint8_t)app;
			assert(hashInsert(table, father, &childValue, childIndex) == 0); //<<DOVREBBE FALLIRE PRIMA QUESTA
			//assert(hashLookup(table, father, &childValue) == childIndex);
			printf("OK(%llu, %u)\n", father, childValue);
		}
		childIndex = (childIndex == ROOT_INDEX - 1)? (childIndex + 2) : (childIndex + 1);
		childValue = 0;
		app = 0;
	}
	return 0;
}
