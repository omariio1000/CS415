# Makefile for CS 415P/515 Lab 1 (J. Li)
#

all:	sum sum-pthd sum-oo Sum sum-omp sum-mpi sum1 sum2

sum: 	sum.c
	gcc -g -o sum sum.c

sum-pthd: sum-pthd.c	
	gcc -pthread -g -o sum-pthd sum-pthd.c	

sum-oo:	sum-oo.cpp
	g++ -pthread -g -o sum-oo sum-oo.cpp

Sum:    Sum.java
	javac Sum.java        

sum-omp: sum-omp.c
	gcc -fopenmp -g -o sum-omp sum-omp.c

sum-mpi: sum-mpi.c
	mpicc -g -o sum-mpi sum-mpi.c

sum1: 	sum1.chpl
	chpl -g -o sum1 sum1.chpl

sum2: 	sum2.chpl
	chpl -g -o sum2 sum2.chpl

clean:
	rm -f sum sum-pthd sum-oo Sum.class sum-omp sum-mpi sum1 sum2
