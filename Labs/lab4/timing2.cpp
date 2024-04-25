//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Timing example
//
#include <iostream>
#include "unistd.h"
#include <chrono>
using namespace std; 
 
int main() {

  auto start = chrono::steady_clock::now();
  sleep(1);
  auto end = chrono::steady_clock::now();

  auto duration = chrono::duration<double, std::milli> (end - start);
  cout << "Elapsed time: " << duration.count() << " ms\n"; 
}
