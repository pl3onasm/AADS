# Student Database

Compilation (using binary search tree):

```shell
gcc -O2 -std=c99 -pedantic -o a.out ../bstrees/bst.c bststudents.c student/student.c ../../../lib/clib/*.c -lm
```

Compilation (using red-black tree):

```shell
gcc -O2 -std=c99 -pedantic -o a.out ../rbtrees/rbt.c rbtstudents.c student/student.c ../../../lib/clib/*.c -lm
```

Usage:

```shell
./a.out students.txt
```
