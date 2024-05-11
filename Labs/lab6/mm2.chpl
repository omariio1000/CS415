config const N = 8;

var a, b, c: [1..N, 1..N] int;

forall i in 1..N {
    forall j in 1..N {
        a[i,j] = (i-1) + (j-1) + 2;
        b[i,j] = (j-1) + 1;
    }
}

forall i in 1..N {
    forall j in 1..N {
        c[i,j] = + reduce (a[i,..] * b[..,j]);
    }
}

var total = + reduce c;
writeln("Total = ", total, " (N=", N, ")");