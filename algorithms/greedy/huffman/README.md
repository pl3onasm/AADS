# Huffman Codes

## Problem

Given is a text made up of characters taken from an alphabet $\Sigma$ of size $n$. This text needs to be encoded using a binary code, i.e. a mapping from the characters in $\Sigma$ to binary strings, known as codewords. These codewords are called prefix-free if no codeword is a prefix of another codeword, and their length is defined as the number of bits in their corresponding bit string representation. The overall aim then is to find the optimal prefix-free binary code that minimizes the total length of the encoded text, which is in fact the sum of the lengths of the codewords of its characters.

## Huffman's greedy algorithm

The key idea is that the most frequent characters in the text should be encoded with the shortest codewords. This is why the algorithm works with variable-length codewords. It starts by creating a leaf node for each character in the text. These nodes are then inserted in a min-priority queue, where the key of a node is given by its frequency. The algorithm then repeatedly extracts the two nodes with the lowest frequency from the queue, creates a new node with the two nodes as children, and inserts this new node into the queue. This process terminates when only one node remains in the queue, which is the root of the constructed full binary tree, known as the Huffman tree.  
To determine the codeword for each character, all we need to do now is traverse the tree from the root to the leaf node corresponding to the character, assigning a $0$ for each edge we encounter to a left child and a $1$ for each edge to a right child. The codeword for a character is then the entire sequence of $0$'s and $1$'s encountered on the path from the root to the leaf node.

At each step the algorithm makes the greedy choice of grabbing the two nodes with the lowest frequency, without considering the future consequences or depending on the optimal solutions to subproblems. The greedy choice, however, is not only locally optimal, but also globally optimal. This is because the algorithm always merges the two least frequent characters, which are exactly the characters that will be encoded with the longest codewords as they occur lowest in the tree that is being built from the bottom up. The most frequent characters, on the other hand, will be encoded with the shortest codewords as they occur highest in the tree. This is why the algorithm produces an optimal prefix-free binary code.

The running time of the algorithm is $O(n\log n)$, where $n = |\Sigma|$ is the size of the alphabet. This is because the algorithm performs $n-1$ iterations, in order to generate a binary tree with $n-1$ internal nodes. Each of these iterations takes $O(\log n)$ time as it involves two extract-min operations and one insert operation, all of which take $O(\log n)$ time for a min-priority queue of size $n$ implemented as a binary min-heap.

The program outputs the codewords and frequencies for each character in the text, as well as a representation of the constructed Huffman tree. The example from CLRS thus yields the below output.

```text
CODEWORD TABLE

char    freq      code
   a      45      0
   c      12      100
   b      13      101
   f       5      1100
   e       9      1101
   d      16      111

HUFFMAN TREE

root
|0 'a'
|1
-1|0
-1-0|0 'c'
-1-0|1 'b'
-1|1
-1-1|0
-1-1-0|0 'f'
-1-1-0|1 'e'
-1-1|1 'd'
```

Implementation: [Huffman](https://github.com/pl3onasm/AADS/tree/main/algorithms/greedy/huffman/huffman.c)

Compilation:

```bash
cc -O2 -std=c99 -pedantic -o a.out ../../../datastructures/heaps/binheaps/binheap.c huffman.c ../../../lib/clib/*.c -lm
```
