#include "student.h"
#include "../../../lib/clib/clib.h"

//::::::::::::::::::::::::: validation functions :::::::::::::::::::://

bool validDOB(char *dob) {
  /* checks if a date of birth is valid */
  int day, month, year;
  if (sscanf(dob, "%d-%d-%d", &year, &month, &day) != 3)
    return false;
  if (day < 1 || day > 31)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (year < 1900 || year > 2017)
    return false;
  return true;
}

bool validStudent(student *s) {
  /* checks if a student record is valid */
  if (s->id < 1000000 || s->id > 9999999)
    return false;
  if (s->gpa < 0 || s->gpa > 10)
    return false;
  if (!validDOB(s->dob))
    return false;
  if (strlen(s->fname) < 2 || strlen(s->lname) < 2)
    return false;
  return true;
}

student *newStudent() {
  /* allocates memory for a new student record */
  student *s = safeCalloc(1, sizeof(student));
  return s;
}

void printStudent(void *a) {
  /* prints a student record, with a specified delimiter for fields */
  student *s = (student *)a;
  printf("%d | %s | %.2lf | %s %s\n", s->id, s->dob, 
          s->gpa, s->fname, s->lname);
}

void writeStudentToFile(void *a, FILE *f) {
  /* prints a student record to a file */
  student *s = (student *)a;
  fprintf(f, "%d %s %.2lf %s %s\n", s->id, s->dob, 
          s->gpa, s->fname, s->lname);
}

int cmpStudents(void *a, void *b) {
  /* compares two student records */
  student *s1 = (student *)a;
  student *s2 = (student *)b;
  return s1->id - s2->id;
}

int cmpStudentById(void *a, void *b) {
  /* compares a student record with an id */
  student *s = (student *)a;
  int *id = (int *)b;
  return s->id - *id;
}

void freeStudent(student *s) {
  /* frees a student record */
  if (s != NULL)
    free(s);
}

bool studentFromStr(void *a, char *str) {
  // parses a student record from a string, and stores it in a
  student *s = (student *)a;
  if (sscanf(str, "%d %s %lf %s %s", &s->id, s->dob, 
      &s->gpa, s->fname, s->lname) != 5)
    return false;
  if (!validStudent(s))
    return false;
  return true;
}
