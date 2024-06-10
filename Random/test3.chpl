// Program 2
// chpl -g test2.chpl --comm=gasnet
use BlockDist;
const D = {1..N};
const MD = D mapped BlockDist(D);
var a:[MD] int;

for t in D by 2 {
    forall i in MD by 2 do
        if i < N && a(i) > a(i+1) then
            a(i) <=> a(i+1);
    forall i in MD by 2 do
        if i > 1 && a(i-1) > a(i) then
            a(i-1) <=> a(i);
}