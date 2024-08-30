#include "student.h"
#include "../../../../../lib/clib.h"

//::::::::::::::::::::::::: validation functions :::::::::::::::::::://

bool validDOB(char const *dob) {
  // checks if a date of birth is valid
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

bool validStudent(student const *s) {
  // checks if a student record is valid 
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

//::::::::::::::::::::::::: comparison functions :::::::::::::::::::://

int cmpStudents(void const *a, void const *b) {
  /* compares two student records */
  student *s1 = (student *)a;
  student *s2 = (student *)b;
  return s1->id - s2->id;
}

//::::::::::::::::::::::::::: memory management ::::::::::::::::::::://

student *newStudent() {
  /* allocates memory for a new student record */
  student *s = safeCalloc(1, sizeof(student));
  return s;
}

//::::::::::::::::::::::::::::: I/O functions ::::::::::::::::::::::://

void showStudent(void const *a) {
  /* prints a student record, with a specified delimiter for fields */
  student *s = (student *)a;
  printf("%d | %s | %.2lf | %s %s\n", s->id, s->dob, 
          s->gpa, s->fname, s->lname);
}

void writeStudent(void const *a, FILE *f) {
  /* prints a student record to a file */
  student *s = (student *)a;
  fprintf(f, "%d %s %.2lf %s %s\n", s->id, s->dob, 
          s->gpa, s->fname, s->lname);
}

bool studentFromStr(void *a, char const *str) {
  // tries to parse a student record from a string
  // stores the result in a, returns true if successful
  student *s = (student *)a;
  if (sscanf(str, "%d %s %lf %s %s", &s->id, s->dob, 
      &s->gpa, s->fname, s->lname) != 5)
    return false;
  if (!validStudent(s))
    return false;
  return true;
}
