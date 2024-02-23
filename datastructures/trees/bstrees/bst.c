/* 
  Generic binary search tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "bst.h"
#include "../../../lib/clib/clib.h"

//::::::::::::::::::::::: Memory management :::::::::::::::::::::::://

node *newNode (tree *T, void *data) {
  // allocates memory for a new node
  node *n = safeCalloc(1, sizeof(node));
  n->data = data;
  n->parent = n->left = n->right = NIL;
  return n;
}

tree *newTree (void) {
  // allocates memory for a new tree
  tree *T = safeCalloc(1, sizeof(tree));
  NIL = newNode(T, NULL);
  ROOT = NIL;
  return T;
}

void freeNode (node *n) {
  // frees a node 
  if (n != NULL){
    free(n->data);
    free(n);
  }
}

void freeNodes (node *x, tree *T) {
  // frees all nodes in the subtree rooted at x 
  if (x != NIL) {
    freeNodes(x->left, T);
    freeNodes(x->right, T);
    freeNode(x);
  }
}

void freeTree (tree *T) {
  // entirely frees a tree
  if (T){
    freeNodes(ROOT, T);
    free(NIL);
    free(T);
  }
}

//:::::::::::::::::::::::: Tree operations ::::::::::::::::::::::::://

void insertNode (tree *T, node *z, int (*cmp)(void *, void *)) {
  // inserts a node into the tree, 
  // using the given comparison function
  node *y = NIL;
  node *x = ROOT;

  while (x != NIL) {
    y = x;
    if (cmp(z->data, x->data) < 0)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == NIL)
    ROOT = z;
  else if (cmp(z->data, y->data) < 0)
    y->left = z;
  else
    y->right = z;
}

node *searchKey (tree *T, void *key, int (*cmp)(void *, void *)) {
  // searches for a key in the tree 
  node *x = ROOT;
  while (x != NIL && cmp(x->data, key) != 0){
    if (cmp(key, x->data) < 0)
      x = x->left;
    else
      x = x->right;
  }
  return x;
}

void transplant (tree *T, node *u, node *v) {
  // replaces node u with node v in the tree 
  if (u->parent == NULL)
    ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != NULL)
    v->parent = u->parent;
}

node *treeMinimum (tree *T, node *x) {
  // returns the node with the smallest key in 
  // the subtree rooted at x 
  while (x->left != NIL)
    x = x->left;
  return x;
}

void deleteNode (tree *T, node *z) {
  // deletes a node from the tree 
  if (z->left == NULL)
    transplant(T, z, z->right);
  else if (z->right == NULL)
    transplant(T, z, z->left);
  else {
    node *y = treeMinimum(T, z->right);
    if (y->parent != z) {
      transplant(T, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(T, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
  freeNode(z);
}

//::::::::::::::::::::::::::: Printing ::::::::::::::::::::::::::::://

void printTree (tree *T, node *x, short *count, 
    void (*printData)(void *)) {
  // prints the data in the tree in order, 20 items at a time 
  char buffer[100], ch;
  if (x != NIL) {
    printTree(T, x->left, count, printData);
    if (*count < 20){
      printData(x->data);
      *count += 1;
    } else if (*count == 20){
      printf("Print 20 more? (y/n): ");
      if ((fgets (buffer, 100, stdin) && 
      sscanf(buffer, "%c", &ch) != 1) || ch != 'y')
        *count = 21;
      else
        *count = 0;
      clearStdin(buffer);
    }
    printTree(T, x->right, count, printData);
  }
}

void printNode (node *n, void (*printData)(void *)) {
  // prints the data in a node 
  if (n) printData(n->data);
}

//:::::::::::::::::::::::: File operations ::::::::::::::::::::::::://

void writeTreeToFile (tree *T, node *x, FILE *fp, 
    void (*printData)(void *, FILE *)) {
  // writes the data in the tree in order to given file 
  if (x != NIL) {
    writeTreeToFile(T, x->left, fp, printData);
    printData(x->data, fp);
    writeTreeToFile(T, x->right, fp, printData);
  }
}

void buildTreeFromFile (tree *T, char *filename, size_t dataSize,
    int (*cmp)(void *, void *), bool (*dataFromStr)(void *, char *)) {
  // reads data from input file and inserts it into the tree
  FILE *fp; char buffer[100];
  size_t lineNr = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s\n", filename);
    freeTree(T);
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, 100, fp) != NULL) {
    lineNr++;
    void *data = safeCalloc(1, dataSize);
    if (! dataFromStr(data, buffer)) {
      printf("Error: invalid input data on line %lu.\n"
             "Check file %s for errors and try again.\n", lineNr, filename);
      free(data);
      freeTree(T);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    node *n = newNode(T, data);
    insertNode(T, n, cmp);
  }
  printf("Data successfully read from file %s\n", filename);
  fclose(fp);
}