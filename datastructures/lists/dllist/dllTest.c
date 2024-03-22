/* 
  Some tests for the doubly linked list
  Author: David De Potter
*/

#include <string.h>

#include "dll.h" 
#include "../../../lib/clib/clib.h"

// person structure
typedef struct {
  char *name;
  int age;
} person;

// copy functions
static void *copyStr(void *data) {
  char *new = safeCalloc(strlen((char *)data) + 1, sizeof(char));
  strcpy(new, (char *)data);
  return new;
}

static void *copyInt(void *data) {
  int *new = safeCalloc(1, sizeof(int));
  *new = *(int *)data;
  return new;
}

// show functions
static void showStr(void *data) {
  printf("%s", (char *)data);
}

static void showInt(void *data) {
  printf("%d", *(int *)data);
}

static void showPerson(void *data) {
  person *p = (person *)data;
  printf("%s (%d)", p->name, p->age);
}

// comparison functions
static int cmpStr(void *a, void *b) {
  return strcmp((char *)a, (char *)b);
}

static int cmpInt(void *a, void *b) {
  return *(int *)a - *(int *)b;
}

static int cmpPerson(void *a, void *b) {
  return strcmp(((person *)a)->name, ((person *)b)->name);
}


int main (){  
  // Some test data
  char *numbers[] = {"zero", "one", "two", "three", "four", 
    "five", "six", "seven", "eight", "nine"};
  
  int digits[] = {9, 8, 7, 5, 4, 3, 2, 1, 0};

  person *people[] = {
    &(person){"Alice", 25},
    &(person){"Bob", 30},
    &(person){"Charlie", 28},
    &(person){"Dan", 27},
    &(person){"Eve", 18},
    &(person){"Frank", 23},
    &(person){"Grace", 20}
  };

  // Tests
  // Data set 1
  dll *N = dllNew();
  for (int i = 0; i < 10; ++i) 
    dllPushBack(N, numbers[i]);
  
  dllSetLabel(N, "Numbers");
  dllSetShow(N, showStr);
  
  dllShow(N);
  
  dllSetCmp(N, cmpStr);
  dllDeleteData(N, "five");
  dllDeleteData(N, "zero");
  
  for (char *data = dllLast(N); data; data = dllPrev(N)) {
    showStr(data);
    printf(dllStart(N) ? "\n" : ", ");
  }

  dll *N2 = dllCopy(N, copyStr, free);
  dllPushBack(N2, "eleven");

  for (char *data = dllFirst(N2); data; data = dllNext(N2)) {
    showStr(data);
    printf(dllEnd(N2) ? "\n" : ", ");
  }

  // Data set 2
  dll *D = dllNew();
  dllCopyData(D, copyInt, free);
  
  for (int i = 0; i < 9; ++i) 
    dllPush(D, &digits[i]);

  dllSetLabel(D, "Digits");
  dllSetShow(D, showInt);
  dllSetCmp(D, cmpInt);
  dllInsert(D, &(int){6});
  dllDeleteData(D, &(int){2});
  dllPush(D, &(int){-1});
  dllShow(D);

  // Data set 3
  dll *P = dllNew();
  for (int i = 0; i < 7; ++i) 
    dllPush(P, people[i]);

  dllSetLabel(P, "People");
  dllSetShow(P, showPerson);

  dllShow(P);

  // search for a person
  dllSetCmp(P, cmpPerson);
  person *p = dllFind(P, &(char *){"Frank"});
  if (p) showPerson(p);

  printf("\n");
  dllFree(N);
  dllFree(N2);
  dllFree(D);
  dllFree(P);
 
  return 0;
}