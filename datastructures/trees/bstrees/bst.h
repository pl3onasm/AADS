#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// constants and macros
#define ROOT T->root

// data structures and types
typedef struct node {
  void *data;
  struct node *parent;
  struct node *left;
  struct node *right;
} node;

typedef struct {
  node *root;
} tree;

// function prototypes
tree *newTree (void);
node *newNode (void *data);
void insertNode (tree *tree, node *n, int (*cmp)(void *, void *));
void freeTree (tree *tree);
node *searchKey (node *x, void *key, int (*cmp)(void *, void *));
void deleteNode (tree *tree, node *z);
node *treeMinimum (node *x);
void printTree (node *x, short *count, void (*printData)(void *));
void printNode(node *x, void (*printData)(void *));
void writeTreeToFile (node *x, FILE *fp, 
  void (*printData)(void *, FILE *));
void buildTreeFromFile (tree *tree, char *filename, size_t dataSize,
  int (*cmp)(void *, void *), bool (*dataFromStr)(void *, char *));

#endif  // BST_H_INCLUDED