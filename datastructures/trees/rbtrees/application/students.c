/* 
  file: rbtstudents.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: this is a simple example of using student records
    as data with a tree as the underlying data structure.
    The student records are stored in a file, one record per line.
    This file is read into a tree, and the user can then
    search for a student by id, delete or insert a student,
    or print all existing records in the tree. 
    The tree is ordered by student number.
  time complexity: all operations are in O(h), where h is the height
    of the tree. Since the tree is a red-black tree, the height is
    guaranteed to be O(log n), where n is the number of nodes 
    in the tree.
*/

#include "../rbt.h"
#include "../../../../lib/clib.h"
#include "student/student.h"

int main (int argc, char *argv[]) {
  FILE *fp; rbnode *n;
  char buffer[500];
  short option, count = 0;

  if (argc != 2) {
    printf("Usage: %s <student records file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  
  rbtree *T = rbtFromFile(argv[1], sizeof(student),
                          cmpStudents, studentFromStr);
  rbtOwnData(T, free);
  rbtSetShow(T, showStudent);

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
        if (fgets(buffer, 100, stdin) && !studentFromStr(s, buffer)) {
          printf("Error: invalid student record\n");
          free(s);
          clearStdin(buffer);
          continue;
        }
        n = rbtSearch(T, s);
        if (n) {
          printf("Error: student with id %d already exists\n", s->id);
          free(s);
        } else {
          // insert into tree
          rbtInsert(T, s);
          printf("Inserted student with id %d\n", s->id);  
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
        student *lookup = newStudent();
        lookup->id = id;
        n = rbtSearch(T, lookup);
        if (n) {
          printf("Delete following student record? (y/n)\n");
          showStudent(n->data);
          char ch;
          if ((fgets(buffer, 10, stdin) && 
               sscanf(buffer, "%c", &ch) != 1) || ch != 'y') {
            free(lookup);
            clearStdin(buffer);
            continue;
          }
          // delete from tree
          rbtDelete(T, n);
          printf("Deleted student with id %d\n", id);
        } else
          printf("Error: student with id %d not found\n", id);
        clearStdin(buffer);
        free(lookup);
        break;
      case 3:
        printf("Enter student id: ");
        if ((fgets(buffer, 20, stdin) && sscanf(buffer, "%d", &id) != 1)
        || id < 1000000 || id > 9999999) {
          printf("Error: invalid id\n");
          clearStdin(buffer);
          continue;
        }
        lookup = newStudent();
        lookup->id = id;
        n = rbtSearch(T, lookup);
        if (n)
          showStudent(n->data);
        else
          printf("Error: student with id %d not found\n", id);
        clearStdin(buffer);
        free(lookup);
        break;
      case 4:
        // print all student records in tree in order
        rbtShow(T, T->ROOT);
        count = 0;
        break;
      case 5:
        printf("Are you sure you want to exit without saving? (y/n): ");
        char ch;
        if ((fgets(buffer, 10, stdin) && 
            sscanf(buffer, "%c", &ch) != 1) || ch != 'y')
          continue;
        rbtFree(T);
        exit(EXIT_SUCCESS);
      case 6:
        // overwrite student records file with updated tree
        fp = fopen(argv[1], "w+");
        if (fp == NULL) {
          printf("Error: could not open file %s\n", argv[1]);
          rbtFree(T);
          exit(EXIT_FAILURE);
        }
        rbtWrite(T, T->ROOT, fp, writeStudent);
        fclose(fp);
        rbtFree(T);
        exit(EXIT_SUCCESS);
      default:
        printf("Error: invalid command\n");
    }
  }
  return 0;
}