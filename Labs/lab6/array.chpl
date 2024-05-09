//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Array operations
//
config const m = 4, n = 6;
var a: [1..m,1..n] int, inner: [1..m-2,1..n-2] int;
var row1, row2: [1..n] int, row3s: [1..n/2] int;
a = [(i,j) in {1..m,1..n}] j + (i-1)*n;
row1 = a[1,1..n];
row2 = a[2,..];
row3s = a[3,1..n by 2];
inner = a[2..m-1,2..n-1];

writeln("a:");
writeln(a);
writeln("row1  = ", row1);
writeln("row2  = ", row2);
writeln("row3s = ", row3s);
writeln("inner:");
writeln(inner);
