# Expression Validator Parallelizer and Mapper for Vector Computing System

<p>Software takes math expression as input value. Then it checks for syntax errors such as 
unclosed braces and wrong variable names. Subsequently it builds graph of the expression`s
calulation sequence. Sequence of operations which can be calculated concurrently is put on 
a same layer of the graph. In the process of graph generation expression is being optimised
for better parallelism. The final step is loading the graph in Vector System <a href="https://en.wikipedia.org/wiki/Vector_processor#Difference_between_SIMD_and_vector_processor">(wiki link)</a> and 
calculating performance parameters of executing the expression against single core execution</p>

Features:


Screenshots:
