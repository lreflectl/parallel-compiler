# Expression Validator, Parallelizer and Mapper for Vector Computing System

<p>Software takes math expression as an input value. Then it checks for syntax errors such as 
unclosed brace and wrong variable names. Subsequently it builds graph of the expression`s
calculation sequence. Sequence of operations which can be calculated concurrently is put on 
the same layer of the graph. In the process of graph generation expression is being optimised
for better parallelism. The final step is loading the graph in Vector System <a href="https://en.wikipedia.org/wiki/Vector_processor#Difference_between_SIMD_and_vector_processor">(wiki link)</a> and 
calculating performance parameters of executing the expression against single core execution.</p>

<h2>Features:</h2>
<ul>
<li>Syntax checks</li>
<li>Build execution priority graph</li>
<li>Trailing division operations optimization ( e.g. a/b/c/d = a/(b*c*d) )</li>
<li>Substraction optimization ( e.g. -a-b-c = -(a+b+c) )</li>
<li>Commutation of free additions (e.g. a+b*c+d = b*c+a+d)</li>
<li>Calculation of execution time on Vector System</li>
<li>Acceleration and efficiency factors</li>
<li><strong>Expression execution visualization on the Vector System with custom parameters</strong></li>
</ul>

<h2>Screenshots:</h2>
<a data-flickr-embed="true" href="https://www.flickr.com/photos/197052252@N02/52543069158/in/dateposted-public/" title="screenshot-1"><img src="https://live.staticflickr.com/65535/52543069158_5df1f0fa82_c.jpg" width="800" height="465" alt="screenshot-1"></a>

<a data-flickr-embed="true" href="https://www.flickr.com/photos/197052252@N02/52542817494/in/dateposted-public/" title="screenshot-2"><img src="https://live.staticflickr.com/65535/52542817494_e19cf3441c_c.jpg" width="800" height="489" alt="screenshot-2"></a>

<a data-flickr-embed="true" href="https://www.flickr.com/photos/197052252@N02/52542062182/in/dateposted-public/" title="screenshot-3"><img src="https://live.staticflickr.com/65535/52542062182_f7fb7f2d37_c.jpg" width="800" height="478" alt="screenshot-3"></a>

<a data-flickr-embed="true" href="https://www.flickr.com/photos/197052252@N02/52542522711/in/dateposted-public/" title="screenshot-4"><img src="https://live.staticflickr.com/65535/52542522711_17a8554066_c.jpg" width="800" height="527" alt="screenshot-4"></a>

<h2>Tips:</h2>
Project works with msvc compiler from <a href="https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022">Microsoft Build Tools</a>.
