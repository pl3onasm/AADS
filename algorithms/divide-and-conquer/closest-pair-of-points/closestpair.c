/* file: closestpair.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: closest pair of points using divide and conquer
   time complexity: O(nlogn), which is achieved by presorting
     not only on x-coordinates but also on y-coordinates. 
     We find T(n) = 2T(n/2) + Θ(n) = Θ(nlogn) by case 2 of 
     the master theorem.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//:::::::::::::::::::::::: data structures ::::::::::::::::::::::::://

typedef struct point {
  double x;             // x-coordinate of point
  double y;             // y-coordinate of point
} point;

typedef struct pair {
  point p1;             // first point of the pair
  point p2;             // second point of the pair
  double dist;          // distance between the two points
} pair;

//::::::::::::::::::::::: memory management :::::::::::::::::::::::://

void *safeCalloc (int n, int size) {
  /* allocates n elements of size size, initializing them to 0, and
     checks whether the allocation was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

//:::::::::::::::::::::::: helper functions :::::::::::::::::::::::://

int xcompare(const void *a, const void *b) {
  /* compares two points by x-coordinate */
  point *p1 = (point*) a;
  point *p2 = (point*) b;
  return p1->x - p2->x;
}

int ycompare(const void *a, const void *b) {
  /* compares two points by y-coordinate */
  point *p1 = (point*) a;
  point *p2 = (point*) b;
  return p1->y - p2->y;
}

double distance(point p1, point p2) {
  /* returns euclidean distance between two points */
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void setPair (pair *p, point p1, point p2, double dist) {
  /* sets pair p to points p1 and p2 with distance dist */
  p->p1 = p1;
  p->p2 = p2;
  p->dist = dist;
}

//::::::::::::::::::::: closest pair algorithm ::::::::::::::::::::://

pair findClosestPairInStrip (point *ypoints, int ysize, 
  double median, double delta) {
  /* finds closest pair in strip of width 2*delta around median */
  point *strip = safeCalloc (ysize, sizeof(point));
  int len = 0; pair p = {{0,0}, {0,0}, DBL_MAX};

  // make strip of points within 2*delta of median
  for (int i = 0; i < ysize; ++i) 
    if (abs(ypoints[i].x - median) < delta) 
      strip[len++] = ypoints[i];
  
  // pass through strip in groups of 7 points at a time
  // and keep track of the new closest pair (if any)
  for (int i = 0; i < len; ++i)
    // check next 7 points
    for (int j = 1; j < 8 && i + j < len; ++j) {  
      double d = distance (strip[i], strip[i+j]);
      if (d < delta) {  // new closest pair found?
        delta = d;
        setPair (&p, strip[i], strip[i+j], d);
      }
    }

  free (strip);
  return p;
}

pair findClosestPair(point *xpoints, point *ypoints, int ysize, int n) {
  pair minpair, pair1, pair2, pair3; double d1, d2, d3; 

  // BASE CASES: brute force for n <= 3
  if (n <= 3) {
    d1 = distance(xpoints[0], xpoints[1]);
    setPair (&minpair, xpoints[0], xpoints[1], d1);
    if (n == 2) return minpair;   
    d2 = distance(xpoints[0], xpoints[2]);
    d3 = distance(xpoints[1], xpoints[2]);
    if (d2 < d1 && d2 < d3) 
      setPair (&minpair, xpoints[0], xpoints[2], d2);
    if (d3 < d1 && d3 < d2) 
      setPair (&minpair, xpoints[1], xpoints[2], d3);
    return minpair;
  }

  // DIVIDE: split ypoints into left and right half based on
  //         whether they are left or right of the median
  point *yrpoints = safeCalloc(ysize, sizeof(point));
  point *ylpoints = safeCalloc(ysize, sizeof(point));
  int yl = 0, yr = 0, mid = n / 2;
  int median = xpoints[mid].x;
  for (int i = 0; i < ysize; ++i) {
    if (ypoints[i].x < median) 
      ylpoints[yl++] = ypoints[i];
    else 
      yrpoints[yr++] = ypoints[i];
  }

  // CONQUER: find closest pair in each half
  pair1 = findClosestPair(xpoints, ylpoints, yl, mid);         
  pair2 = findClosestPair(xpoints + mid, yrpoints, yr, n - mid); 
  free (yrpoints); free (ylpoints);
  
  // COMBINE: find closest pair in strip of width 2*delta around median
  //          and return the closest of the three pairs
  double delta = MIN(pair1.dist, pair2.dist); 
  pair3 = findClosestPairInStrip(ypoints, ysize, median, delta);

  d1 = pair1.dist; d2 = pair2.dist; d3 = pair3.dist; 
  if (d1 <= d2 && d1 <= d3) return pair1;
  if (d2 <= d1 && d2 <= d3) return pair2;
  return pair3;
}

//::::::::::::::::::::::::: main function :::::::::::::::::::::::::://

int main(int argc, char *argv[]) {
  int n; 
  scanf("%d\n", &n);
  point *xpoints = safeCalloc(n, sizeof(point));
  point *ypoints = safeCalloc(n, sizeof(point));

  // read points from stdin
  for (int i = 0; i < n; i++) {
    scanf("(%lf,%lf),", &xpoints[i].x, &xpoints[i].y);
    ypoints[i] = xpoints[i];
  } 
  
  // presort points once by x and once by y coordinates
  qsort(xpoints, n, sizeof(point), xcompare);
  qsort(ypoints, n, sizeof(point), ycompare);

  // find closest pair of points
  pair pair = findClosestPair(xpoints, ypoints, n, n);

  // print result
  printf("The closest distance is %lf between (%lf,%lf)" 
         " and (%lf,%lf).\n", pair.dist, pair.p1.x, 
          pair.p1.y, pair.p2.x, pair.p2.y);

  free(xpoints); 
  free(ypoints);
  return 0;
}