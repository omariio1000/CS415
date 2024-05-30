use BlockDist;

config const N = 8;
const D = {1..N, 1..N};
const PD = D.expand(1,1);      // padded domain

const DmPD = PD dmapped blockDist(PD);
const DmD = D dmapped blockDist(D);

var a: [DmPD] real; // single buffer for a

// Initialize boundary conditions
forall (i, j) in a.domain {
  if i == PD.dim(0).high && j == PD.dim(1).low then
    a[i, j] = 1.0;
  else if i == PD.dim(0).low && j == PD.dim(1).high then
    a[i, j] = 2.0;
  else
    a[i, j] = 0.0;
}

writeln("Init:");
printArray(a);

config const epsilon = 0.01;  // convergence tolerance
var delta: real;     // for tracking convergence
var t = 0;           // iteration count

do {
  var localDelta = 0.0;
  forall (i, j) in D with (ref localDelta) {
    var newValue = (a[i+1,j] + a[i-1,j] + a[i,j+1] + a[i,j-1]) / 4.0;
    localDelta = max(localDelta, abs(newValue - a[i, j]));
    a[i, j] = newValue;
  }
  delta = localDelta;
  t += 1;
} while (delta > epsilon);

writeln("Result: t=", t);
printArray(a);

// Verification code to check partitioning
writeln("Locale:");
forall (i,j) in a.domain do
  writef("%2i", a[i,j].locale.id);
writeln();

// Utility procedure to print the array
proc printArray(a: []) {
  for i in a.domain.dim(0) {
    for j in a.domain.dim(1) do
      writef("%5.2dr ", a[i,j]);
    writeln();
  }
}
