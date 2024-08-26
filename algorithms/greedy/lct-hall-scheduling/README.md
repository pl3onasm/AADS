${\huge\color{Cadetblue}\text{Lecture Hall}}$  
${\huge\color{Cadetblue}\text{Scheduling (LHS)}}$

<br />

${\Large\color{rosybrown}\text{Problem}}$

Given is a set of activities $A = \lbrace a_1, a_2, \ldots, a_n \rbrace$, where each activity $a_i$ is defined by an half-open interval $[s_i, f_i)$, where $s_i$ is the start time and $f_i$ is the finish time. Our task is to schedule the activities in a minimum number of lecture halls, such that all activities are compatible. Two activities $a_i$ and $a_j$ are ${\color{peru}\text{compatible}}$ if the intervals $[s_i, f_i)$ and $[s_j, f_j)$ don't overlap, i.e. $s_j \geq f_i$ or $s_i \geq f_j$.

<br />

${\Large\color{rosybrown}\text{Example}}$

Consider the following set of activities taken from test case 1:

<br/>

<div align="center">

 ${\color{cornflowerblue}\text{Activity}}$      |  ${\color{cadetblue}\text{Start time}}$  |${\color{cadetblue}\text{Finish time}}$  |
:-----:|:-----:|:------:|
${\color{cornflowerblue}a_1}$     | 3     | 9      |
${\color{cornflowerblue}a_2}$     | 1     | 4      |
${\color{cornflowerblue}a_3}$     | 10    | 12     |
${\color{cornflowerblue}a_4}$     | 5     | 8      |
${\color{cornflowerblue}a_5}$     | 11    | 15     |
${\color{cornflowerblue}a_6}$     | 3     | 6      |
${\color{cornflowerblue}a_7}$     | 4     | 5      |
${\color{cornflowerblue}a_8}$     | 10    | 14     |
${\color{cornflowerblue}a_9}$     | 0     | 3      |

</div>

<br />

The task then is to partition this set into a minimum number of partitions of mutually compatible activities. This is in fact an instance of the ${\color{peru}\text{interval graph coloring problem}}$, a special case of the graph coloring problem, in which colors have to be assigned to intervals in such a way that two overlapping intervals are colored differently and a minimum number of colors is used. Taking the example above, we can represent the set of activities as an interval graph, where each activity is a vertex, and each edge represents a conflict between two activities: an overlap between their intervals which makes them mutually incompatible. As the figure below shows, the activities can thus be partitioned into three lecture halls, each represented by a different color.

<p align="center" width="85%">
<img src="images/scheduling.png"
     alt="obsts3"
     style="float: left; padding-bottom: 40px;" />
</p><br clear="left">

At first glance, the problem seems similar to the [activity-selection problem](https://github.com/pl3onasm/CLRS/tree/main/algorithms/greedy/activity-selection), and we might be tempted to use this approach repeatedly to tackle the current problem as well, by filling up a lecture hall with the maximum number of mutually compatible activities, and then moving on to the next lecture hall and repeating the process until all activities have been scheduled. However, this approach is not optimal, as can be seen when applied to the example above: it would result in a total number of four lecture halls, while the optimal solution is three.

<br/>

${\Large\color{darkseagreen}\text{Brute force}}$

A brute force approach to the problem is to simply count the number of conflicting activities at each point in time. To do this, we consider all points in time between the smallest start time and the largest finish time, and for each point, check the number of activities that conflict at that point. The maximum number of conflicting activities at any point in time is then the minimum number of lecture halls needed to schedule all activities. This approach has a time complexity of $\mathcal{O}((t_{\text{max}} - t_{\text{min}}) \cdot n)$, where $t_{\text{max}}$ and $t_{\text{min}}$ are the maximum and minimum times, respectively, and $n$ is the number of activities.

Implementation: [LHS - Brute force 1](https://github.com/pl3onasm/CLRS/blob/main/algorithms/greedy/lct-hall-scheduling/lhs-1.c)

An optimized version is to check for each activity $a_i$ how many other activities that start before or at the same time as $a_i$ finish after $a_i$ starts. The maximum number of such activities is then the minimum number of lecture halls needed. This approach has a time complexity of $\mathcal{O}(n^2)$.

Implementation: [LHS - Brute force 2](https://github.com/pl3onasm/CLRS/blob/main/algorithms/greedy/lct-hall-scheduling/lhs-2.c)

<br/>

${\Large\color{darkseagreen}\text{Greedy approach}}$

The problem can be solved using a greedy approach, by first splitting up the activities into a series of events, each event being either a start or a finish time of an activity. We then sort these events by time, and iterate over them, keeping track of the number of lecture halls needed at each point in time. Whenever we encounter a start time, we increment the number of lecture halls needed, and whenever we encounter a finish time, we decrement the number of lecture halls needed. The maximum number of lecture halls needed at any point in time is then the minimum number of lecture halls needed to schedule all activities. A stack can be used to keep track of the available lecture halls, so that we can check if there is an available lecture hall whenever we encounter a start event, and add a lecture hall back to the stack whenever we encounter a finish event. If at any point in time there is no available lecture hall, we simply add a new lecture hall to the stack. The time complexity of this approach is $\mathcal{O}(n \log n)$.

Implementation: [LHS - Greedy 1](https://github.com/pl3onasm/CLRS/blob/main/algorithms/greedy/lct-hall-scheduling/lhs-3.c)

An alternative approach is to use a binary min-heap to keep track of the lecture halls that are currently in use. This heap is ordered by the finish time of the last activity in each lecture hall, so that we can easily find the earliest available lecture hall. After sorting the activities by start time, we process them one by one, and for each activity, we then check if there is an available lecture hall. If there is, we assign the activity to that lecture hall, and update the min-heap with the new finish time. If there isn't, we add a new lecture hall to the heap. The time complexity of this approach is the same as the previous one, $\mathcal{O}(n \log n)$.
