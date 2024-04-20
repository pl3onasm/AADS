/* 
  Generic binary search tree implementation 
  Author: David De Potter
  LICENSE: MIT, see LICENSE file in repository root folder
*/

#include "bst.h"
#include "../../../lib/clib/clib.h"

//===================================================================
// Creates a new node with the given data
static bsnode *bstNewNode (bstree *T, void *data) {
  bsnode *n = safeCalloc(1, sizeof(bsnode));
  if (T->copy)
    n->data = T->copy(data);
  else
    n->data = data;
  n->parent = n->left = n->right = T->NIL;
  return n;
}

//===================================================================
// Creates a new binary search tree
bstree *bstNew (bstCmpData cmp) {
  
  bstree *T = safeCalloc(1, sizeof(bstree));
  T->cmp = cmp;
  T->NIL = bstNewNode(T, NULL);
  T->ROOT = T->NIL;
  return T;
}

//===================================================================
// Makes the tree make copies of the data
void bstCopyData (bstree *T, bstCpyData copy, bstFreeData free) {
  T->copy = copy;
  T->free = free;
}

//===================================================================
// Sets the show function for the tree
void bstSetShow (bstree *T, bstShowData show) {
  T->show = show;
}

//===================================================================
// Sets the tree to own the data; i.e. the tree will free the data
// when the tree is deallocated
void bstOwnData (bstree *T, bstFreeData free) {
  T->free = free;
}

//===================================================================
// Deallocates a node
static void bstFreeNode (bstree *T, bsnode *n) { 
  if (n){
    if (T->free)
      T->free(n->data);
    free(n);
  }
}

//===================================================================
// Deallocates all nodes in the subtree rooted at x
static void bstFreeNodes (bstree *T, bsnode *x) {
  if (x != T->NIL) {
    bstFreeNodes(T, x->left);
    bstFreeNodes(T, x->right);
    bstFreeNode(T, x);
  }
}

//===================================================================
// Deallocates the binary search tree
void bstFree (bstree *T) {
  if (T){
    bstFreeNodes(T, T->ROOT);
    free(T->NIL);
    free(T);
  }
}

//===================================================================
// Checks if the tree is empty
bool bstIsEmpty (bstree *T) {
  return T->ROOT == T->NIL;
}

//===================================================================
// Inserts a new node into the tree
void bstInsert (bstree *T, void *data) {
  bsnode *z = bstNewNode(T, data);
  bsnode *y = T->NIL;
  bsnode *x = T->ROOT;

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
  T->size++;
}

