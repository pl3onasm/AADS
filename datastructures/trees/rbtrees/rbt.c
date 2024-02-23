/* 
  Generic red-black tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "rbt.h"
#include "../../../lib/clib/clib.h"

//::::::::::::::::::::::: Memory management :::::::::::::::::::::::://

node *newNode (tree *T, void *data) {
  // allocates memory for a new node
  node *n = safeCalloc(1, sizeof(node));
  n->data = data;
  n->color = RED;
  n->parent = n->left = n->right = NIL;
  return n;
}

tree *newTree (void) {
  // allocates memory for a new tree
  tree *T = safeCalloc(1, sizeof(tree));
  NIL = newNode(T, NULL);
  NIL->color = BLACK;
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
  freeNodes(ROOT, T);
  free(NIL);
  free(T);
}

//:::::::::::::::::::::::: Tree operations ::::::::::::::::::::::::://

node *treeMinimum (tree *T, node *x) {
  // returns the node with the smallest key in 
  // the subtree rooted at x 
  while (x->left != NIL)
    x = x->left;
  return x;
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

void leftRotate (tree *T, node *x) {
  // performs a left rotation on the subtree rooted at x 
  node *y = x->right;
  x->right = y->left;
  if (y->left != NIL)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == NIL)
    ROOT = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void rightRotate (tree *T, node *x) {
  // performs a right rotation on the subtree rooted at x 
  node *y = x->left;
  x->left = y->right;
  if (y->right != NIL)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == NIL)
    ROOT = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else 
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void insertFixup (tree *T, node *z) {
  // restores the red-black properties after insertion of z 
  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      node *y = z->parent->parent->right;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          leftRotate(T, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rightRotate(T, z->parent->parent);
      }
    } else {
      node *y = z->parent->parent->left;
      if (y->color == RED) {
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rightRotate(T, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        leftRotate(T, z->parent->parent);
      }
    }
  }
  ROOT->color = BLACK;
}

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
  insertFixup(T, z);
}

void transplant (tree *T, node *u, node *v) {
  // replaces the subtree rooted at u with the subtree rooted at v 
  if (u->parent == NIL)
    ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else 
    u->parent->right = v;
  v->parent = u->parent;
}

void deleteFixup (tree *T, node *x) {
  // restores the red-black properties after deletion 
  while (x != ROOT && x->color == BLACK) {
    if (x == x->parent->left) {
      node *w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        leftRotate(T, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rightRotate(T, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        leftRotate(T, x->parent);
        x = ROOT;
      }
    } else {
      node *w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rightRotate(T, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          leftRotate(T, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rightRotate(T, x->parent);
        x = ROOT;
      }
    }
  }
  x->color = BLACK;
}

void deleteNode (tree *T, node *z) {
  // deletes a node from the tree 
  node *y = z;
  node *x;
  char y_originalColor = y->color;
  if (z->left == NIL) {
    x = z->right;
    transplant(T, z, z->right);
  } else if (z->right == NIL) {
    x = z->left;
    transplant(T, z, z->left);
  } else {
    y = treeMinimum(T, z->right);
    y_originalColor = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else {
      transplant(T, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(T, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_originalColor == BLACK)
    deleteFixup(T, x);
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