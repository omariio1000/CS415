// Program 2
// chpl -g test2.chpl --comm=gasnet
use BlockDist;
const D1 = {1..6};
const D2 = D1 dmapped blockDist(D1);
var x: [D1] int;
var y: [D2] int;
forall i in D1 do x[i] = here.id;
forall i in D2 do y[i] = here.id;
writeln(x);
writeln(y);