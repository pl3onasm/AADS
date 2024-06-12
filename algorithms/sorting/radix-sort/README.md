# ${\color{cadetblue} \text{Radix sort}}$

Radix sort is a ${\color{darkseagreen} \text{linear-time}}$ sorting algorithm that exploits the fact that the input consists of a sequence of $n$ d-digit numbers, where each digit can take on up to k possible values. For example, we might sort 20-digit decimal numbers, where each digit is in the range 0 through 9 ($d=20$, $k = 10$), or 5-digit binary numbers, where each digit is either 0 or 1 ($d=5$, $k = 2$).

In radix sort, we sort the input numbers digit by digit, starting with the least significant digit and working up to the most significant digit. For example, if we were to sort a list of 3-digit positive integers using radix sort ($d=3$, $k=10$), we would begin by sorting the list by the one's digit, then by the ten's digit, and finally by the hundred's digit to get the sorted result.

Each time we sort by a digit, it is important that we use a ${\color{darkseagreen} \text{stable}}$ sorting algorithm, such as [counting sort](../counting-sort/), so that the result of sorting on previous digits is preserved when the sort on the current digit is performed. Otherwise, if for example, we sorted the array $[23,21]$ on the one's digit to get $[21,23]$, and then sort it again on the ten's digit, we might get $[23,21]$ again, since the ten's digit of both numbers is 2, thus undoing the work we have already done. If the stable sort we use takes $\Theta(n+k)$ time, then we can sort $n$ d-digit base-k numbers in ${\color{rosybrown}\Theta(d(n+k))}$ time.

The implementation uses the example of sorting a list of dates in international format (YYYY-MM-DD), which has the property that the integers for the year, month, and day are already placed in order of significance: the leftmost number of the year is the most significant, whereas the rightmost number of the day is the least significant. This means we can treat these dates as if they were 8-digit base-10 numbers, and sort them using radix sort. The implementation uses counting sort as the stable sort.

Implementation: [Radix Sort](https://github.com/pl3onasm/CLRS/blob/main/algorithms/sorting/radix-sort/radixsort.c)
