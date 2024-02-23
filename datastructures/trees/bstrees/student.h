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

bool validStudent(student *s); 

void printStudent(void *a);

void writeStudentToFile(void *a, FILE *f);

int cmpStudents(void *a, void *b);

int cmpStudentById(void *a, void *b);

void freeStudent(student *s);

bool studentFromStr(void *a, char *str);

#endif // STUDENT_H_INCLUDED