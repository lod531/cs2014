#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

static char NUM[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static int NUM_SIZE = 10;

static char SUPPORTED_OPERATORS[] = {'+', '-', 'X'};

static int SUPPORTED_OPERATORS_SIZE = 3;


//PARSING FUNCTIONS BEGIN HERE //////////////////////////////////////////////////////////////////////


int isValidOperator(char targerOperator)
{
	int index;
	int result = 0;
	for(index = 0; index < SUPPORTED_OPERATORS_SIZE && result == 0; index++)
	{
		result = (targerOperator == SUPPORTED_OPERATORS[index])? 1 : 0;
	}
	return result;
}
int isDigit(char targetNumber)
{
  int i;
  int returnValue = 0;
  for(i = 0; i < NUM_SIZE && returnValue == 0; i++)
  {
    returnValue = (targetNumber == NUM[i])? 1:0;
  }
  return returnValue;
}

int getNumericValue(char targetNumber)
{
  int i;
  int returnValue = -1;
  for(i = 0; i < NUM_SIZE && returnValue == -1; i++)
  {
    returnValue = (targetNumber == NUM[i])? i:-1;
  }
  return returnValue;
}


//isNumber(), as in if the string is corrupted and has a char in it, I'll discard it.
int isNumber(char * userInputChars, int userInputCharsLength)
{
	int indexOffset = 0;
	int index = 0;
	while(index + indexOffset < userInputCharsLength)
	{
		if(isDigit(userInputChars[index + indexOffset]))
		{
			if( (index + 1) < userInputCharsLength)
			{
				indexOffset++;
				while(((index + indexOffset) < userInputCharsLength) && isDigit(userInputChars[index + indexOffset]) )			//important to check if within array
				{																																//before checking if isDigit
					indexOffset++;
				}
				if(((index + indexOffset) == userInputCharsLength))	// because null char will be counted in the while loop
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 1;
			}
		}	
		else
		{
			if(indexOffset + 1 < userInputCharsLength)
			{
				indexOffset++;
			}
			else
			{
				return 0;
			}												
		}
	}
	return 0; //this return is here just to get this method to compile.
}

double getDouble(char * userInputChars, int userInputCharsLength)
{
	int indexOffset = 0;
	int negative = 0;
	int index = 0;
	while(index + indexOffset < userInputCharsLength)
	{
		if(userInputChars[index + indexOffset] == '-')
		{
			negative = !negative;
		}
		if(isDigit(userInputChars[index + indexOffset]))
		{
			double currentValue = getNumericValue(userInputChars[index + indexOffset]);
			if( (index + 1) <userInputCharsLength)
			{
				indexOffset++;
				while(((index + indexOffset) < userInputCharsLength) && isDigit(userInputChars[index + indexOffset]) )			//important to check if within array
				{																																//before checking if isDigit
					currentValue *= 10;
					currentValue += getNumericValue(userInputChars[index + indexOffset]);
					indexOffset++;
				}
				if(((index + indexOffset) < userInputCharsLength) && userInputChars[index + indexOffset] == '.')
				{
					indexOffset++;
					double tempFractionValue = getNumericValue(userInputChars[index + indexOffset]);
					indexOffset++;
					while(((index + indexOffset) < userInputCharsLength) && isDigit(userInputChars[index + indexOffset]) )			//important to check if within array
					{																																//before checking if isDigit
						tempFractionValue *= 10;
						tempFractionValue += getNumericValue(userInputChars[index + indexOffset]);
						indexOffset++;
					}
					while(tempFractionValue >= 1)
					{
						tempFractionValue *= 0.1;
					}
					currentValue += tempFractionValue;
				}
			}	
			index += indexOffset;
			return currentValue*((negative)? -1:1);
		}	
		else
		{
			if(indexOffset + 1 < userInputCharsLength)
			{
				indexOffset++;
			}
			else
			{
				return 0;
			}
		}
	}
return -1;								//this return is here just to get it to compile
}




//PARSING FUNCTIONS END HERE//////////////////////////////////////////////////////////////////////






//BEGINNING OF STACK ADT//////////////////////////////////////////////////////////////////////////

//empty ascending
struct double_stack
{
	double * array;
	int currentIndex;
	int currentSize;
};


//initialSize is 64 just because.
struct double_stack * double_stack_new()
{
	struct double_stack * newStack = malloc(sizeof(struct double_stack));
	int initialSize = 64;
	newStack->array = malloc(sizeof(int)*initialSize);
	newStack->currentIndex = 0;
	newStack->currentSize = initialSize;
}

void double_stack_print(struct double_stack * this)
{
	int index;
	for(index = 0; index < this->currentIndex; index++)
	{
		double temp = this->array[index];
		printf("%f\n", temp);
	}
}


//adjusts size of stack to passedSize, preserving values already present
//deletes values which would be newly out of bounds.
void double_stack_resize(struct double_stack * this, int passedSize)
{
	double * tempArray = malloc(sizeof(double) * passedSize);
	int index;
	for(index = 0; index < passedSize && index < this->currentSize; index++)
	{
		tempArray[index] = this->array[index];
	}
	this->array = tempArray;
	this->currentSize = passedSize;
}


//empty ascending
//doubles size of stack every time stack runs out of space
void double_stack_push(struct double_stack * this, double item)
{
	if(this->currentIndex == this->currentSize)
	{
		double_stack_resize(this, this->currentSize * 2);
	}
	this->array[this->currentIndex] = item;
	this->currentIndex = this->currentIndex + 1;
}

double double_stack_pop(struct double_stack * this)
{
	if(this->currentIndex > 0)
	{
		if(this->currentIndex < this->currentSize/4)
		{
			double_stack_resize(this, this->currentSize/4);
		}
		double tempItem = this->array[this->currentIndex-1];
		this->currentIndex = this->currentIndex - 1;
		return tempItem;
	}
	else
	{
		printf("Tried to pop off an empty stack.\n");
		return 0xcafebabe;
	}
}

int double_stack_isempty(struct double_stack * this)
{
	return this->currentIndex == 0;
}

int double_stack_size(struct double_stack *this)
{
	return this->currentIndex;
}


//STACK ADT ENDS HERE/////////////////////////////////////////////////////////////////////////////


double applyOperator(char operator, double firstOperand, double secondOperand)
{
	assert(isValidOperator(operator));
	switch(operator)
	{
		case ('+'):
		{
			return firstOperand + secondOperand;
		}
		case ('-'):
		{
			return firstOperand - secondOperand;
		}
		case ('X'):
		{
			return firstOperand * secondOperand;
		}
		default:
		{
			printf("In applyOperator, default case was reached, which should never happen");
		}
	}
}


//ingores the argv[0], and I just assume that whoever is running this knows that it's just an RPN calculator
//also assumes input is correct, as in there won't entries like 11a3 etc.
//RPN examples: 	7 8 3 X 28 + + = 59
//					5 1 2 + 4 × + 3 − = 14
int main(int argc, char ** argv)
{
	struct double_stack * newStack = double_stack_new();
	int index;
	for(index = 1; index < argc; index++)												//index = 1 because 0 is appname
	{
		if(isNumber(argv[index], (int) strlen(argv[index])))
		{
			double tempDouble = getDouble(argv[index], strlen(argv[index]));
			double_stack_push(newStack, tempDouble);
		}
		else if(isValidOperator(argv[index][0]))										//if valid operator
		{
			assert(double_stack_size(newStack) > 1);
			double secondOperand = double_stack_pop(newStack);							//note that the first operand comes first.
			double firstOperand = double_stack_pop(newStack);
			double result = applyOperator(argv[index][0], firstOperand, secondOperand);	//applyOperand(operator, operands) 
			double_stack_push(newStack, result);
		}
		else
		{
			printf("Invalid string\n");
		}
	}
	printf("The result is: ");
	double_stack_print(newStack);
	return 0;
}