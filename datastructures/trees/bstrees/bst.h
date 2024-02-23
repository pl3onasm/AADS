#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// constants and macros
#define ROOT tree->root

// data structures and types
typedef struct node {
  void *data;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root;
} bst;

// function prototypes
bst *newBST (void);
node *newBSTnode (void *data);
void BSTinsert (bst *tree, node *n, int (*cmp)(void *, void *));
void freeBST (bst *tree);
node *BSTsearch (node *x, void *key, int (*cmp)(void *, void *));
void BSTdelete (bst *tree, node *z);
node *BSTminimum (node *x);
void printBST (node *x, short *count, void (*printData)(void *));
void printBSTnode(node *x, void (*printData)(void *));
void writeBSTtoFile (node *x, FILE *fp, 
  void (*printData)(void *, FILE *));
void readBSTfromFile (bst *tree, char *filename, size_t dataSize,
  int (*cmp)(void *, void *), bool (*dataFromStr)(void *, char *));

#endif  // BST_H_INCLUDED