//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Prime-Finding -- Data parallel version (Chapel)
//
config const N = 100;
var limit = sqrt(N):int;
var candidate:[2..N] bool;

proc primePar(): void {
  candidate = true;          // initialization

  for i in 2..limit do       // finding primes
    if (candidate[i]) then
      forall j in i*2..N by i do
        candidate[j] = false;
    
  var cnt = candidate.count(true); // counting primes
  writeln("prime-par found ", cnt, " primes in [1..", N, "]:");
                              
  for i in 2..N do           // printing results
    if (candidate[i]) then 
      write(i, " ");
  writeln();
}
