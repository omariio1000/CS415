//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Matrix multiplication (shared-memory version)
//  - approach 1: with loops
//  - approach 2: with reduce
//

config const N = 8;
var D = {1..N, 1..N};
var a, b, c, d: [D] int;

// initialization
a = [(i,j) in D] i + j;
b = [(i,j) in D] j;
c = 0;
d = 0;

// approach 1
forall (i,j) in D do
  for k in 1..N do
    c(i,j) += a(i,k) * b(k,j);

// approach 2
d = [(i,j) in D] + reduce [k in 1..N] a(i,k) * b(k,j);

writeln("total = ", + reduce c);
writeln("total = ", + reduce d);

