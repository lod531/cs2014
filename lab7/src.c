#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//START OF ALPHABET OPS///////////////////////////////////////////////////////////
const ALPHABET_SIZE = 256;

void alphabet_print(int * this)
{
  int i;
  for(i = 0; i < ALPHABET_SIZE; i++)
  {
    printf("%c: %d\n", i, this[i]);

  }
}

void alphabet_populate(int * this)
{
  int i;
  for(i = 0; i < ALPHABET_SIZE; i++)
  {
    this[i] = 1;
  }
}
//END OF ALPHABET OPS///////////////////////////////////////////////////////////

//START OF STRING MANIPULATION//////////////////////////////////////////////////

char * append_char(char * string, char c)
{
    char * result = malloc(sizeof(char) *(strlen(string)+2));
    int i;
    for(i = 0; i < strlen(string); i++)
    {
      result[i] = string[i];
    }
    result[strlen(string)] = c;
    result[strlen(string) + 1] = 0;
    return result;
}

/*
char * concat_strings(char * string1, char * string2)
{
  char * result = malloc(sizeof(char) *(strlen(string1) + strlen(string2) + 1));
  int i;
  for(i = 0; i < (int) strlen(string1); i++)
  {
    result[i] = string1[i];
  }
  for(i = 0; i < (int) strlen(string2); i++)
  {
    result[(int) strlen(string1) + i] = string2[i];
  }
  result[(int) (strlen(string1) + strlen(string2))] = 0;
  return result;
}
*/

//END OF STRING MANIPULATION//////////////////////////////////////////////////


//START OF HUFFMAN OPS//////////////////////////////////////////////////////////
struct huffman_node
{
  union
  {
    struct
    {
      struct huffman_node * left;
      struct huffman_node * right;
    } branch;
    char nodeChar;
  } type;
  int isLeaf;
  int combinedOccurrence;
};

struct huffman_node * huffman_node_leaf(char c, int occurrence)
{
  struct huffman_node * result = malloc(sizeof(struct huffman_node));
  result->type.nodeChar = c;
  result->combinedOccurrence = occurrence;
  result->isLeaf = 1;
  return result;
}

//combinedOccurrence is derived from occurrence of left + occurence of right
struct huffman_node * huffman_node_branch(struct huffman_node * passedLeft, struct huffman_node * passedRight)
{
  struct huffman_node * result = malloc(sizeof(struct huffman_node));
  result->type.branch.left = passedLeft;
  result->type.branch.right = passedRight;
  result->isLeaf = 0;
  result->combinedOccurrence = passedLeft->combinedOccurrence + passedRight->combinedOccurrence;
  return result;
}

int huffman_tree_count_nodes(struct huffman_node ** tree)
{
  int result = 0;
  int i;
  for(i = 0; i < ALPHABET_SIZE; i++)
  {
    result += (tree[i] == NULL)? 0 : 1;
  }
  return result;
}

//assumes the tree is not null.
//deletes the least node from the tree and returns a pointer to it.
struct huffman_node * huffman_tree_delete_least_node(struct huffman_node ** tree)
{
  struct huffman_node * result = NULL;
  int i;
  int indexForDeletion = -1;
  for(i = 0; i < ALPHABET_SIZE; i++)
  {
    if(result == NULL)
    {
      result = tree[i];
      indexForDeletion = i;
    }
    else if(result != NULL && tree[i] != NULL)
    {
      if(result->combinedOccurrence > tree[i]->combinedOccurrence)
      {
        result = tree[i];
        indexForDeletion = i;
      }
    }
  }
  if(indexForDeletion != -1)
  {
    tree[indexForDeletion] = NULL;
  }
  return result;
}


//inserts the node into the least index, if possible.
void huffman_tree_insert_node(struct huffman_node ** tree, struct huffman_node * node)
{
  int i;
  int inserted = 0;
  for(i = 0; i < ALPHABET_SIZE && inserted == 0; i++)
  {
    if(tree[i] == NULL)
    {
      tree[i] = node;
      inserted = 1;
    }
  }
  return;
}

