use BlockDist;

config const n = 8;
const Space = {1..n, 1..n};

const DmA = Space dmapped blockDist(Space);
const DmB = Space dmapped blockDist(Space);
const DmC = Space dmapped blockDist(Space);

var A: [DmA] real;
var B: [DmB] real;
var C: [DmC] real;

forall (i, j) in A.domain {
  A[i, j] = i + j;
  B[i, j] = j;
}


forall (i, j) in C.domain {
  for k in 1..n do
    C[i, j] += A[i, k] * B[k, j];
}

writeln("total = ", + reduce C);

writeln("Locale:");
for (i,j) in C.domain do
  writef("%2i", C[i, j].locale.id);
writeln();
