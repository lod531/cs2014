#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct node 
{
  char * data;
  struct node * next;
};

struct node * node_new(char * passedData)
{
	struct node * result = malloc(sizeof(struct node));
	result->data = passedData;
	result->next = NULL;
	return result;
}






struct hashtable 
{
  struct node ** array;
  int size;
};

unsigned hash_string(char * str)
{
  unsigned hash = 0;
  int i;
  for (i = 0; str[i] != '\0'; i++ ) {
    hash = hash * 37 + str[i];
  }
  return hash;
}

struct hashtable * hashtable_new(int passedSize)
{
	struct hashtable * result = malloc(sizeof(struct hashtable));
	result->array = malloc(sizeof(struct node *) * passedSize);
	result->size = passedSize;
	return result;
}

void hashtable_print(struct hashtable * this)
{
	int index;
	for(index = 0; index < this->size; index++)
	{
		printf("%d: ", index);
		if(this->array[index] == NULL)
		{
			printf("<empty>\n");
		}
		else
		{
			struct node * currentNode = this->array[index];
			while(currentNode!= NULL)
			{
				printf("%s ->", currentNode->data);
				currentNode = currentNode->next;
			}
			printf(" NULL\n");
		}
	}
}

int hashtable_lookup(struct hashtable * this, char * item)
{
	int result = 0;
	struct node * currentNode = this->array[hash_string(item) % this->size];
	if(currentNode != NULL)
	{
		while(!result && currentNode != NULL)
		{
			result = strcmp(item, currentNode->data) == 0;
			currentNode = currentNode->next;
		}
	}
	return result;
}

char * copyString(char * src1)
{
	char * result = malloc(sizeof(src1));
	assert(result != NULL);
	int index;
	for(index = 0; index < sizeof(src1); index++)
	{
		result[index] = src1[index];
	}
	return result;
}

//the function iterates through the chain. If the element isn't found, then it is added
//if it is, then nothing. It is mildly more efficient than calling lookup at start this way.
void hashtable_insert(struct hashtable * this, char * item)
{
	char * privateItem = copyString(item);
	struct node * newNode = node_new(privateItem);		//privateItem ensures that the string will not be altered.
	int hashtableIndex = hash_string(privateItem) % this->size;
	if(this->array[hashtableIndex] == NULL)	//if index is empty
	{
		this->array[hashtableIndex] =newNode;
	}
	else
	{	
		int newElement = 1;
		struct node * currentNode= this->array[hashtableIndex];
		struct node * previousNode;
		while(newElement && currentNode != NULL)
		{
			previousNode = currentNode;
			newElement = strcmp(currentNode->data, privateItem);	//strcmp will return true whilst items are unequal
			currentNode = currentNode->next;
		}
		if(newElement)
		{
			previousNode->next = newNode;
		}
		//else nothing.
	}
}

//the function iterates through the chain. If the element isn't found, then nothing,
//if it is, then it is removed. It is mildly more efficient than calling lookup at start this way.
void hashtable_remove(struct hashtable * this, char * item)
{

	/*
	OLD VERSION OF THE FUNCTION BELOW.
	//I know that this function looks rather large, but it is actually extremely simple
	//and runs at most a few lines of code at a time. It's  size is due to the if/else
	//structure required to deal with all cases, i.e. if element to be removed is
	//the first element, the last element, or somewhere in between.
	if(hashtable_lookup(this, item))	//if item is in hashtable
	{
		int hashtableIndex = hash_string(item) % this->size;
		struct node * currentNode = this->array[hashtableIndex];
		//I do not have to check whether currentNode == NULL, as I know for a fact that the element
		//is in the hashtable (due to hashtable_lookup)
		if(!strcmp(currentNode->data, item))	//if the element to be removed is the first element
		{										//!strcmp because it returns 0 if equal
			if(currentNode->next == NULL)
			{
				this->array[hashtableIndex] = NULL;
				free(currentNode);
			}
			else
			{			
				this->array[hashtableIndex] = currentNode->next;
				free(currentNode);
			}
		}
		else	//else element is not the first element, and there is at least 
		{
			struct node * previousNode = currentNode;
			currentNode = currentNode->next;	//I know for a fact that there is a next node, as the element to be deleted
												//is not the first element, but it is present in table

			while(strcmp(currentNode->data, item))	//again, I do not have to check whether currentNode == NULL
			{
				previousNode = currentNode;
				currentNode = currentNode->next;
			}
			if(currentNode->next == NULL)	//if the element to be removed is the last element
			{	
				previousNode->next = NULL;
				free(currentNode);
			}
			else							//else the element is in the middle, neither first or last, and hence
			{								//it is in the middle.
				previousNode->next = currentNode->next;
				free(currentNode);
			}
		}
	}
	*/

	//NEW VERSION
	int hashtableIndex = hash_string(item) % this->size;
	struct node * currentNode = this->array[hashtableIndex];
	if(currentNode == NULL)
	{
		//do nothing.
	}
	else if(!strcmp(currentNode->data, item))	//if first item is to be removed
	{
		if(currentNode->next == NULL)	//if no other nodes
		{
			this->array[hashtableIndex] = NULL;
			free(currentNode);
		}
		else							//else there are nodes following it
		{
			this->array[hashtableIndex] = currentNode->next;
			free(currentNode);
		}
	}
	else	//else the element to remove is either not present, or further in the chain
	{

		struct node * previousNode = currentNode;
		while(currentNode != NULL && strcmp(currentNode->data, item))	//strcmp rather than !strcmp because
		{																//strcmp returns 0 if equal, and true otherwise
			previousNode = currentNode;
			currentNode = currentNode->next;
		}
		if(currentNode == NULL)				//means data not found
		{
			//do nothing.
		}
		else if(currentNode->next == NULL)	//if element to be removed is the last in the chain
		{
			previousNode->next = NULL;
			free(currentNode);
		}
		else								//else the element is in the middle someplace
		{
			previousNode->next = currentNode->next;
			free(currentNode);
		}
	}
}

int main(int argc, char ** argv)
{
	struct hashtable * testTable = hashtable_new(9);
	int index;
	for(index = 1; index < argc; index++)
	{
		hashtable_insert(testTable, argv[index]);
	}
	hashtable_remove(testTable, "porridge");
	hashtable_insert(testTable, "fish");
	hashtable_print(testTable);
	return 0;
}