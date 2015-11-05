#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


const int BITS_IN_UNSIGNED_CHAR = 8;
const int UNIVERSE_SIZE = 256;


//STRUCT ASSUMES SIZE TO BE NON-ZERO-INCLUSIVE
//so a bitset of size 10 will have 0..9 elements
struct bitset
{
	unsigned char * array;
	int arraySize;
	int size;
	unsigned char mask;
};

struct bitset * bitset_new(int passedSize)
{
	if(passedSize > 0)
	{
		struct bitset * newSet = malloc(sizeof(struct bitset));
		newSet->size = passedSize;
		int tempArraySize;
		if(passedSize%BITS_IN_UNSIGNED_CHAR == 0)
		{
			tempArraySize = passedSize/BITS_IN_UNSIGNED_CHAR;
		}
		else
		{
			tempArraySize = passedSize/BITS_IN_UNSIGNED_CHAR + 1;	//+1 to account for the passedSize%BITS_IN_UNSIGNED_CHAR 
		}															//remainer bits
		newSet->array = malloc(sizeof(unsigned char) * tempArraySize);
		newSet->arraySize = tempArraySize;
		newSet->mask = 0x80;	// mask 1000 0000
		return newSet;
	}
	else
	{
		printf("Invalid size specified.\n");
		return NULL;
	}
}


void bitset_deep_copy(struct bitset * dest, struct bitset * src1)
{
	dest->array = malloc(sizeof(unsigned char) * src1->arraySize);
	dest->size = src1->size;
	dest->arraySize = src1->arraySize;
	int index;
	for(index = 0; index < src1->arraySize; index++)
	{
		dest->array[index] = src1->array[index];
	}
}

//bitset_lookup assumes 0-based indexin
int bitset_lookup(struct bitset * this, int item)
{
	if(item >= 0 && item < this->size)
	{
		unsigned char * targetChar = &this->array[item/BITS_IN_UNSIGNED_CHAR];
		unsigned char mask = (this->mask) >> (item%BITS_IN_UNSIGNED_CHAR);
		return (*targetChar & mask) != 0;
	}
	else
	{
		return -1;
	}
}


int bitset_add(struct bitset * this, int item)
{
	if(item >= 0 && item < this->size)
	{
		unsigned char mask = (this->mask) >> item%BITS_IN_UNSIGNED_CHAR;
		unsigned char * targetChar = &this->array[item/BITS_IN_UNSIGNED_CHAR];
		*targetChar = *targetChar | mask;
		return 1;
	}
	else
	{
		return 0;
	}
}

int bitset_remove(struct bitset * this, int item)
{
	if(item >= 0 && item < this-> size)
	{
		unsigned char mask = ~((this->mask) >> item%BITS_IN_UNSIGNED_CHAR);
		unsigned char * targetChar = &this->array[item/BITS_IN_UNSIGNED_CHAR];
		*targetChar = *targetChar & mask;
		return 1;
	}
	else
	{
		return 0;
	}
}

void bitset_union(struct bitset * dest, struct bitset * src1, struct bitset * src2)
{
		assert(src1->size == src2->size);
		bitset_deep_copy(dest, src1);	//copy larger set into dest
		int index;
		for(index = 0; index < src2->arraySize; index++)
		{
			dest->array[index] = dest->array[index] | src2->array[index];
		}
}

//dest gets overwritten with src1, whether or not src1 is smaller than src2
void bitset_intersect(struct bitset * dest, struct bitset * src1, struct bitset * src2)
{
	assert(src1->size == src2->size);
	bitset_deep_copy(dest, src1);
	int index;
	for(index = 0; index < src2->arraySize; index++)
	{
		dest->array[index] = dest->array[index] & src2->array[index];
	}
}


void printCharSet(struct bitset * this)
{
	int index;
	for(index = 0; index < UNIVERSE_SIZE; index++)
	{
		if(bitset_lookup(this, index))
		{
			printf("%c ", index);
		}
	}
	printf("\n");
}

int main()
{
	int size = 1000;

	char *firstString = malloc(sizeof(char) * size);
	char *secondString = malloc(sizeof(char) * size);

	fgets(firstString, size, stdin);
	fgets(secondString, size, stdin);

	struct bitset * firstSet = bitset_new(UNIVERSE_SIZE);
	struct bitset * secondSet = bitset_new(UNIVERSE_SIZE);

	int index;
	for(index = 0; index < size; index++)	//setting appropriate bits to represent chars
	{
		bitset_add(firstSet, (unsigned char) firstString[index]);
		bitset_add(secondSet, (unsigned char) secondString[index]);
	}

	printf("Set of characters in first line: \n");
	printCharSet(firstSet);
	printf("Set of characters in second line: \n");
	printCharSet(secondSet);

	struct bitset * dest = bitset_new(UNIVERSE_SIZE);

	bitset_union(dest, firstSet, secondSet);
	printf("Union of sets (1) and (2): \n");
	printCharSet(dest);
	printf("Intersection of sets (1) and (2): \n");
	bitset_intersect(dest, firstSet, secondSet);
	printCharSet(dest);
	return 0;	
}
