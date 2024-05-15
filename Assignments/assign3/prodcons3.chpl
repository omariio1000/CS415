// A producer-consumer program using the cqueue module 
//
// Version 2: single producer and multiple consumers
//

use cqueue;

config const numItems = 32;
config const numCons = 2;

proc producer() {
  for i in 1..numItems do {
    var added = cqueue.add(i);
    writeln("Producer added ", i, " at buf[", added, "]");
  }

  for i in 1..numCons do
    cqueue.add(-1);
}

proc consumer(n) {
  while true < numItems do {
    var removed = cqueue.remove();
    if (removed[1] == -1) then
      return;
    else
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
