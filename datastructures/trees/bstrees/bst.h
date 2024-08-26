/* 
  Generic binary search tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../../lists/dll.h"

// function pointer types
typedef int (*bstCmpData)(void const *a, void const *b);
typedef void (*bstShowData)(void const *data);
typedef void (*bstWriteData)(void const *data, FILE *file);
typedef bool (*bstStrToData)(void *data, char const *str);
typedef void (*bstFreeData)(void *data);
typedef void *(*bstCpyData)(void const *data);

// data structures and types
typedef struct bsnode {
  void *data;               // data stored in the node
  struct bsnode *parent;    // parent node
  struct bsnode *left;      // left child
  struct bsnode *right;     // right child
} bsnode;

typedef struct {
  bsnode *ROOT, *NIL;       // root and sentinel nodes
  bstCmpData cmp;           // comparison function
  bstShowData show;         // show function
  bstWriteData write;       // function to write data to file
  bstStrToData fromStr;     // parse string to data
  bstFreeData free;         // function to free data
  bstCpyData copy;          // function to copy data
  size_t size;              // number of tree nodes
} bstree;

// function prototypes

  // creates a new tree
bstree *bstNew(bstCmpData cmp);

  // makes the tree make copies of the data`
void bstCopyData(bstree *T, bstCpyData copy, 
                 bstFreeData free);

  // sets the tree to own the data
void bstOwnData(bstree *T, bstFreeData free);

  // sets the show function for the tree
void bstSetShow(bstree *T, bstShowData show);

  // returns true if the tree is empty
bool bstIsEmpty (bstree *T);

  // inserts a new node into the tree
void bstInsert(bstree *tree, void *data);

  // deallocates the tree
void bstFree(bstree *tree);

  // searches the tree for a key
bsnode *bstSearch(bstree *T, void *key);

  // deletes a node from the tree
void bstDelete(bstree *tree, bsnode *z);

  // returns the minimum node in the tree
bsnode *bstMinimum(bstree *T, bsnode *x);

  // returns the maximum node in the tree
bsnode *bstMaximum(bstree *T, bsnode *x);

  // returns the successor of a node
bsnode *bstSuccessor(bstree *T, bsnode *x);

  // returns the predecessor of a node
bsnode *bstPredecessor(bstree *T, bsnode *x);

  // displays the (sub)tree rooted at x in order
void bstShow(bstree *T, bsnode *x);

  // shows the structure of the tree rooted at x
void bstShowTree(bstree *T, bsnode *x);

  // displays a tree node
void bstShowNode(bstree *T, bsnode *x);

  // writes a tree to a file in order
void bstWrite (bstree *T, bsnode *x, FILE *fp, 
  bstWriteData write);

  // reads a tree from a file
bstree *bstFromFile(char *filename, size_t dataSize,
  bstCmpData cmp, bstStrToData fromStr);

  // returns the number of nodes in the tree
static inline size_t bstSize(bstree *T) {
  return T->size;
}

  // returns an in-order traversal list of the tree
dll *bstInOrder(bstree *T);

  // returns a pre-order traversal list of the tree
dll *bstPreOrder(bstree *T);
  
  // returns a post-order traversal list of the tree
dll *bstPostOrder(bstree *T);

#endif  // BST_H_INCLUDED