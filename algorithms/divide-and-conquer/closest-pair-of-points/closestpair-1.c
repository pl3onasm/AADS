/* file: closestpair-1.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: closest pair of points using divide and conquer
   time complexity: O(nlog²n), since we only presort on 
     x-coordinates and sort the strip on y-coordinates for each 
     recursive call.
     We find T(n) = 2T(n/2) + Θ(nlogn) = Θ(nlog²n) by case 2 
     of the master theorem.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct point {
  double x;
  double y;
} point;

typedef struct pair {
  double dist;
  point p1;
  point p2;
} pair;

void *safeMalloc (int n) {
  /* allocates n bytes of memory and checks whether the allocation
     was successful */
  void *ptr = malloc(n);
  if (ptr == NULL) {
    printf("Error: malloc(%d) failed. Out of memory?\n", n);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

// compare two points by x-coordinate
int xcompare(const void *a, const void *b) {
    point *p1 = (point*) a;
    point *p2 = (point*) b;
    return p1->x - p2->x;
}

// compare two points by y-coordinate
int ycompare(const void *a, const void *b) {
    point *p1 = (point*) a;
    point *p2 = (point*) b;
    return p1->y - p2->y;
}

// returns euclidean distance between two points
double distance(point p1, point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

pair findClosestPairInStrip (point *points, int n, double min) {
  /* find closest pair in the strip between the two halves
     of the points array */
  point *strip = safeMalloc (n * sizeof(point));
  int len = 0; pair p = {DBL_MAX, {0,0}, {0,0}}; 
  
  for (int i = 0; i < n; ++i) {
    if (abs(points[i].x - points[n/2].x) < min) {
      strip[len++] = points[i];
    }
  }
  qsort (strip, len, sizeof(point), ycompare);
  for (int i = 0; i < len; ++i) {
    for (int j = 1; j < 8 && i + j < len; ++j) {
      double d = distance (strip[i], strip[i+j]);
      if (d < min) {
        min = d;
        p.p1 = strip[i];
        p.p2 = strip[i+j];
        p.dist = d;
      }
    }
  }
  free (strip); 
  return p;
}

pair findClosestPair(point *points, int n) {
  pair pair1, pair2; double d1, d2, d3; 

  // base cases: 2 or 3 points
  if (n == 2) {
    pair1.p1 = points[0];
    pair1.p2 = points[1];
    pair1.dist = distance(points[0], points[1]);
    return pair1;
  }
  if (n == 3) {
    d1 = distance(points[0], points[1]);
    d2 = distance(points[0], points[2]);
    d3 = distance(points[1], points[2]);
    pair1.p1 = points[0];
    pair1.p2 = points[1];
    pair1.dist = d1;
    if (d2 < d1 && d2 < d3) {
      pair1.p1 = points[0];
      pair1.p2 = points[2];
      pair1.dist = d2;
    }
    if (d3 < d1 && d3 < d2) {
      pair1.p1 = points[1];
      pair1.p2 = points[2];
      pair1.dist = d3;
    }
    return pair1;
  }

  // divide in two halves
  int mid = n / 2;
  // find closest pair in left and right half recursively
  pair1 = findClosestPair(points, mid);           
  pair2 = findClosestPair(points + mid, n - mid);
  
  // get minimum distance of the two halves
  double min = MIN(pair1.dist, pair2.dist);
  // find closest pair in the strip between the two halves 
  pair pair3 = findClosestPairInStrip(points, n, min);

  // combine to get the closest pair
  d1 = pair1.dist; d2 = pair2.dist; d3 = pair3.dist; 
  if (d1 <= d2 && d1 <= d3) return pair1;
  if (d2 <= d1 && d2 <= d3) return pair2;
  return pair3;
}

int main(int argc, char *argv[]) {
  int n; 
  scanf("%d\n", &n);
  point *points = safeMalloc(n * sizeof(point));

  for (int i = 0; i < n; i++) 
    scanf("(%lf,%lf),", &points[i].x, &points[i].y);

  // sort points on x-coordinate
  qsort(points, n, sizeof(point), xcompare);

  pair pair = findClosestPair(points, n);

  printf("The closest distance is %lf between (%lf,%lf)" 
         " and (%lf,%lf).\n", pair.dist, pair.p1.x, 
          pair.p1.y, pair.p2.x, pair.p2.y);

  free(points); 

  return 0;
}