// Oddven sort (data parallel version with early termination)
//
use Random;
config const DEBUG = true;
config const WORST = false;
config const N = 10;

var randStream = new randomStream(uint(8));
var swappedOdd = false;
var swappedEven = false;

// Initialize array with random uint(8) values
// (except if WORST flag is on, set array to the reverse of 1..N)
proc init_array(ref a:[]) {
  // ... fill ...
  for i in 1..N do
    if WORST then
      a[i] = N:uint(8) - i:uint(8) + 1;
    else
      a[i] = randStream.next();

  if DEBUG then
    writeln("Init: ", a); 
}

// Verify that array is sorted
// (if not sorted, show the violation pair of elements)
proc verify_array(ref a:[]) {
  // ... fill ...
  for i in 1..N-1 do
    if (a[i] > a[i+1]) then {
      writeln("Failed: a[", i,"]=", a[i], " a[", i+1,"]=", a[i+1]);
      return;
    }

  writeln(N, " element array is sorted.");  
}

// Oddeven sort
// 
proc oddeven_sort(ref a:[]) {
  // ... fill ...
  for t in 1..N/2 do {
    for i in 2..N-1 by 2 do {
      if (a[i] > a[i+1]) then {
        a[i] <=> a[i+1];
        swappedEven = true;
      }
    }

    for i in 1..N-1 by 2 do {
      if (a[i] > a[i+1]) then {
        a[i] <=> a[i+1];
        swappedOdd = true;
      }
    }

    if DEBUG then {
      writeln("t=", t, ":  ", a);
    }

    if (swappedEven == false && swappedOdd == false && t < N/2) {
      var k = N/2 - t;
      writeln("Early termination saved ", k, " rounds!");
      return;
    }

    swappedEven = false;
    swappedOdd = false;

  }
}

proc main() {
  var a: [1..N] uint(8);
  init_array(a);
  oddeven_sort(a);
  verify_array(a);
}
