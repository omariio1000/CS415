//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Prime-Finding -- Sequential version (Chapel)
//
config const N = 100;
var limit = sqrt(N):int;
var candidate:[2..N] bool;

proc primeSeq(): void {
  for i in 2..N do           // initialization
    candidate[i] = true;
  for i in 2..limit do       // finding primes
    if (candidate[i]) then
      for j in i*2..N by i do
        candidate[j] = false;
  var cnt = 0;               // counting primes
  for i in 2..N do
    if (candidate[i]) then
      cnt += 1;
  writeln("prime-seq found ", cnt, " primes in [1..", N, "]:");
  for i in 2..N do           // printing results 
    if (candidate[i]) then 
      write(i, " ");
  writeln();
}
