${\color{Cadetblue}\text{\huge Algorithms}}$

<br/>

An algorithm is a well-defined computational procedure that takes some value, or set of values, as *input* and produces some value, or set of values, as *output* in a finite number of steps. An algorithm is thus a sequence of computational steps that transform the input into the output.

An algorithm can also be seen as a tool for solving a well-specified *computational problem*. The statement of the problem specifies in general terms the desired input/output relationship and the algorithm describes a specific computational procedure for achieving that relationship.

The input to an algorithm is called an *instance* of the problem that the algorithm solves and the output obtained is called the *solution* to the problem instance.

An algorithm is said to be *correct* if, for every input instance, it halts and outputs the correct solution to the problem instance. An algorithm is said to be *incorrect* if it halts with an incorrect output or fails to halt at all for some input instance.

The efficiency of an algorithm is measured by the *running time* of the algorithm, which is the number of primitive operations or "steps" executed. This running time is affected by many factors, including the programming language, the compiler, the input, the hardware platform, and the quality of the implementation. For the study of algorithms, however, we want to abstract away all these factors and focus on the intrinsic efficiency of the algorithm.

Usually, we are interested in how the running time of an algorithm increases with the size of the input in the limit, as the size of the input increases without bound. This approach is called the *asymptotic analysis* of algorithms. We often focus on the *worst-case running time* of an algorithm, which gives an upper bound on the running time for any input of size $n$.
