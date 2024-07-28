/*
  Application for priority queues
  File: sudoku.c
  Author: David De Potter
  Description: This program solves a sudoku puzzle
    using a binary heap, a mrv heuristic and forward
    checking. The program reads a sudoku puzzle from
    a file and prints the solution to the screen.

    Implementation in C:
    Priority queue of singly linked lists which are
    the domains of the variables ranging from 1 to 9.
    At each step, the program selects the variable with
    the smallest domain and assigns it a value. The
    domains of the other variables are updated and the
    process is repeated until the puzzle is solved.
    This means that at each step, the key = list length
    must be updated in the heap.
*/

#include "binMinHeap.h"
#include "../../../lib/clib/clib.h"

#define SIZE 9
#define EMPTY '.'

void readBoard(char board[][SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      scanf("%c", &board[i][j]);
    }
  }
}

void cmp(void *a, void *b) {
  return *(int *)a - *(int *)b;
}

void updateDomains(char board[][SIZE][SIZE], int i, int j, heap *H) {
  for (int k = 0; k < SIZE; k++) {
    if (board[i][k] == EMPTY) {
      deleteKey(H, i * SIZE + k, NULL);
    }
    if (board[k][j] == EMPTY) {
      deleteKey(H, k * SIZE + j, NULL);
    }
  }
  int startRow = (i / 3) * 3;
  int startCol = (j / 3) * 3;
  for (int k = startRow; k < startRow + 3; k++) {
    for (int l = startCol; l < startCol + 3; l++) {
      if (board[k][l] == EMPTY) {
        deleteKey(H, k * SIZE + l, NULL);
      }
    }
  }
}

void initHeap(heap *H, char board[][SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == EMPTY) {
        insertToMinHP(H, i * SIZE + j,
      }
    }
  }
} 

int main () {
  // declare a board with 9x9 cells holding 10 possible values
  char board[SIZE][SIZE][SIZE] = {0};
  readBoard(board);
  heap *H = newMinHP(SIZE * SIZE);
  initHeap(H, board);

  return 0;
}
