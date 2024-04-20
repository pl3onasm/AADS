/* 
  Generic red-black tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#ifndef RBT_H_INCLUDED
#define RBT_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "../../lists/dllist/dll.h"

// function pointer types
typedef int (*rbtCmpData)(void const *a, void const *b);
typedef void (*rbtShowData)(void const *data);
typedef void (*rbtWriteData)(void const *data, FILE *file);
typedef bool (*rbtStrToData)(void *data, char const *str);
typedef void (*rbtFreeData)(void *data);
typedef void *(*rbtCpyData)(void const *data);

// data structures and types
typedef struct rbnode {
  void *data;                 // data stored in the node
  enum {RED, BLACK} color;    // color of the node
  struct rbnode *parent;      // parent node
  struct rbnode *left;        // left child
  struct rbnode *right;       // right child
} rbnode;

typedef struct {
  rbnode *ROOT, *NIL;         // root and sentinel nodes
  rbtCmpData cmp;             // comparison function
  rbtShowData show;           // show function
  rbtWriteData write;         // function to write data to file
  rbtStrToData fromStr;       // parse string to data
  rbtFreeData free;           // function to free data
  rbtCpyData copy;            // function to copy data
  size_t size;                // number of tree nodes
} rbtree; 

// function prototypes

  // creates a new red-black tree
rbtree *rbtNew (rbtCmpData cmp);

  // makes the tree make copies of the data
void rbtCopyData (rbtree *T, rbtCpyData copy, 
                  rbtFreeData free);

  // sets the tree to own the data
void rbtOwnData (rbtree *T, rbtFreeData free);
  
  // sets the show function for the tree
void rbtSetShow (rbtree *T, rbtShowData show);
  
  // returns true if the tree is empty
bool rbtIsEmpty (rbtree *T);

  // inserts a new node into the tree
void rbtInsert (rbtree *T, void *data);

  // deallocates the tree
void rbtFree (rbtree *T);

  // searches the tree for a key
rbnode *rbtSearch (rbtree *T, void *key);

  // deletes a node from the tree
void rbtDelete (rbtree *T, rbnode *z);

  // returns the minimum node in the tree
rbnode *rbtMinimum (rbtree *T, rbnode *x);

  // returns the maximum node in the tree
rbnode *rbtMaximum (rbtree *T, rbnode *x);
  
  // returns the successor of a node
rbnode *rbtSuccessor (rbtree *T, rbnode *x);

  // returns the predecessor of a node
rbnode *rbtPredecessor (rbtree *T, rbnode *x);

  // displays the (sub)tree rooted at x in order
void rbtShow (rbtree *T, rbnode *x);

  // shows the structure of the tree rooted at x
void rbtShowTree (rbtree *T, rbnode *x);

  // shows a tree node
void rbtShowNode (rbtree *T, rbnode *x);

  // writes a tree to a file in order
void rbtWrite (rbtree *T, rbnode *x, FILE *fp, 
  rbtWriteData write);

  // reads a tree from a file
rbtree *rbtFromFile (char *filename, size_t dataSize,
  rbtCmpData cmp, rbtStrToData fromStr);

  // returns the number of nodes in the tree
static inline size_t rbtSize (rbtree *T) {
  return T->size;
}

  // returns the height of the tree
size_t rbtHeight (rbtree *T);

  // returns an in-order traversal list of the tree
dll *rbtInOrder (rbtree *T);

  // returns a pre-order traversal list of the tree
dll *rbtPreOrder (rbtree *T);

  // returns a post-order traversal list of the tree
dll *rbtPostOrder (rbtree *T);

#endif  // RBT_H_INCLUDED