#include "bst.h"
#include "../../../lib/clib/clib.h"
#include <stdio.h>
#include <time.h>

//===================================================================
// Compares two integers
int cmpInt (void const *a, void const *b) {
  return *(int *)a - *(int *)b;
}

//===================================================================
// Shows an integer
void showInt (void const *a) {
  printf("%d", *(int *)a);
}

//===================================================================

int main (void) {
  srand(time(NULL));
  bstree *T = bstNew(cmpInt);
  bstSetShow(T, showInt);
  bstOwnData(T, free);

  // insert 20 random integers
  for (int i = 0; i < 20; i++) {
    int *d = safeMalloc(sizeof(int));
    *d = rand() % 100;
    bstInsert(T, d);
  }

  // the tree structure shows that the 
  // binary search tree property is satisfied 
  // but that the tree is not guaranteed to be 
  // balanced 
  bstShowTree(T, T->ROOT);
  printf("\n");

  // in-order traversal
  printf("\nIn-order \n");
  printf("---------\n");
  dll *inOrder = bstInOrder(T);
  dllSetShow(inOrder, showInt);
  dllShow(inOrder);
  dllFree(inOrder);
  printf("\n");

  // pre-order traversal
  printf("Pre-order \n");
  printf("---------\n");
  dll *preOrder = bstPreOrder(T);
  dllSetShow(preOrder, showInt);
  dllShow(preOrder);
  dllFree(preOrder);
  printf("\n");

  // post-order traversal
  printf("Post-order \n");
  printf("----------\n");
  dll *postOrder = bstPostOrder(T);
  dllSetShow(postOrder, showInt);
  dllShow(postOrder);
  dllFree(postOrder);
  printf("\n");

  // make a second tree with data that
  // is inserted in ascending order
  bstree *T2 = bstNew(cmpInt);
  bstSetShow(T2, showInt);
  bstOwnData(T2, free);
  for (int i = 1; i < 21; i++) {
    int *d = safeMalloc(sizeof(int));
    *d = i;
    bstInsert(T2, d);
  }

  // the tree structure shows that this 
  // triggers the worst case scenario for 
  // a binary search tree:
  // the tree has become a linked list
  bstShowTree(T2, T2->ROOT);

  bstFree(T);
  bstFree(T2);
  return 0;
}
