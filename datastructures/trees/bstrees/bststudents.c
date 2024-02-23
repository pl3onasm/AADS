/* file: bststudents.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: this is a simple example of a binary search tree
    using student records as data.
    The student records are stored in a file, one record per line.
    This file is read into a binary search tree, and the user can
    then search for a student by id, delete or insert a student,
    or print all existing records in the tree. 
    The tree is ordered by student number.
   time complexity: all operations are in O(h), where h is the height
    of the tree. If the tree is balanced, this is O(lg n), where n
    is the number of nodes in the tree. If the tree is unbalanced,
    this is O(n) in the worst case.

    Compilation:
    $ gcc -O2 -std=c99 -pedantic -o a.out bst.c bststudents.c student.c ../../../lib/clib/*.c -lm
*/

#include "../../../lib/clib/clib.h"

#include "bst.h"
#include "student.h"

int main (int argc, char *argv[]) {
  FILE *fp; node *n;
  char buffer[500];
  short option, d, count = 0;

  if (argc != 2) {
    printf("Usage: %s <student records file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  bst *tree = newBST();
  readBSTfromFile(tree, argv[1], sizeof(student),
                  cmpStudents, studentFromStr);

  while (true) {
    
    printf("\nEnter command: \n"
            "(1) insert\n"
            "(2) delete\n"
            "(3) search\n"
            "(4) print\n"
            "(5) exit\n"
            "(6) save and exit\n\n");

    if (fgets(buffer, 10, stdin) && sscanf(buffer, "%hd", &option) != 1) {
      // clear input buffer
      clearStdin(buffer);
      printf("Error: invalid command\n"); 
      continue;
    }
    
    switch (option) {
      case 1:
        printf("Enter student id, dob, gpa, first name, last name: ");
        student *s = newStudent();
        if ((fgets(buffer, 100, stdin) && sscanf(buffer, "%d %s %lf %s %s", 
            &s->id, s->dob, &s->gpa, s->fname, s->lname) != 5) 
            || !validStudent(s)) {
          printf("Error: invalid student record\n");
          free(s);
          clearStdin(buffer);
          continue;
        }
        n = BSTsearch(ROOT, &(s->id), cmpStudents);
        if (!n) {
          // insert into BST
          n = newBSTnode(s);
          BSTinsert(tree, n, cmpStudents);
          printf("Inserted student with id %d\n", s->id);
        } else {
          printf("Error: student with id %d already exists\n", s->id);
          free(s);
        }
        clearStdin(buffer);
        break;
      case 2:
        printf("Enter student id: ");
        int id;
        if ((fgets(buffer, 20, stdin) && sscanf(buffer, "%d", &id) != 1)
        || id < 1000000 || id > 9999999) {
          printf("Error: invalid id\n");
          clearStdin(buffer);
          continue;
        }
        n = BSTsearch(ROOT, &id, cmpStudentById);
        if (n != NULL) {
          // delete from BST
          BSTdelete(tree, n);
          printf("Deleted student with id %d\n", id);
        } else
          printf("Error: student with id %d not found\n", id);
        break;
      case 3:
        printf("Enter student id: ");
        if ((fgets(buffer, 20, stdin) && sscanf(buffer, "%d", &id) != 1)
        || id < 1000000 || id > 9999999) {
          printf("Error: invalid id\n");
          clearStdin(buffer);
          continue;
        }
        n = BSTsearch(ROOT, &id, cmpStudentById);
        if (n != NULL)
          printStudent(n->data);
        else
          printf("Error: student with id %d not found\n", id);
        clearStdin(buffer);
        break;
      case 4:
        // print all student records in BST in order, using printStudent
        printBST(ROOT, &count, printStudent);
        count = 0;
        break;
      case 5:
        printf("Are you sure you want to exit without saving? (y/n): ");
        char ch;
        if ((fgets(buffer, 10, stdin) && 
            sscanf(buffer, "%c", &ch) != 1) || ch != 'y')
          continue;
        freeBST(tree);
        exit(EXIT_SUCCESS);
      case 6:
        // overwrite student records file with updated BST
        fp = fopen(argv[1], "w+");
        if (fp == NULL) {
          printf("Error: could not open file %s\n", argv[1]);
          freeBST(tree);
          exit(EXIT_FAILURE);
        }
        writeBSTtoFile(ROOT, fp, writeStudentToFile);
        fclose(fp);
        freeBST(tree);
        exit(EXIT_SUCCESS);
      default:
        printf("Error: invalid command\n");
    }
  }
  return 0;
}


