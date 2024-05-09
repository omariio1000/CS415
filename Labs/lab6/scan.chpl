//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Scans and reductions
//
config const n = 5;
var a: [1..n] int = [1,3,5,7,9];
var total = + reduce a;       // 25
var maxVal = max reduce a;    // 9
var minMax = minmax reduce a; // (1,9)
var maxLoc = maxloc reduce zip(a,a.domain); // (9,5)
var listTtl = + scan a;       // [1,4,9,16,25]
var listMax = max scan a;     // [1,3,5,7,9]
var cnt = a.count(9);         // 1

writeln("a = ", a);
writeln("total   = ", total);
writeln("maxVal  = ", maxVal);
writeln("minMax  = ", minMax);
writeln("maxLoc  = ", maxLoc);
writeln("listTtl = ", listTtl);
writeln("listMax = ", listMax);
