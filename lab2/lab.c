#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static char ALPHA[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 
  'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
  'X', 'Y', 'Z'};
static int ALPHA_SIZE = 52;

static char NUM[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

static int NUM_SIZE = 10;

int isValidLetter(char targetChar)
{
  int i;
  int returnValue = 0;
  for(i = 0; i < ALPHA_SIZE && returnValue == 0; i++)
  {
    returnValue = (targetChar == ALPHA[i])? 1:0;
  }
  return returnValue;
}

int isValidNumber(char targetNumber)
{
  int i;
  int returnValue = 0;
  for(i = 0; i < NUM_SIZE && returnValue == 0; i++)
  {
    returnValue = (targetNumber == NUM[i])? 1:0;
  }
  return returnValue;
}

int isValidEmailAddress(char * address, char ** terminators, int num_terminators)
{
  //RETURNS 1 FOR VALID EMAIL ADDRESS
  //RETURNS 0 FOR INVALID EMAIL ADDRESS
  //nested for loops check for each requirement, requirements being first identifier, the @ sign, second identifier and a termination sequence.
  int containsSpace = 0;
  int inValidCharSequence = 0;
  int encounteredIdentifier = 0;
  int encounteredAt = 0;
  int encounteredSecondIdentifier = 0;
  int encounteredTerminator = 0;                                //encounteredTerminator starts with boolean TRUE to accommodate for loop structure (the deepest nested loop)
  int addressLength = strlen(address);
  int currentIndex = 0;                                         //currentIndex IS A PRESERVED ACROSS FOR LOOPS



  //This if tree structure could be collapsed into a single huge for loop, but I left it this way to make it more readable.
  //The algorhithm could be more efficient that way, but oh well.
  if((isValidNumber(address[0]) || isValidLetter(address[0])) && (isValidNumber(address[addressLength-1]) || isValidLetter(address[addressLength-1])))  //checks first and last letters to be valid letters or numbers
  {
    for(;currentIndex < addressLength && containsSpace == 0; currentIndex++)
    {
      containsSpace = (address[currentIndex] == ' ')? 1 : 0;
    }
    if(!containsSpace)
    {
    //below for loop ensures that no two adjacent chars are nonLetter chars
      for(;currentIndex < addressLength && inValidCharSequence <2; currentIndex++)
      {
        if(isValidLetter(address[currentIndex]) || isValidNumber(address[currentIndex]))
        {
          inValidCharSequence = 0;
        }
        else
        {
          inValidCharSequence++;
        }
      }  

      if(inValidCharSequence < 2)
      {
        currentIndex = 0;     //essentially reinitializing the curentIndex.
        for(; currentIndex < addressLength && encounteredIdentifier == 0; currentIndex++)
        {
          encounteredIdentifier = (isValidLetter(address[currentIndex]))? 1 : 0;
        }
        if(encounteredIdentifier == 1)
        {
          // at this point the parser has found an initial identifier
          for(; currentIndex < addressLength && encounteredAt == 0; currentIndex++)
          {
            encounteredAt = (address[currentIndex] == '@')? 1:0; 
          }
          if(encounteredAt == 1)
          {
            //at this point the parser has found the @

            for(; currentIndex < addressLength && encounteredSecondIdentifier == 0; currentIndex++)
            {
              encounteredSecondIdentifier = (isValidLetter(address[currentIndex]))? 1 : 0;
            }
            if(encounteredSecondIdentifier == 1)
            {
              //at this point the parser has found found a second identifier
              int currentTerminatorIndex;
              for(currentTerminatorIndex = 0; currentTerminatorIndex < num_terminators; currentTerminatorIndex++)
              {
                int lengthOfCurrentTerminator = (int) strlen(*&(terminators[currentTerminatorIndex]));
                int endCharIndex;
                int currentTerminatorStringIndex = 0;
                int numberOfCharsChecked = 0;
                encounteredTerminator = 0;
                

                //checks if the terminator is preceded by a period
                if(address[addressLength - lengthOfCurrentTerminator - 1] == '.')
                {
                  for(endCharIndex = addressLength - lengthOfCurrentTerminator;
                   endCharIndex < addressLength && currentTerminatorStringIndex < lengthOfCurrentTerminator && encounteredTerminator == 0; 
                    endCharIndex++, currentTerminatorStringIndex++, numberOfCharsChecked++)
                  {
                    //Kay, so, this for loop checks the last chars (hence endCharIndex) against the terminator chars. If the terminator is of length 3 (say com) then 
                    //the loop will check the third last letter of address against c, then second last agains o, and lastly last letter against m.
                    //Throughout numberOfCharsChecked will be incremented. If the address or the terminator has fewer characters than expected, the counter will note that.
                    //The loop works on the condition that encounteredTerminator == 0. If the encounteredTerminator is ever 1, that means the chars did not match and 
                    //the terminator sequence was broken. If encounteredTerminator is 0 by the end of the loop, but numberOfCharsChecked != lengthOfCurrentTerminator,
                    //then not all chars were checked against one another, and the terminator sequence was broken. If both (encounteredTerminator == 0 && numberOfCharsChecked == lengthOfCurrentTerminator)
                    // conditions are met, then the termination sequence was found.
                    encounteredTerminator = (address[endCharIndex] == terminators[currentTerminatorIndex][currentTerminatorStringIndex])? 0:1;
                  }
                  if(encounteredTerminator == 0 && numberOfCharsChecked == lengthOfCurrentTerminator)
                  {
                    return 1;
                  }
                }
              }
            }
          }
        }
      }
    }  
  }
  return 0;
}

void print_validity(char * address, char** terminators,
                    int length)
{
  printf("The string %s is ", address);
  if (!isValidEmailAddress(address, terminators, length)) {
    printf("not ");
  }
  printf("a valid email address\n");
}



int main()
{
  const int length = 5;
  char * terminators[length];
  char * address1, * address2;
  char c = '0';

  int test = isValidLetter(c);

  terminators[0] = "com";
  terminators[1] = "net";
  terminators[2] = "edu";
  terminators[3] = "ie";
  terminators[4] = "tv";

  address1 = "1a@123a.tv";
  address2 = "I.am fish.net";
  int lengthOfAddress = (int) strlen(address2);
  char testChar = address2[lengthOfAddress-1];

  //printf("%c\n", testChar);



  print_validity(address1, terminators, length);
  print_validity(address2, terminators, length);


  //         \\\\\\\\\\\\\\\\\\\\\\

  return 0;
}