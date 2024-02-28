/* file: huffman.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: Huffman coding
     using a greedy approach and a minimum priority queue
   time complexity: O(n log n), where n is the number 
     of distinct characters in the input text, i.e. the size 
     of the alphabet
   assumption: the input text contains only characters from 
     the extended ASCII table, reason why we use unsigned char
*/ 

#include "../../../datastructures/heaps/binheaps/binheap.h"
#include "../../../lib/clib/clib.h"

#define LENGTH  256

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct node {
  size_t freq;          // frequency of the character
  uchar ch;             // character
  struct node *left;    // left child
  struct node *right;   // right child
} node;

//::::::::::::::::::::::::: tree functions ::::::::::::::::::::::::://

node *newNode () {
  /* creates a new node */
  node *n = safeCalloc(1, sizeof(node));
  n->ch = 0;
  return n;
}

int cmpNodesByFreq (const void *a, const void *b) {
  /* comparison function for nodes, based on frequency */
  return ((node *)a)->freq - ((node *)b)->freq;
}

void freeTree (node *n) {
  /* frees the tree */
  if (!n) return;
  freeTree(n->left);
  freeTree(n->right);
  free(n);
}

size_t getHeight (node *n) {
  /* returns the height of the tree */
  if (!n) return 0;
  return 1 + MAX(getHeight(n->left), getHeight(n->right));
}

//::::::::::::::::::::::::: input / output ::::::::::::::::::::::::://

binheap *readInput (void) {
  /* reads the input text and counts the char frequencies, 
     then creates a node for each character with non-zero 
     frequency and inserts it into a min binary heap */
  size_t freqs[LENGTH] = {0};
  uchar ch;
  while (scanf("%c", &ch) != EOF)
    if (ch >= 32 && ch < LENGTH)  // only printable characters
      freqs[ch-0]++;
  // make nodes for all characters with non-zero frequency
  binheap *H = newBinHeap(LENGTH, true, cmpNodesByFreq);
  for (size_t i = 0; i < LENGTH; ++i)
    if (freqs[i]) {
      node *n = newNode();
      n->ch = i;
      n->freq = freqs[i];
      pushToBinHeap(H, n);
    }
  return H;
}

void showCodes(node *n, short *code, int level) {
  /* prints the huffman codes for all characters */
  if (!n->left && !n->right) {
    printf("%4c %7ld      ", n->ch, n->freq);
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

void showTree(node *n, short *code, int level) {
  /* prints the huffman tree */
  if (!n) return;
  if (!level) printf("root\n");
  else {
    for (size_t i = 0; i < level-1; ++i)
      printf("-%hd", code[i]);
    printf("|%hd ", code[level-1]);
    if (!n->left && !n->right)  // leaf node
      printf("'%c'\n", n->ch);
    else  // internal node
      printf("\n");
  }
  code[level] = 0;
  showTree(n->left, code, level+1);
  code[level] = 1;
  showTree(n->right, code, level+1);
}

void printResult (node *tree, short *code) {
  /* prints the codes and the tree*/
  printf("CODEWORD TABLE\n\n");
  printf("char    freq      code\n");
  showCodes(tree, code, 0);
  printf("\n\nHUFFMAN TREE\n\n"); 
  showTree(tree, code, 0);
  printf("\n");
}

//::::::::::::::::::::::: huffman functions :::::::::::::::::::::::://

node *huffman (binheap *H) {
  /* creates the Huffman tree */
  while (H->size > 1) {
    node *z = newNode();
    z->left = popFromBinHeap(H);
    z->right = popFromBinHeap(H);
    z->freq = z->left->freq + z->right->freq;
    pushToBinHeap(H, z);
  }
  return popFromBinHeap(H);
}

//::::::::::::::::::::::::::::: main ::::::::::::::::::::::::::::::://

int main() {
  
  // reads the input text and counts the frequencies
  binheap *H = readInput();
  
  // creates the Huffman tree
  node *tree = huffman(H);    

  // height of the tree is the maximum code length
  size_t height = getHeight(tree);  
  short *code = safeCalloc(height, sizeof(short));

  // prints the codes and the tree
  printResult(tree, code);
 
  freeTree(tree);
  freeBinHeap(H);
  free(code);
  return 0; 
}