//===================================================================
// Searches the tree for a key
bsnode *bstSearch (bstree *T, void *key) {
  bsnode *x = T->ROOT;
  while (x != T->NIL && T->cmp(x->data, key) != 0) {
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
// Replaces the subtree rooted at u with the subtree rooted at v
static void bstTransplant (bstree *T, bsnode *u, bsnode *v) {
  if (u->parent == T->NIL)
    T->ROOT = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

//===================================================================
// Returns the node with the smallest key in the subtree rooted at x
bsnode *bstMinimum (bstree *T, bsnode *x) {
  while (x->left != T->NIL)
    x = x->left;
  return x;
}

//===================================================================
// Returns the maximum node in the tree
bsnode *bstMaximum (bstree *T, bsnode *x) {
  while (x->right != T->NIL)
    x = x->right;
  return x;
}

//===================================================================
// Deletes a node from the tree
void bstDelete (bstree *T, bsnode *z) {
  if (z->left == T->NIL)
    bstTransplant(T, z, z->right);
  else if (z->right == T->NIL)
    bstTransplant(T, z, z->left);
  else {
    bsnode *y = bstMinimum(T, z->right);
    if (y->parent != z) {
      bstTransplant(T, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    bstTransplant(T, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
  bstFreeNode(T, z);
  T->size--;
}

//===================================================================
// Returns the successor of a node
bsnode *bstSuccessor (bstree *T, bsnode *x) {
  if (x->right != T->NIL)
    return bstMinimum(T, x->right);
  bsnode *y = x->parent;
  while (y != T->NIL && x == y->right) {
    x = y;
    y = y->parent;
  }
  return y;
}

//===================================================================
// Returns the predecessor of a node
bsnode *bstPredecessor (bstree *T, bsnode *x) {
  if (x->left != T->NIL)
    return bstMaximum(T, x->left);
  bsnode *y = x->parent;
  while (y != T->NIL && x == y->left) {
    x = y;
    y = y->parent;
  }
  return y;
}

//===================================================================
// Shows the data in the tree in order, 20 items at a time
static void bstShowAll (bstree *T, bsnode *x, short *count) {
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  char buffer[100], ch;
  if (x != T->NIL) {
    bstShowAll(T, x->left, count);
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
    bstShowAll(T, x->right, count);
  }
}

//===================================================================
// Shows the data in the tree in order
void bstShow (bstree *T, bsnode *x) {
  short count = 0;
  bstShowAll(T, x, &count);
  printf("\n");
}

//===================================================================
// Shows all levels of the tree
static void bstShowLevels (bstree *T, bsnode *x, size_t level) {
  if (x == T->NIL) return;
  if (! level) {
    printf("ROOT\n");
    bstShowNode(T, x);
    printf("\n");
  } else {
    for (size_t i = 0; i < level; i++)
      printf("-");
    if (x->parent->left == x)
      printf("|L(%zu): ", level);
    else
      printf("|R(%zu): ", level);
    bstShowNode(T, x);
    printf("\n");

  }
  bstShowLevels(T, x->left, level + 1);
  bstShowLevels(T, x->right, level + 1);
}

//===================================================================
// Shows the structure of the tree rooted at x
void bstShowTree (bstree *T, bsnode *x) {
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  size_t level = 0;

  printf("--------------\n"
         "Tree structure\n"
         "--------------\n");
  bstShowLevels(T, x, level);
}


//===================================================================
// Shows the data in a node
void bstShowNode (bstree *T, bsnode *n) { 
  if (! T->show) {
    fprintf(stderr, "Error: show function not set\n");
    return;
  }
  if (n) T->show(n->data);
}

//===================================================================
// Writes the data in the tree in order to given file
void bstWrite (bstree *T, bsnode *x, FILE *fp, 
               bstWriteData write) {
  
  if (x != T->NIL) {
    bstWrite(T, x->left, fp, write);
    write(x->data, fp);
    bstWrite(T, x->right, fp, write);
  }
}

//===================================================================
// Reads data from a file and inserts it into the tree
bstree *bstFromFile (char *filename, size_t dataSize,
    bstCmpData cmp, bstStrToData fromStr) {
  
  FILE *fp; char buffer[100];
  size_t lineNr = 0;

  fp = fopen(filename, "r");
  if (! fp) {
    printf("Error: could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  bstree *T = bstNew(cmp);
  while (fgets(buffer, 100, fp) != NULL) {
    lineNr++;
    void *data = safeCalloc(1, dataSize);
    if (! fromStr(data, buffer)) {
      printf("Error: invalid input data on line %lu.\n"
             "Check file %s for errors and try again.\n", 
              lineNr, filename);
      if (T->free) 
        T->free(data);
      bstFree(T);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    bstInsert(T, data);
  }
  printf("Data successfully read from file %s\n", filename);
  fclose(fp);
  return T;
}

//===================================================================
// Traverses the tree in order and adds the data to a list
static void bstInOrderList (bstree *T, bsnode *x, dll *L) {
  if (x != T->NIL) {
    bstInOrderList(T, x->left, L);
    dllPushBack(L, x->data);
    bstInOrderList(T, x->right, L);
  }
}

//===================================================================
// Returns an in-order traversal list of the tree
dll *bstInOrder (bstree *T) {
  dll *L = dllNew();
  bstInOrderList(T, T->ROOT, L);
  return L;
}

//===================================================================
// Traverses the tree in pre-order and adds the data to a list
void bstPreOrderList (bstree *T, bsnode *x, dll *L) {
  if (x != T->NIL) {
    dllPushBack(L, x->data);
    bstPreOrderList(T, x->left, L);
    bstPreOrderList(T, x->right, L);
  }
}

//===================================================================
// Returns a pre-order traversal list of the tree
dll *bstPreOrder (bstree *T) {
  dll *L = dllNew();
  bstPreOrderList(T, T->ROOT, L);
  return L;
}

//===================================================================
// Traverses the tree in post-order and adds the data to a list
void bstPostOrderList (bstree *T, bsnode *x, dll *L) {
  if (x != T->NIL) {
    bstPostOrderList(T, x->left, L);
    bstPostOrderList(T, x->right, L);
    dllPushBack(L, x->data);
  }
}

//===================================================================
// Returns a post-order traversal list of the tree
dll *bstPostOrder (bstree *T) {
  dll *L = dllNew();
  bstPostOrderList(T, T->ROOT, L);
  return L;
}

