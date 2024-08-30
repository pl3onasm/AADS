#ifndef STUDENT_H_INCLUDED
#define STUDENT_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

#define MAX_NAME_LEN 50

typedef struct {
  int id;
  double gpa;
  char dob[11];
  char fname[MAX_NAME_LEN];
  char lname[MAX_NAME_LEN];
} student;

student *newStudent(void);

bool validStudent(student const *s); 

void showStudent(void const *a);

void writeStudent(void const *a, FILE *f);

int cmpStudents(void const *a, void const *b);

bool studentFromStr(void *a, char const *str);

#endif // STUDENT_H_INCLUDED