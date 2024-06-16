/* 
  file: sa.c
  author: David De Potter
  email: pl3onasm@gmail.com
  license: MIT, see LICENSE file in repository root folder
  description: suffix arrays, longest common prefixes, string
    matching, longest repeated substrings, number of unique
    substrings, longest palindromic substring
  assumption: Σ is the extended ASCII alphabet (256 characters)
*/

#include "../../../lib/clib/clib.h"

typedef struct {
  size_t index;       // index of suffix in text
  size_t leftRank;    // rank of left half of suffix
  size_t rightRank;   // rank of right half of suffix
} suffix;

//===================================================================
// Sorts suffixes by their ranks by using counting sort twice: 
// first on the right ranks and then on the left ranks
void radixSort(suffix *suffixes, size_t n, size_t maxRank) {
  
  size_t *count = safeCalloc(maxRank, sizeof(size_t));
  suffix *output = safeCalloc(n, sizeof(suffix));

    // SORT BY RIGHT RANKS
    // count occurences of each rank
  for (size_t i = 0; i < n; ++i) 
    count[suffixes[i].rightRank]++;
    // compute cumulative counts
  for (size_t i = 1; i < maxRank; ++i) 
    count[i] += count[i - 1];
    // sort suffixes
  for (size_t i = n; i--; ) {
    output[count[suffixes[i].rightRank] - 1] = suffixes[i];
    count[suffixes[i].rightRank]--;
  }
  
    // SORT BY LEFT RANKS
    // reset count
  memset(count, 0, maxRank * sizeof(size_t));
    // count occurences of each rank
  for (size_t i = 0; i < n; ++i) 
    count[output[i].leftRank]++;
    // compute cumulative counts
  for (size_t i = 1; i < maxRank; ++i) 
    count[i] += count[i - 1];
    // sort suffixes
  for (size_t i = n; i--; ) {
    suffixes[count[output[i].leftRank] - 1] = output[i];
    count[output[i].leftRank]--;
  }
  free(count);
  free(output);
}

//===================================================================
// Assigns ranks to suffixes and returns the maximum rank
size_t makeRanks (suffix *suffixes, size_t *ranks, size_t n) {
  size_t rank = 1;
  ranks[suffixes[0].index] = rank;
  for (size_t i = 1; i < n; i++) {
    if (suffixes[i - 1].leftRank != suffixes[i].leftRank  
        || suffixes[i - 1].rightRank != suffixes[i].rightRank) 
      rank++;
    ranks[suffixes[i].index] = rank;
  }
  return rank;
}

//===================================================================
// Builds a suffix array for text of length n in O(nlogn) time
size_t *buildSuffixArray(string *text) {

  size_t n = strLen(text);
  size_t *sa = safeCalloc(n, sizeof(size_t));
  suffix *suffixes = safeCalloc(n, sizeof(suffix));
  size_t *ranks = safeCalloc(n, sizeof(size_t));

    // initialize suffixes
  for (size_t i = 0; i < n; i++) {
    suffixes[i].index = i;
    suffixes[i].leftRank = charAt(text, i);
    suffixes[i].rightRank = (i + 1 < n) ? charAt(text, i + 1) : 0;
  }

    // sort suffixes by their ranks
  radixSort(suffixes, n, 256);
    // length of sorted suffixes
  size_t len = 2; 
 
    // repeat until all suffixes are sorted
  while (len < n) {
      // assign ranks to suffixes and return maximum rank
    size_t maxRank = makeRanks(suffixes, ranks, n);

      // update left and right ranks
    for (size_t i = 0; i < n; i++) {
      suffixes[i].leftRank = ranks[i];
      suffixes[i].rightRank = (i + len < n) ? ranks[i + len] : 0;	
      suffixes[i].index = i;
    }
      // sort suffixes according to new substring ranks
    radixSort(suffixes, n, maxRank + 1);
      // the sorted suffixes are twice as long
    len *= 2;
  }
    // store the final indices of the sorted suffixes
  for (size_t i = 0; i < n; i++) 
    sa[i] = suffixes[i].index;
  free(suffixes);
  free(ranks); 
  return sa;
}

//===================================================================
// Builds a longest common prefix array for given text and its 
// suffix array in O(n) time
size_t *buildLCPArray(string *text, size_t *sa) {
  
  size_t n = strLen(text);
  size_t *lcp = safeCalloc(n, sizeof(size_t));
  size_t *ranks = safeCalloc(n, sizeof(size_t));

    // compute suffix ranks; this is the inverse of sa
    // ranks[i] = sorted index of suffix at text index i
  for (size_t i = 0; i < n; i++) 
    ranks[sa[i]] = i; 

    // lcp of first suffix is 0 
  lcp[0] = 0;         
    // length of longest common prefix
  size_t len = 0;     
    // iterate over all suffixes in text order
  for (size_t i = 0; i < n; i++) { 
    if (ranks[i] == 0) 
      continue;       
      // get text index j of the lexicographically preceding suffix
    size_t j = sa[ranks[i] - 1];      
      // compute lcp of suffixes at text indices i and j
    while (i + len < n && j + len < n 
           && charAt(text, i + len) == charAt(text, j + len)) 
      len++;  
      // store lcp of suffix at sorted index ranks[i]
    lcp[ranks[i]] = len; 
      // lcp of next suffix is at least one less 
      // than current lcp if len > 0, otherwise it is 0
    if (len > 0)
      len--;  
  }
  free(ranks);
  return lcp;
}

