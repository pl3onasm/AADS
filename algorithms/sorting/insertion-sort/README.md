$\huge{\color{cadetblue} \text{Insertion sort}}$

<br/>

Insertion sort is an ${\color{darkseagreen} \text{in-place}}$ comparison sort which sorts an array $A[0:n-1]$ in ascending order in a similar manner to how we sort a hand of cards. We start with an empty left hand and the cards face down on the table. We then remove one card at a time from the pile on the table and insert it into the correct position in the left hand. To find the correct position for a card, we compare it with each of the cards in the left hand, from right to left, until we find a card that is smaller than it, or until we reach the left end of the hand. At that point, the card we are trying to insert is placed into the position immediately to the right of the card that is smaller than it, or at the left end of the hand if all cards in the hand are larger than it.

Similarly to the method of sorting a hand of cards, insertion sort maintains two subarrays: a left subarray of the elements that have already been sorted (the cards in the left hand) and a right subarray of the elements that have not yet been sorted (the cards on the table). At each iteration, the boundary between the two subarrays is moved one element to the right, and the next element from the right subarray is inserted into its correct position in the left subarray. This process is repeated until the entire array is sorted.  

If we were to implement insertion sort recursively, we would have a base case of an array of size 1, which is trivially sorted. The recursive case would be to sort the left subarray and then insert the last element of the right subarray into the correct position in the left subarray, resulting in a recurrence relation of the form ${\color{rosybrown}T(n) = T(n-1) + \Theta(n)}$, which has a solution of ${\color{rosybrown}\mathcal{O}(n^2)}$. The worst-case running time of $\mathcal{O}(n^2)$ occurs when the array is sorted in reverse order. The best-case running time, however, is $\mathcal{O}(n)$, which occurs when the array is already sorted.

Like [bubble sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/bubble-sort/), insertion sort is a ${\color{darkseagreen} \text{stable}}$ sort, since it inserts elements that are equal to each other in the same order as they appear in the input array. Note that this is true because we use the strict greater-than operator in the inner loop. If we had used the greater-than-or-equal-to operator, insertion sort would have been unstable.

Implementation: [Insertion sort](insertionsort.c)
