// A producer-consumer program using the cqueue module 
//
// Version 1: Base version: single producer and single consumer
//

use cqueue;

config const numItems = 32;

proc producer() { 
  for i in 1..numItems do {
    var added = cqueue.add(i);
    writeln("Producer added ", i, " from buf[", added, "]");
  }
}

proc consumer() { 
  for i in 1..numItems do {
    var removed = cqueue.remove();
    writeln("Consumer rem'd ", removed[1], " from buf[", removed[0], "]");
  }
    
}

proc main() {
  cobegin {
    producer();
    consumer();
  }
} 
