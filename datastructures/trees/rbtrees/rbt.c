/* 
  Generic red-black tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "rbt.h"
#include "../../../lib/clib/clib.h"

//===================================================================
// Creates a new node with given data
static rbnode *rbtNewNode (rbtree *T, void *data) {
  rbnode *n = safeCalloc(1, sizeof(rbnode));
  if (T->copy)
    n->data = T->copy(data);
  else
    n->data = data;
  n->color = RED;
  n->parent = n->left = n->right = T->NIL;
  return n;
}

//===================================================================
// Creates a new red-black tree
rbtree *rbtNew (rbtCmpData cmp) {
  
  rbtree *T = safeCalloc(1, sizeof(rbtree));
  T->cmp = cmp;
  T->NIL = rbtNewNode(T, NULL);
  T->NIL->color = BLACK;
  T->ROOT = T->NIL;
  return T;
}

//===================================================================
// Makes the tree make copies of the data
void rbtCopyData (rbtree *T, rbtCpyData copy, rbtFreeData free) {
  T->copy = copy;
  T->free = free;
}

//===================================================================
// Sets the tree to own the data
void rbtOwnData (rbtree *T, rbtFreeData free) {
  T->free = free;
}

//===================================================================
// Sets the show function for the tree
void rbtSetShow (rbtree *T, rbtShowData show) {
  T->show = show;
}

//===================================================================
// Deallocates memory for a node
static void rbtFreeNode (rbtree *T, rbnode *n) {
  if (n) {
    if (T->free)
      T->free(n->data);
    free(n);
  }
}

//===================================================================
// Deallocates memory for all nodes in a subtree rooted at x
static void rbtFreeNodes (rbtree *T, rbnode *x) {
  if (x != T->NIL) {
    rbtFreeNodes(T, x->left);
    rbtFreeNodes(T, x->right);
    rbtFreeNode(T, x);
  }
}

//===================================================================
// Deallocates the red-black tree
void rbtFree (rbtree *T) {
  if (T) {
    rbtFreeNodes(T, T->ROOT);
    rbtFreeNode(T, T->NIL);
    free(T);
  }
}

//===================================================================
// Checks if the tree is empty
bool rbtIsEmpty (rbtree *T) {
  return T->ROOT == T->NIL;
}

//===================================================================
// Returns the node with the smallest key in the subtree rooted at x
rbnode *rbtMinimum (rbtree *T, rbnode *x) {
  while (x->left != T->NIL)
    x = x->left;
  return x;
}

//===================================================================
// Returns the node with the largest key in the subtree rooted at x
rbnode *rbtMaximum (rbtree *T, rbnode *x) {
  while (x->right != T->NIL)
    x = x->right;
  return x;
}

//===================================================================
// Returns the successor of a node
rbnode *rbtSuccessor (rbtree *T, rbnode *x) {
  if (x->right != T->NIL)
    return rbtMinimum(T, x->right);
  rbnode *y = x->parent;
  while (y != T->NIL && x == y->right) {
    x = y;
    y = y->parent;
  }
  return y;
}

//===================================================================
// Returns the predecessor of a node
rbnode *rbtPredecessor (rbtree *T, rbnode *x) {
  if (x->left != T->NIL)
    return rbtMaximum(T, x->left);
  rbnode *y = x->parent;
  while (y != T->NIL && x == y->left) {
    x = y;
    y = y->parent;
  }
  return y;
}

//===================================================================
// Searches for a key in the red-black tree
rbnode *rbtSearch (rbtree *T, void *key) {
  rbnode *x = T->ROOT;
  while (x != T->NIL && T->cmp(x->data, key) != 0){
    if (T->cmp(key, x->data) < 0)
      x = x->left;
    else
      x = x->right;
  }
  if (x == T->NIL)
    return NULL;
  return x;
}

//===================================================================
// Performs a left rotation on the subtree rooted at x
static void leftRotate (rbtree *T, rbnode *x) { 
  rbnode *y = x->right;
  x->right = y->left;
  if (y->left != T->NIL)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == T->NIL)
    T->ROOT = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

//===================================================================
// Performs a right rotation on the subtree rooted at x
static void rightRotate (rbtree *T, rbnode *x) {
  rbnode *y = x->left;
  x->left = y->right;
  if (y->right != T->NIL)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == T->NIL)
    T->ROOT = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else 
    x->parent->left = y;
  y->right = x;
  x->parent = y;
}

//===================================================================
// Restores the red-black properties after insertion of z
void insertFixup (rbtree *T, rbnode *z) {
  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      rbnode *y = z->parent->parent->right;
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
      rbnode *y = z->parent->parent->left;
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
  T->ROOT->color = BLACK;
}

//===================================================================
// Inserts a node into the red-black tree
void rbtInsert (rbtree *T, void *data) {
  rbnode *z = rbtNewNode(T, data);
  rbnode *y = T->NIL;
  rbnode *x = T->ROOT;

  while (x != T->NIL) {
    y = x;
    if (T->cmp(z->data, x->data) < 0)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;
  if (y == T->NIL)
    T->ROOT = z;
  else if (T->cmp(z->data, y->data) < 0)
    y->left = z;
  else
    y->right = z;
  insertFixup(T, z);
}

//===================================================================
// Replaces the subtree rooted at u with the subtree rooted at v
static void rbtTransplant (rbtree *T, rbnode *u, rbnode *v) {
  if (u->parent == T->NIL)
    T->ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else 
    u->parent->right = v;
  v->parent = u->parent;
}

//===================================================================
// Restores the red-black properties after deletion
static void deleteFixup (rbtree *T, rbnode *x) {
  while (x != T->ROOT && x->color == BLACK) {
    if (x == x->parent->left) {
      rbnode *w = x->parent->right;
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
        x = T->ROOT;
      }
    } else {
      rbnode *w = x->parent->left;
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
        x = T->ROOT;
      }
    }
  }
  x->color = BLACK;
}

//===================================================================
// Deletes a node from the red-black tree
void rbtDelete (rbtree *T, rbnode *z) {
  rbnode *y = z;
  rbnode *x;
  char y_originalColor = y->color;
  if (z->left == T->NIL) {
    x = z->right;
    rbtTransplant(T, z, z->right);
  } else if (z->right == T->NIL) {
    x = z->left;
    rbtTransplant(T, z, z->left);
  } else {
    y = rbtMinimum(T, z->right);
    y_originalColor = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else {
      rbtTransplant(T, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtTransplant(T, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_originalColor == BLACK)
    deleteFixup(T, x);
  rbtFreeNode(T, z);
}

//===================================================================
// Shows the data in the tree in order, 20 items at a time
static void rbtShowAll (rbtree *T, rbnode *x, short *count) {
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  char buffer[100], ch;
  if (x != T->NIL) {
    rbtShowAll(T, x->left, count);
    if (*count < 20){
      T->show(x->data);
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
    rbtShowAll(T, x->right, count);
  }
}

//===================================================================
// Shows the data in the tree in order
void rbtShow (rbtree *T, rbnode *x) {
  short count = 0;
  rbtShowAll(T, x, &count);
  printf("\n");
}

//===================================================================
// Shows the data in the data in a node
void rbtShowNode (rbtree *T, rbnode *n) {
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  if (n) T->show(n->data);
}

//===================================================================
// Shows all levels of the tree
static void rbtShowLevels (rbtree *T, rbnode *x, size_t level) {
  if (x == T->NIL) return;
  if (! level) {
    printf("ROOT\n");
    rbtShowNode(T, x);
    printf("\n");
  } else {
    for (size_t i = 0; i < level; i++)
      printf("-");
    if (x->parent->left == x)
      printf("|L(%zu): ", level);
    else
      printf("|R(%zu): ", level);
    rbtShowNode(T, x);
    printf("\n");

  }
  rbtShowLevels(T, x->left, level + 1);
  rbtShowLevels(T, x->right, level + 1);
}

//===================================================================
// Shows the tree structure
void rbtShowTree (rbtree *T, rbnode *x) {
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  size_t level = 0;

  printf("--------------\n"
         "Tree structure\n"
         "--------------\n");
  rbtShowLevels(T, x, level);
}

//===================================================================
// Writes the data in the tree in order to a file
void rbtWrite (rbtree *T, rbnode *x, FILE *fp, rbtWriteData write) {
  if (x != T->NIL) {
    rbtWrite(T, x->left, fp, write);
    write(x->data, fp);
    rbtWrite(T, x->right, fp, write);
  }
}

//===================================================================
// Reads data from a file and inserts it into the tree
rbtree *rbtFromFile (char *filename, size_t dataSize,
    rbtCmpData cmp, rbtStrToData fromStr) {
  
  FILE *fp; char buffer[100];
  size_t lineNr = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  rbtree *T = rbtNew(cmp);
  while (fgets(buffer, 100, fp) != NULL) {
    lineNr++;
    void *data = safeCalloc(1, dataSize);
    if (! fromStr(data, buffer)) {
      printf("Error: invalid input data on line %lu.\n"
             "Check file %s for errors and try again.\n", 
              lineNr, filename);
      if (T->free)
        T->free(data);
      rbtFree(T);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    rbtInsert(T, data);
  }
  printf("Data successfully read from file %s\n", filename);
  fclose(fp);
  return T;
}

//===================================================================
// Traverses the tree in order and adds the data to a list
static void rbtInOrderList (rbtree *T, rbnode *x, dll *list) {
  if (x != T->NIL) {
    rbtInOrderList(T, x->left, list);
    dllPushBack(list, x->data);
    rbtInOrderList(T, x->right, list);
  }
}

//===================================================================
// Returns an in-order traversal list of the tree
dll *rbtInOrder (rbtree *T) {
  dll *list = dllNew();
  rbtInOrderList(T, T->ROOT, list);
  return list;
}

//===================================================================
// Traverses the tree in pre-order and adds the data to a list  
static void rbtPreOrderList (rbtree *T, rbnode *x, dll *list) {
  if (x != T->NIL) {
    dllPushBack(list, x->data);
    rbtPreOrderList(T, x->left, list);
    rbtPreOrderList(T, x->right, list);
  }
}

//===================================================================
// Returns a pre-order traversal list of the tree
dll *rbtPreOrder (rbtree *T) {
  dll *list = dllNew();
  rbtPreOrderList(T, T->ROOT, list);
  return list;
}

//===================================================================
// Traverses the tree in post-order and adds the data to a list
static void rbtPostOrderList (rbtree *T, rbnode *x, dll *list) {
  if (x != T->NIL) {
    rbtPostOrderList(T, x->left, list);
    rbtPostOrderList(T, x->right, list);
    dllPushBack(list, x->data);
  }
}

//===================================================================
// Returns a post-order traversal list of the tree
dll *rbtPostOrder (rbtree *T) {
  dll *list = dllNew();
  rbtPostOrderList(T, T->ROOT, list);
  return list;
}

//===================================================================
// Returns the height of the tree
size_t rbtHeight (rbtree *T) {
  size_t height = 0;
  rbnode *x = T->ROOT;
  while (x != T->NIL) {
    height++;
    x = x->left;
  }
  return height;
}