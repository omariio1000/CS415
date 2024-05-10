config const n = 8; 

var A: [1..n] int = [i in 1..n] i;


var Odd = A[1..n by 2];
var Even = A[2..n by 2];
var Front = A[1..n/2];
var Back = A[n/2+1..n];
var Reverse: [1..n] int;
var Shuffle: [1..n] int;

for i in 1..n {
    Reverse[i] = A[n-i+1];
}

for i in 1..n/2 {
    Shuffle[2*i-1] = Front[i];
    Shuffle[2*i] = Back[i+n/2];
}


writeln("Original = ", A);
writeln("Odd = ", Odd);
writeln("Even = ", Even);
writeln("Front = ", Front);
writeln("Back = ", Back);
writeln("Reverse = ", Reverse);
writeln("Shuffle = ", Shuffle);
