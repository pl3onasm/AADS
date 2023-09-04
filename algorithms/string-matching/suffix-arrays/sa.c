/* file: sa.c
   author: David De Potter
   email: pl3onasm@gmail.com
   license: MIT, see LICENSE file in repository root folder
   description: suffix arrays
   assumption: Σ is the extended ASCII alphabet (256 characters)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef enum {               
  false = 0,
  true = 1
} bool;

typedef struct {
  uint index;       // index of suffix in text
  uint leftRank;    // rank of left half of suffix
  uint rightRank;   // rank of right half of suffix
} suffix;

void *safeCalloc (int n, int size) {
  /* allocates memory and checks whether this was successful */
  void *ptr = calloc(n, size);
  if (ptr == NULL) {
    printf("Error: calloc(%d, %d) failed. Out of memory?\n", n, size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *safeRealloc (void *ptr, int newSize) {
  /* reallocates memory and checks whether the allocation was successful */
  ptr = realloc(ptr, newSize);
  if (ptr == NULL) {
    printf("Error: realloc(%d) failed. Out of memory?\n", newSize);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

uchar *readString(uint *size, short type) {
  /* reads a string from stdin and stores its length in size */
  uchar c; uint len = 0; 
  uchar tok = type ? '\n' : EOF;   // type = 1: read until newline
  uchar *str = safeCalloc(100, sizeof(uchar));
  while (scanf("%c", &c) == 1 && c != tok) {
    if (c == '\n') c = ' '; // replace newline with space
    str[len++] = c; 
    if (len % 100 == 0) str = safeRealloc(str, (len+100) * sizeof(uchar));
  }
  str[len] = '\0';
  *size = len;
  return str;
}

void radixSort(suffix *suffixes, uint n, uint k) {
  /* sorts suffixes by first using counting sort on the 
     right ranks and then on the left ranks */
  uint *count = safeCalloc(k, sizeof(uint));
  suffix *output = safeCalloc(n, sizeof(suffix));
  
  /* sort by right ranks */
  // count occurences of each rank
  for (uint i = 0; i < n; ++i) {
    count[suffixes[i].rightRank]++;
  }
  // compute cumulative counts
  for (uint i = 1; i < k; ++i) count[i] += count[i-1];
  // sort suffixes
  for (int i = n-1; i >= 0; --i) {
    output[count[suffixes[i].rightRank]-1] = suffixes[i];
    count[suffixes[i].rightRank]--;
  }
  
  /* sort by left ranks */
  // reset count
  memset(count, 0, k * sizeof(uint));
  // count occurences of each rank
  for (uint i = 0; i < n; ++i) count[output[i].leftRank]++;
  // compute cumulative counts
  for (uint i = 1; i < k; ++i) count[i] += count[i-1];
  // sort suffixes
  for (int i = n-1; i >= 0; --i) {
    suffixes[count[output[i].leftRank]-1] = output[i];
    count[output[i].leftRank]--;
  }
  free(count);
  free(output);
}

uint makeRanks (suffix *suffixes, uint *rank, uint n) {
  /* assigns ranks to suffixes, and returns the maximum rank */
  uint r = 1;
  rank[suffixes[0].index] = r;
  for (uint i = 1; i < n; i++) {
    if (suffixes[i-1].leftRank != suffixes[i].leftRank || 
        suffixes[i-1].rightRank != suffixes[i].rightRank) r++;
    rank[suffixes[i].index] = r;
  }
  return r;
}

uint *buildSuffixArray(uchar *text, uint n) {
  /* builds a suffix array for text of length n */

  // allocate memory for suffix array and suffixes
  uint *sa = safeCalloc(n, sizeof(uint));
  suffix *suffixes = safeCalloc(n, sizeof(suffix));
  uint *rank = safeCalloc(n, sizeof(uint));

  // initialize suffixes
  for (uint i = 0; i < n; i++) {
    suffixes[i].index = i;
    suffixes[i].leftRank = text[i];
    suffixes[i].rightRank = (i+1 < n) ? text[i+1] : 0;
  }
  // sort suffixes according to their left and right ranks
  radixSort(suffixes, n, 256);
  // length of sorted suffixes
  uint l = 2; 

  // repeat until all suffixes are sorted
  while (l < n) {
    // assign ranks to suffixes and return maximum rank
    uint r = makeRanks(suffixes, rank, n);

    // update left and right ranks
    for (uint i = 0; i < n; i++) {
      suffixes[i].leftRank = rank[i];
      suffixes[i].rightRank = (i + l < n) ? rank[i+l] : 0;	
      suffixes[i].index = i;
    }
    // sort suffixes according to new ranks
    radixSort(suffixes, n, r+1);
    // double length of sorted suffixes
    l *= 2; 
  }
  // store suffix array
  for (uint i = 0; i < n; i++) sa[i] = suffixes[i].index;
  free(suffixes);
  free(rank);
  return sa;
}

uint *buildLCPArray(uchar *text, uint *sa, uint n) {
  /* builds a longest common prefix array for text of length n
     and suffix array sa */
  uint *lcp = safeCalloc(n, sizeof(uint));
  uint *rank = safeCalloc(n, sizeof(uint));

  // compute rank of each suffix; this is the inverse of sa
  // rank[i] = sorted index of suffix i, i.e. position of suffix i in sa
  for (uint i = 0; i < n; i++) rank[sa[i]] = i; 

  // compute lcp of each suffix
  lcp[0] = 0;  // lcp of first suffix is 0 (no preceding suffix)
  uint l = 0;  // length of longest common prefix
  for (uint i = 0; i < n; i++) { // goes over suffixes in text order
    if (rank[i] == 0) continue;  // first suffix has no preceding suffix
    uint j = sa[rank[i]-1];      // text index of the lexicographically preceding suffix
    while (i+l < n && j+l < n && text[i+l] == text[j+l]) l++;  
    lcp[rank[i]] = l; 
    if (l > 0) l--;   // lcp of next suffix is at most one shorter than current lcp
  }
  free(rank);
  return lcp;
}

void matcher(uchar *pattern, uchar *text, uint *sa, uint *lcp, uint n, uint m) {
  /* finds all occurences of pattern in text in O(mlogn) time, using binary search */
  uint l = 0, r = n-1, i, j, mid; 
  bool found = false;

  // find occurence of pattern using binary search
  while (l <= r) {
    mid = l + (r-l)/2;  
    i = sa[mid];        // index of suffix 
    j = 0;              // index of pattern
    // compare pattern and suffix at middle index
    while (i+j < n && j < m && text[i+j] == pattern[j]) j++; 
    if (j == m) {
      found = true;
      break;
    }
    // pattern lexicographically larger ?
    if (j < m && text[i+j] < pattern[j]) l = mid+1; 
    // pattern is lexicographically smaller
    else r = mid-1; 
  }
  if (!found) {
    printf("No matches found.\n");
    return;
  }
  // find all other occurences of pattern using middle index and LCP array
  uint start = mid, end = mid;
  // find start of occurences
  while (start > 0 && lcp[start] >= m) start--;
  // find end of occurences
  while (end < n-1 && lcp[end+1] >= m) end++;
  // print valid shifts
  printf("Pattern: %s\n", pattern);
  printf("Shifts (%d): ", end-start+1);
  for (uint i = start; i <= end; i++) {
    printf("%d", sa[i]);
    if (i < end) printf(", ");
  }
  printf("\n");
}

void getLrs(uchar *text, uint *sa, uint *lcp, uint n) {
  /* finds all longest repeated substrings in text in O(n) time */

  // find length of longest repeated substring
  uint max = 0, k = 0, r = 0;
  for (uint i = 1; i < n; i++) 
    if (lcp[i] > max)  max = lcp[i];
  
  // find all longest repeated substrings
  if (max == 0) {
    printf("No repeated substrings found.\n");
    return;
  }
  printf("Longest repeated substrings:\n");
  while (k < n-1) {
    if (lcp[k] == max) {
      r = 0;
      while (k+r < n-1 && lcp[k+r] == max) r++;
      if (r) printf("  %d times: ", r+1);
      else printf("  1 time: ");
      for (uint i = 0; i < max; i++) printf("%c", text[sa[k]+i]);
      printf("\n");
      k += r;
    }
    k++;
  }
}

void getNoSubs(uchar *text, uint *sa, uint *lcp, uint n) {
  /* computes the total number of substrings in text 
     without counting duplicates in O(n) time */
  uint lcpSum = 0;
  for (uint i = 1; i < n; ++i)
    lcpSum += lcp[i];
  printf("Number of non-duplicate substrings: %d\n", n*(n+1)/2 - lcpSum);
}

void getLps(uchar *text, uint n) {
  /* computes the longest palindromic substring in text in O(nlogn) time */
  uint *lps = safeCalloc(n, sizeof(uint));
  // concatenate text and its reverse
  uchar *str = safeCalloc(2*n, sizeof(uchar));
  for (uint i = 0; i < n; ++i) str[i] = tolower(text[i]);
  for (uint i = 0; i < n; ++i) str[n+i] = tolower(text[n-i-1]);
  // build suffix array and LCP array
  uint *sa = buildSuffixArray(str, 2*n);
  uint *lcp = buildLCPArray(str, sa, 2*n);

  // find longest palindromic substring
  uint max = 0, k = 0;
  for (uint i = 1; i < 2*n; ++i) {
    if (lcp[i] > max && sa[i] == 2*n - sa[i-1] - lcp[i]) {
      max = lcp[i];
      k = sa[i];
    }
  }
  printf("Longest palindromic substring: ");
  for (uint i = 0; i < max; ++i) printf("%c", str[k+i]);
  printf("\n");
  free(lps); free(str);
  free(sa); free(lcp);
}

int main() {
  uint n, m; // lengths of text and pattern
  uchar *pattern = readString(&m, 1); 
  uchar *text = readString(&n, 0); 

  // build suffix array and LCP array
  uint *sa = buildSuffixArray(text, n); 
  uint *lcp = buildLCPArray(text, sa, n); 

  // find all occurences of pattern in text
  matcher(pattern, text, sa, lcp, n, m);

  // find longest repeated substring
  getLrs(text, sa, lcp, n);

  // compute number of substrings in text
  getNoSubs(text, sa, lcp, n);

  // compute longest palindromic substring
  getLps(text, n);
  
  free(pattern);
  free(text);
  free(sa);
  free(lcp);
  return 0;
}