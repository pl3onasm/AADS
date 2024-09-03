$\huge{\color{cadetblue} \text{Radix sort}}$

<br/>

Radix sort is a ${\color{darkseagreen} \text{linear-time}}$ sorting algorithm that exploits the fact that the input consists of a sequence of $n$ d-digit numbers, where each digit can take on up to k possible values. For example, we might sort 20-digit decimal numbers, where each digit is in the range 0 through 9 ($d=20$, $k = 10$), or 5-digit binary numbers, where each digit is either 0 or 1 ($d=5$, $k = 2$).

In radix sort, we sort the input numbers digit by digit, starting with the least significant digit and working up to the most significant digit. For example, if we were to sort a list of 3-digit positive integers using radix sort ($d=3$, $k=10$), we would begin by sorting the list by the one's digit, then by the ten's digit, and finally by the hundred's digit to get the sorted result.

Each time we sort by a digit, it is important that we use a ${\color{darkseagreen} \text{stable}}$ sorting algorithm, such as [counting sort](https://github.com/pl3onasm/CLRS/tree/main/algorithms/sorting/counting-sort), so that the result of sorting on previous digits is preserved when the sort on the current digit is performed. Otherwise, if for example, we sorted the array $[23,21]$ on the one's digit to get $[21,23]$, and then sort it again on the ten's digit, we might get $[23,21]$ again, since the ten's digit of both numbers is 2, thus undoing the work we have already done. If the stable sort we use takes $\Theta(n+k)$ time, then we can sort $n$ d-digit base-k numbers in ${\color{rosybrown}\Theta(d(n+k))}$ time.

Note that the digits can also act as keys pointing to ${\color{darkseagreen} \text{fields}}$ in the input data that we want to sort, so that we can sort the data based on the significance of the fields in the data. This is useful when we want to sort data that is not just a sequence of numbers, but a sequence of records, where each record has multiple fields whose significance varies from field to field. For example, we might want to sort a list of dates, where each date is a record with three fields: day, month, and year. Each of these fields has a different significance: the day is the least significant field, the month is the next most significant field, and the year is the most significant field. By giving each field a ${\color{darkseagreen} \text{rank}}$ corresponding to its order of significance, we can sort the dates using radix sort.

The implementation uses this example of sorting a list of dates in different formats. Each format is given as a string of the form "DMY" (day, month, year) at the top of the input file, followed by a list of dates in that format. The dates are first converted to integer arrays of the form [day, month, year] and depending on the format, each cell of the array is given a rank in the order of significance of the fields in the data, so that the dates can be sorted using radix sort. The implementation uses counting sort as the stable sort to sort the dates by each field.

Implementation: [Radix Sort](https://github.com/pl3onasm/CLRS/blob/main/algorithms/sorting/radix-sort/radixsort.c)

For another application of radix sort, see its use in the construction of [suffix arrays](https://github.com/pl3onasm/CLRS/blob/main/algorithms/string-matching/suffix-arrays/sa.c).