//assumes the tree has already been constructed.
struct huffman_node * huffman_tree_get_root(struct huffman_node ** tree)
{
  assert(huffman_tree_count_nodes(tree) == 1);
  struct huffman_node * result = NULL;
  int i;
  for(i = 0; i < ALPHABET_SIZE && result == NULL; i++)
  {
    if(tree[i] != NULL)
    {
      result = tree[i];
    }
  }
  return result;
}

struct huffman_node ** huffman_tree(int * alphabet)
{
  struct huffman_node ** result = malloc(sizeof(struct huffman_node) * ALPHABET_SIZE);
  int i;
  for(i = 0; i < ALPHABET_SIZE; i++)
  {
    result[i] = huffman_node_leaf(i, alphabet[i]);
  }
  while(huffman_tree_count_nodes(result) > 1)
  {
    struct huffman_node * firstNode = huffman_tree_delete_least_node(result);
    struct huffman_node * secondNode = huffman_tree_delete_least_node(result);
    assert(firstNode != NULL && secondNode != NULL);
    struct huffman_node * branch = huffman_node_branch(secondNode, firstNode);
    huffman_tree_insert_node(result, branch);
  }
  return result;
}

char * deep_copy(char * string)
{
  char * result = malloc(sizeof(char) * (strlen(string) + 1));
  int i;
  for(i = 0; i <= strlen(string); i++)
  {
    result[i] = string[i];
  }
  return result;
}

/*
char * huffman_tree_get_maps(struct huffman_node * root, char * acc, char * result)
{
  if(root->isLeaf)
  {
     append_char(acc, root->type.nodeChar);
     append_char(acc, (char) 10);

     return concat_strings(result, acc);
  }
  else
  {
    char * left = deep_copy(acc);
    left = append_char(left, '0');
    left = huffman_tree_get_maps(root->type.branch.left, left, result);

    char * right = deep_copy(acc);
    right = append_char(right, '1');
    right = huffman_tree_get_maps(root->type.branch.right, right, result);

    return concat_strings(left, right);
  }
}

*/

void huffman_tree_encoding_list_helper(struct huffman_node * root, char * acc)
{
  if(root->isLeaf)
  {
    printf("%s", acc);
    printf("  '%c'\n\n", root->type.nodeChar);
  }
  else
  {
    char * rightAcc = deep_copy(acc);
    rightAcc = append_char(rightAcc, '1');
    huffman_tree_encoding_list_helper(root->type.branch.right, rightAcc);

    char * leftAcc = deep_copy(acc);
    leftAcc = append_char(leftAcc, '0');
    huffman_tree_encoding_list_helper(root->type.branch.left, leftAcc);
  }
}

void huffman_tree_encoding_list(struct huffman_node ** tree)
{
  struct huffman_node * root = huffman_tree_get_root(tree);
  char * tempAcc = "";
  huffman_tree_encoding_list_helper(root, tempAcc);
}




//END OF HUFFMAN OPS//////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
  int * alphabet = malloc(sizeof(int) * ALPHABET_SIZE);
  alphabet_populate(alphabet);
  unsigned char c;
  FILE * file;

  if ( argc != 2 ) 
  {
    fprintf(stderr, "Useage: huffman <filename>\n");
    exit(1);      // exit with error code
  }

  file = fopen(argv[1], "r");
  assert( file != NULL );
  c = fgetc(file);
  while( !feof(file) ) 
  {
    c = fgetc(file);
    alphabet[c] = alphabet[c] + 1;
  }
  fclose(file);
  struct huffman_node ** testTree = huffman_tree(alphabet);
  huffman_tree_encoding_list(testTree);
  return 0;
}

//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n

/*

  struct huffman_node * testLeafLeft = huffman_node_leaf('c', 10);
  struct huffman_node * testLeafRight = huffman_node_leaf('a', 11);

  struct huffman_node * testBranch = huffman_node_branch(testLeafLeft, testLeafRight);
  struct huffman_node * testTest = testBranch->type.branch.left;

  printf("%d\n", testBranch->combinedOccurrence);

*/ 