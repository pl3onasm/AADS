#include "../rbt.h"
#include "../../../../lib/clib.h"
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
  rbtree *T = rbtNew(cmpInt);
  rbtSetShow(T, showInt);
  rbtOwnData(T, free);

  // insert 20 random integers
  for (int i = 0; i < 20; i++) {
    int *d = safeMalloc(sizeof(int));
    *d = rand() % 100;
    rbtInsert(T, d);
  }

  // the tree structure shows that the 
  // red-black tree properties are satisfied
  // and that the tree is balanced
  rbtShowTree(T, T->ROOT);
  printf("\n");

  // in-order traversal
  printf("\nIn-order \n");
  printf("---------\n");
  dll *inOrder = rbtInOrder(T);
  dllSetShow(inOrder, showInt);
  dllShow(inOrder);
  dllFree(inOrder);
  printf("\n");

  // pre-order traversal
  printf("Pre-order \n");
  printf("---------\n");
  dll *preOrder = rbtPreOrder(T);
  dllSetShow(preOrder, showInt);
  dllShow(preOrder);
  dllFree(preOrder);
  printf("\n");

  // post-order traversal
  printf("Post-order \n");
  printf("----------\n");
  dll *postOrder = rbtPostOrder(T);
  dllSetShow(postOrder, showInt);
  dllShow(postOrder);
  dllFree(postOrder);
  printf("\n");

  // make a second tree with data that
  // is inserted in ascending order
  rbtree *T2 = rbtNew(cmpInt);
  rbtSetShow(T2, showInt);
  rbtOwnData(T2, free);
  for (int i = 1; i < 21; i++) {
    int *d = safeMalloc(sizeof(int));
    *d = i;
    rbtInsert(T2, d);
  }

  // the tree structure shows that the
  // worst-case scenario for a binary search
  // tree does not apply to red-black trees:
  // the tree is balanced and the height is
  // O(log n)
  rbtShowTree(T2, T2->ROOT);

  rbtFree(T);
  rbtFree(T2);
  return 0;
}
