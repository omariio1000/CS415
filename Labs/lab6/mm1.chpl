config const N = 8;

var a, b, c: [1..N, 1..N] int;

for i in 1..N {
    for j in 1..N {
        a[i,j] = (i-1) + (j-1) + 2;
        b[i,j] = (j-1) + 1;
    }
}

for i in 1..N {
    for j in 1..N {
        c[i,j] = 0;
        for k in 1..N {
            c[i,j] += a[i,k] * b[k,j];
        }
    }
}

var total = 0;
for i in 1..N {
    for j in 1..N {
        total += c[i,j];
    }
}

writeln("Total = ", total, " (N=", N, ")");