//===================================================================
// Finds all occurences of pattern in text using binary search
// Time complexity: O(mlogn) 
void matcher(string *pattern, string *text, 
             size_t *sa, size_t *lcp) {
  
  size_t m = strLen(pattern), n = strLen(text);
  size_t left = 0, right = n - 1, i, j, mid; 
  bool foundShift = false;

    // find occurence of pattern using binary search
  while (left <= right) {
    mid = left + (right - left)/2;  
    i = sa[mid];        // index of suffix 
    j = 0;              // index of pattern
      // compare pattern and suffix at middle index
    while (i + j < n && j < m 
           && charAt(text, i + j) == charAt(pattern, j))
      j++;
      // pattern found?
    if (j == m) {
      foundShift = true;
      break;
    }
      // is pattern lexicographically larger ?
    if (j < m && charAt(text, i + j) < charAt(pattern, j)) 
      left = mid + 1; 
      // pattern is lexicographically smaller
    else right = mid - 1; 
  }
  if (! foundShift) {
    printf("No matches found.\n");
    return;
  }
 
    // find all other occurences of pattern in text
  size_t start = mid, end = mid;
    // find start of occurences
  while (start > 0 && lcp[start] >= m) start--;
    // find end of occurences
  while (end < n - 1 && lcp[end + 1] >= m) end++;
    // print all valid shifts
  printf("Pattern: ");
  showString(pattern);
  printf("Shifts (%zu): ", end - start + 1);
  for (size_t i = start; i <= end; i++) 
    printf(i < end ? "%zu, " : "%zu\n", sa[i]);
}

//===================================================================
// Finds all longest repeated substrings in text in O(n) time
void getLrs(string *text, size_t *sa, size_t *lcp) {

    // find length of longest repeated substring
  size_t n = strLen(text), max = 0;
  for (size_t i = 1; i < n; i++) 
    if (lcp[i] > max) max = lcp[i];
  
  if (max == 0 || max == 1) {
    printf("No repeated substrings found.\n");
    return;
  }

    // find all longest repeated substrings
  size_t idx = 0, times;
  printf("Longest repeated substrings:\n");
  while (idx < n) {
    if (lcp[idx] == max) {
      times = 2;
      while (idx + 1 < n && lcp[idx + 1] == max) { 
        times++;
        idx++;
      }
      printf("  %zu times: ", times);
      showSubString(text, sa[idx], sa[idx] + max);
    }
    idx++;
  }
}

//===================================================================
// Computes the total number of substrings in text without counting
// duplicates in O(n) time
void getNrSubs(string *text, size_t *sa, size_t  *lcp) {
   
  size_t lcpSum = 0, n = strLen(text);
  for (size_t i = 1; i < n; ++i)
    lcpSum += lcp[i];
  printf("Number of unique substrings: %zu\n", 
          n * (n + 1) / 2 - lcpSum);
}

//===================================================================
// Computes the longest palindromic substring that occurs in text in  
// O(nlogn) time
void getLps(string *text) {
  
  size_t n = strLen(text);
  size_t *lps = safeCalloc(n, sizeof(size_t));
    
    // concatenate text and its reverse
  string *str = copyString(text);
  string *rev = copyString(text);
  concatStrings(str, reverseString(rev));

    // build suffix array and LCP array for concatenated string
  size_t *sa = buildSuffixArray(str);
  size_t *lcp = buildLCPArray(str, sa);

    // find longest palindromic substring
  size_t max = 0, k = 0;
  for (size_t i = 1; i < 2*n; ++i) 
    if (lcp[i] > max && sa[i] == 2 * n - sa[i - 1] - lcp[i]) {
      max = lcp[i];
      k = sa[i];
    }

  printf("Longest palindromic substring: ");
  showSubString(str, k, k + max);
  free(lps); freeString(str); freeString(rev);
  free(sa); free(lcp);
}

//===================================================================

int main() {
  
  READ_STRING(pattern, '\n');
  READ_STRING(text, EOF);

    // build suffix array and LCP array
  size_t *sa = buildSuffixArray(text); 
  size_t *lcp = buildLCPArray(text, sa); 

    // find all occurences of pattern in text
  matcher(pattern, text, sa, lcp);

    // compute longest repeated substring
  getLrs(text, sa, lcp);

    // compute number of unique substrings
  getNrSubs(text, sa, lcp);

    // find longest palindromic substring
  getLps(text);
  
  free(sa);
  free(lcp);
  freeString(pattern);
  freeString(text);
  return 0;
}