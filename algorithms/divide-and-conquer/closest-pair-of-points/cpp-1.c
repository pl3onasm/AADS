/* 
  file: cpp-1.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: closest pair of points using brute force
  time complexity: O(n^2)
*/ 

#include <math.h>
#include <float.h>
#include "../../../lib/clib.h"

//===================================================================
// Defines a point and a pair of points

typedef struct point {
  double x;             // x-coordinate of point
  double y;             // y-coordinate of point
} point;

typedef struct pair {
  point p1;             // first point of the pair
  point p2;             // second point of the pair
  double dist;          // distance between the two points
} pair;

//===================================================================
// Returns the Euclidean distance between two points p1 and p2
double distance(point p1, point p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//===================================================================
// Finds the closest pair of points in an array of points
pair findClosestPair(point *points, size_t size) {
  pair closestPair;
  closestPair.dist = DBL_MAX;

  for (size_t i = 0; i < size; i++) {
    for (size_t j = i + 1; j < size; j++) {
      double dist = distance(points[i], points[j]);
      if (dist < closestPair.dist) {
        closestPair.dist = dist;
        closestPair.p1 = points[i];
        closestPair.p2 = points[j];
      }
    }
  }

  return closestPair;
}

//===================================================================

int main() {

  size_t n; 
  assert(scanf("%zu\n", &n) == 1);

  point *points = safeCalloc(n, sizeof(point));

    // read points from stdin
  for (int i = 0; i < n; i++) 
    assert(scanf("(%lf,%lf),", &points[i].x, &points[i].y) == 2);

    // find closest pair of points
  pair pair = findClosestPair(points, n);

    // print result
  printf("Closest distance: %lf\n"
         "Found between (%lf,%lf) and (%lf,%lf)\n", 
          pair.dist, pair.p1.x, pair.p1.y,
          pair.p2.x, pair.p2.y);

  free(points); 
  return 0;
}