#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Space implementation is as efficient as can be, but this sucks on performance.
//edited out code would be more efficient in terms of performance, but worse on data.
//also the code not commented out treats 0 as an element
struct int_set
{
  int * array;
  int maxElements;
  int currentElements;
  int MIN_SIZE;
  int MAX_SIZE;
};

struct int_set * new_int_set()
{
  struct int_set * new_set = malloc(sizeof(struct int_set));
  new_set -> MIN_SIZE = 1;
  new_set -> MAX_SIZE = 1048576;  //2^20
  new_set->array = malloc(sizeof(int) * new_set->MIN_SIZE);
  new_set->maxElements = 0;
  new_set->currentElements = 0;

  return new_set;
}

void int_set_print(struct int_set * this)
{
  int index;
  for(index = 0; index < this->currentElements; index++)
  {
    printf("%d\n", this->array[index]);
  }
}

int int_set_lookup(struct int_set * this, int item)
{
  int result = 0;
  int index;
  for(index = 0; index < this->currentElements && result == 0; index++)
  {
    result = (this->array[index] == item)? 1 : 0;
  }
  return result;
}

/*
int int_set_zero_count(struct int_set * this)
{
  int index;
  int zeroCount = 0;
  for(index = 0; index < this->maxElements; index++)
  {
    zeroCount += (this->array[index] == 0)? 1 : 0;
  }
  return zeroCount;
}
*/

int int_set_add(struct int_set * this, int item)
{
  if(this->currentElements >= this->MAX_SIZE)
  {
    printf("Array has reached max size and cannot be altered.\n");
  }
  else
  {
    int result = int_set_lookup(this, item);
    if(!result)
    {
      int * tempArray = malloc(sizeof(int)*(this->currentElements + 1));
      int index;
      for(index = 0; index < this->currentElements; index++)
      {
        tempArray[index] = this->array[index];
      }
      tempArray[this->currentElements] = item;
      this->array = tempArray;
      this->currentElements = this->currentElements + 1;
      return result;
    }
    else
    {
      return !result;
    }
  }
  /*
  int result = int_set_lookup(this, item);

  if(!result)
  {
    if(this->maxElements == this->currentElements)
    {
      int * tempArray = malloc(sizeof(int)*((this->maxElements) + 2));
      int index;
      for(index = 0; index < (this->maxElements + 2); index++)
      {
        tempArray[index] = (index > this->maxElements)? 0 : this->array[index];   //populates new slots with 0
      }
      this->array = tempArray;
      this->maxElements += 2;
    }
    int index = 0;
    int nextEmptyIndex = -1;
    for(index = 0; index < this->maxElements && nextEmptyIndex == -1; index++)
    {
      nextEmptyIndex = (this->array[index] == 0)? index : -1;
    }
    this->array[nextEmptyIndex] = item;
    this->currentElements = this->currentElements + 1;
    return result;
  }
  else
  {
    return result;
  }
  */
}

int int_set_remove(struct int_set * this, int item)
{
  if(this->currentElements <= this->MIN_SIZE)
  {
    printf("Set has reached min size and cannot be altered.\n");
  }
  else
  {
    int index;
    int targetIndex = -1;
    for(index = 0; index < this->currentElements && targetIndex == -1; index++)
    {
      targetIndex = (this->array[index] == item)? index : -1;
    }
    if(targetIndex != -1)
    {
      this->array[targetIndex] = this->array[this->currentElements-1];
      int * tempArray = malloc(sizeof(int)*(this->currentElements - 1));
      for(index = 0; index < this->currentElements-1; index++)
      {
        tempArray[index] = this->array[index];
      }
      this->currentElements = this->currentElements-1;
    }
    else
    {
      return 0;
    }
  }

    /*
    if(int_set_zero_count(this) > (this->maxElements)/2 || int_set_zero_count(this) > 16)
    {
      int * tempArray = malloc(sizeof(int)*(this->maxElements/-16));
      int index;
      int tempArrayIndex = 0;
      for(index = 0; index < this->maxElements; index++)
      {
        if(this->array[index] != 0)
        {
          //printf("%d\n", tempArrayIndex);
          tempArray[tempArrayIndex] = this->array[index];
          tempArrayIndex++;
        }
      }
      this->array = tempArray;
      this->maxElements = this->maxElements - 16;
    }
    int index;
    int targetIndex = -1;
    for(index = 0; index < this->maxElements && targetIndex == -1; index++)
    {
      targetIndex = (this->array[index] == item)? index : -1;
    }
    this->array[targetIndex] = 0;
    this->currentElements = this->currentElements-1;
    
  }
  else
  {
    return result;
  }
  */
}

void int_set_union(struct int_set * dest, struct int_set * src1, struct int_set * src2)
{
  int index;
  dest->array = malloc(sizeof(int)*src1->currentElements);
  dest->currentElements = src1->currentElements;
  for(index = 0; index < src1->currentElements; index++)
  {
    dest->array[index] = src1->array[index];
  } 
  for(index = 0; index < src2->currentElements; index++)
  {
    int_set_add(dest, src2->array[index]);
  }
}

void int_set_intersect(struct int_set * dest, struct int_set * src1, struct int_set * src2)
{
  int index;
  for(index = 0; index < src1->currentElements; index++)
  {
    if(int_set_lookup(src1, src1->array[index]) && int_set_lookup(src2, src1->array[index]))
    {
      int_set_add(dest, src1->array[index]);
    }
  }
}



int main()
{
  struct int_set * set1 = new_int_set();
  struct int_set * set2 = new_int_set();
  int index;
  for(index = -20; index < 25; index++)
  {
    int_set_add(set2, index);
  }
  for(index = -50; index < 50; index++)
  {
    int_set_add(set1, index);
  }
  struct int_set * dest = new_int_set();
  int_set_intersect(dest, set1, set2);
  //int_set_print(set); //\n\n\n\n\n\n\n\n\n\n\n\n\nv
  printf("\n\n\n\n");
  int_set_print(dest);
  return 0;
}