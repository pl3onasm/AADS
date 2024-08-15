/* 
  file: huffman.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: Huffman coding
  time complexity: O(n log n), where n is the number 
    of distinct characters in the input text, i.e. the size 
    of the alphabet |Σ|
  assumption: the input text contains only characters from 
    the extended ASCII table, reason why we use unsigned char 
    which can hold values from 0 to 255
*/ 

#include "../../../datastructures/heaps/binheaps/binheap.h"
#include "../../../lib/clib/clib.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define LENGTH  256

//===================================================================
// Definition of the node structure
typedef struct node {
  size_t freq;          // frequency of the character
  unsigned char ch;     // character
  struct node *left;    // left child
  struct node *right;   // right child
} node;

//===================================================================
// Creates a new node
node *newNode () {
  node *n = safeCalloc(1, sizeof(node));
  return n;
}

//===================================================================
// Comparison function for the binary heap
int cmpNodesByFreq (void const *a, void const *b) {
  return ((node *)a)->freq - ((node *)b)->freq;
}

//===================================================================
// Deallocates the tree
void freeTree (node *n) {
  if (!n) return;
  freeTree(n->left);
  freeTree(n->right);
  free(n);
}

//===================================================================
// Computes the height of the tree
size_t getHeight (node *n) {
  if (!n) return 0;
  return 1 + MAX(getHeight(n->left), getHeight(n->right));
}

//===================================================================
// Reads the input text and counts the frequencies of the characters
// Creates a node for each character with non-zero frequency and
// inserts it into a min binary heap
binheap *readInput () {
  size_t freqs[LENGTH] = {0};
  unsigned char ch;
  while (scanf("%c", &ch) != EOF)
      // only chars from 32 to 255 are considered
    if (ch >= 32 && ch < LENGTH)  
      freqs[ch - 0]++;
    // make a min binary heap with a node for each character
  binheap *H = bhpNew(LENGTH, MIN, cmpNodesByFreq);
  for (size_t i = 0; i < LENGTH; ++i)
    if (freqs[i]) {
      node *n = newNode();
      n->ch = i;
      n->freq = freqs[i];
      bhpPush(H, n);
    }
  return H;
}

//===================================================================
// Shows the code for each character of the alphabet by traversing
// the Huffman tree and printing the path from the root to the leaf
void showCodes(node *n, short *code, size_t level) {
  if (!n->left && !n->right) {
    printf("%4c %9zu       ", n->ch, n->freq);
    for (size_t i = 0; i < level; ++i)
      printf("%hd", code[i]);
    printf("\n");
    return;
  }
  code[level] = 0;
  showCodes(n->left, code, level + 1);
  code[level] = 1;
  showCodes(n->right, code, level + 1);
}

//===================================================================
// Shows the Huffman tree by an in-order traversal
void showTree(node *n, short *code, size_t level) {
  if (!n) return;

  code[level] = 0;
  showTree(n->left, code, level + 1);

  if (level) {
    for (size_t i = 0; i < level; ++i)
      printf("-%hd", code[i]);
    if (! n->left && ! n->right)
      printf(" '%c' (%zu)\n", n->ch, n->freq);
    else printf("\n");
  } else printf("root\n");

  code[level] = 1;
  showTree(n->right, code, level + 1);
}

//===================================================================
// Shows the code table and the Huffman tree
void showResults (node *tree, short *code) {
  printf("CODEWORD TABLE\n\n"
         "char      freq       code\n");
  showCodes(tree, code, 0);
  printf("\n\nHUFFMAN TREE\n\n"); 
  showTree(tree, code, 0);
  printf("\n");
}

//===================================================================
// Creates the Huffman tree from the binary heap and returns the root
node *huffman (binheap *H) {
  while (H->size > 1) {
    node *z = newNode();
    z->left = bhpPop(H);
    z->right = bhpPop(H);
    z->freq = z->left->freq + z->right->freq;
    bhpPush(H, z);
  }
  return bhpPop(H);
}

//===================================================================

int main() {
  
    // reads the input text and counts the frequencies
  binheap *H = readInput();
  
    // builds the Huffman tree
  node *tree = huffman(H);    

    // height of the tree is the maximum code length
  size_t height = getHeight(tree);  
  short *code = safeCalloc(height, sizeof(short));

    // shows the codewords and the Huffman tree
  showResults(tree, code);
 
  freeTree(tree);
  bhpFree(H);
  free(code);

  return 0; 
}
