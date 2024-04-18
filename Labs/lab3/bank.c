//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// A deposit-withdraw program.
//
// Usage: ./bank
//
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define INIT_BALANCE  1000
#define NUM_DEPOSITS  10
#define NUM_WITHDRAWS 10
#define DEPOSIT_AMT   100
#define WITHDRAW_AMT  200

int total;  // account balance

void deposit(int i) { 
  int amount = rand() % DEPOSIT_AMT;
  int oldtotal = total;
  total += amount;
  printf("Deposit-%d %5d (%3d -> %3d)\n", i, amount, oldtotal, total);
}

void withdraw(int i) { 
  int amount = rand() % WITHDRAW_AMT;
  int oldtotal = total;
  if (amount < total) {
    total -= amount;
    printf("Withdraw-%d %4d (%3d -> %3d)\n", i, amount, oldtotal, total);
  } else {
    printf("Withdraw-%d %4d (%3d) **aborted**\n", i, amount, oldtotal);
  }
}

int main() {
  total = INIT_BALANCE;
  srand(time(NULL));
  for (int i = 1; i <= NUM_DEPOSITS; i++)
    deposit(i);
  for (int i = 1; i <= NUM_WITHDRAWS; i++)
    withdraw(i);
}


