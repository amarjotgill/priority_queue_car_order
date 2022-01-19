
// driver.cpp: sample driver for the MQueue class.

#include "mqueue.h"
#include "mqueue.cpp"
using namespace std;
// Priority functions compute an integer priority for an order.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

int main() {
  try{
    MQueue queue1(priorityFn1);
    MQueue queue2(priorityFn1);

    // Create some test orders and insert them into the queue
    //Order(string customer, int priority, int material, int quantity, int arrival)
    Order order1("Ford",1,1,1,20);
    Order order2("Honda",5,1,2,21);
    Order order3("Toyota",2,0,0,22);
    Order order4("GM",4,3,2,23);

    queue1.insertOrder(order3);
    queue1.insertOrder(order1);
    queue1.insertOrder(order2);
    queue1.insertOrder(order4);

    std::cout << "\nqueue1 dump after inserting 4 orders (Toyota, Honda, Ford, GM):" << std::endl;
    queue1.dump();

    Order order5("Ferrari",6,3,2,1);
    Order order6("Hyundai",3,0,0,10);

    cout << "inserting q2" << endl;

    queue2.insertOrder(order5);
    queue2.insertOrder(order6);
    std::cout << "\nqueue2 dump after inserting 2 students (Ferrari, Hyundai):" << std::endl;
    queue2.dump();

    queue1.mergeWithQueue(queue2);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.

    std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after merging with queue2:\n";
    queue1.dump();

    queue1.setPriorityFn(priorityFn2);

    std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
    queue1.dump();
    
    queue1.setPriorityFn(priorityFn1);

    std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
    queue1.dump();

    // Remove and print orders in priority order

    std::cout << "\nFinal queue1 in priority order:\n";
    while (queue1.numOrders() > 0) {
      Order order = queue1.getNextOrder();
      // this will be printed in the ascending order (sorted)
      std::cout << "[" << priorityFn1(order) << "] " << order << std::endl;
  }
  }
    catch(std::out_of_range &e){
      cout << e.what() << endl;
  }
  return 0;
}

int priorityFn1(const Order & order) {
  //priority value is determined based on some criteria
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  int priority = order.getMaterialAvailability() + order.getQuantity() + order.getArrivalTime();
  return priority;
}

int priorityFn2(const Order & order) {
  //priority is determined by a production manager
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  return order.getPMPriority();
}