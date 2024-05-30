use BlockDist;

config const N = 8;
const D = {1..N, 1..N};
const PD = D.expand(1,1);     

const DmPD = PD dmapped blockDist(PD);
const DmD = D dmapped blockDist(D);

var a: [DmPD] real, na: [DmPD] real; 

proc print(a: []) {
  for i in a.domain.dim(0) {
    for j in a.domain.dim(1) do
      writef("%5.2dr", a[i,j]);
    writeln();
  }
}

a[PD.dim(0), 0] = 1.0;
a[0, PD.dim(1)] = 2.0;
na[PD.dim(0), 0] = 1.0;
na[0, PD.dim(1)] = 2.0;
writeln("Init:");
print(a); 

config const epsilon = 0.01;  // convergence tolerance
config const verbose = false; // printing control

var delta: real;     // for tracking convergence 
var t = 0;           // iteration count
do {
  forall ij in D do  // no need to update padded elements
    na[ij] = (a[ij+(0,1)]+a[ij+(0,-1)]+a[ij+(1,0)]+a[ij+(-1,0)])/4.0;
  t += 1;
  delta = max reduce abs(na - a);
  a = na;
  if (verbose) {
    writef("Jacobi iter %i (delta=%5.2r):\n", t, delta);
    print(a);
  }
} while (delta > epsilon);

writeln("Result: t=", t);
print(a);

writeln("Locale:");
forall (i,j) in a.domain do
  writef("%2i", a[i,j].locale.id);
writeln();
