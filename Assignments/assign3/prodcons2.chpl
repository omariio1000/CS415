// A producer-consumer program using the cqueue module 
//
// Version 2: single producer and multiple consumers
//

use cqueue;

config const numItems = 32;
config const numCons = 2;

var numRemoved: sync int = 0;  

proc producer() {
  for i in 1..numItems do {
    var added = cqueue.add(i);
    writeln("Producer added ", i, " at buf[", added, "]");
  }
}

proc consumer(n) {
  while true do {
    var numRem = numRemoved.readFE();
    
    if numRem >= numItems then {
      numRemoved.writeEF(numRem + 1);
      break;  
    }

    var removed = cqueue.remove();
    numRemoved.writeEF(numRem + 1);
    writeln("Consumer[", n, "] removed ", removed[1], " from buf[", removed[0], "]");
  }
}

proc main() {
  cobegin {
    begin producer();
    for n in 1..numCons do
      begin consumer(n); 
  }
}
