#include "mqueue.h"
using namespace std;

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

class Tester{
  public:
  Node* getRoot(MQueue current);
  void checkMinHeap(Node* m_heap, MQueue current, bool &flag);
  void testTimes(MQueue current);
};

int main(){
  Tester tester;
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

    queue2.insertOrder(order5);
    queue2.insertOrder(order6);
    
    queue1.mergeWithQueue(queue2);

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

  cout << endl;
  Node* temp = tester.getRoot(queue1);
  bool test = true;
  tester.checkMinHeap(temp,queue1,test);
  if(test){
    cout << "Test passed! queue properties have been maintained" << endl;
  }else{
    cout << "Test failed" << endl;
  }

  cout << endl;
  MQueue queue3(priorityFn2);
  cout << "Attempting to merge queues with different priority funcitons" << endl;
  try{
    queue3.mergeWithQueue(queue1);
  }catch(std::domain_error &e){
    cout << e.what() << endl;
  }

  cout << endl;
  
  cout << "Trying copy constructor for normal case" << endl;
  MQueue queue5(queue1);
  cout << "contents of new queue5 copied from queue1" << endl;
  queue5.printOrderQueue();
  cout << endl;

  cout << "Trying assignment operator for normal case" << endl;
  MQueue queue6(priorityFn1);
  queue6 = queue1;
  cout << "Contents of new queue6 assigned from queue1" << endl;
  queue6.printOrderQueue();
  cout << endl;

  MQueue emptyQueue(priorityFn2);
   cout << "Trying to copy empty queue to new queue7" << endl;
  MQueue queue7(emptyQueue);
  cout << endl;

  cout << "Trying to assign empty queue to new queue7" << endl;
  queue7 = emptyQueue;
  cout << endl;

  tester.testTimes(queue5);

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
  cout << endl;

  MQueue queue4(priorityFn1);
  cout << "Attempting to dequeue an empty queue" << endl;
  try{
    queue4.getNextOrder();
  }catch(std::domain_error &e){
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

int priorityFn2(const Order & order){
  //priority is determined by a production manager
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  return order.getPMPriority();
}

void Tester::checkMinHeap(Node* m_heap, MQueue current,bool &flag){
  if(m_heap == nullptr){
    return;
  }
  if(m_heap -> m_left && current.m_priorFunc(m_heap->getOrder()) > current.m_priorFunc(m_heap -> m_left -> getOrder())){
      flag = false;
  }if(m_heap -> m_right && current.m_priorFunc(m_heap->getOrder()) > current.m_priorFunc(m_heap -> m_right -> getOrder())){
      flag = false;
  }
  checkMinHeap(m_heap -> m_left, current, flag);
  checkMinHeap(m_heap -> m_right, current, flag);
}

Node* Tester::getRoot(MQueue current){
  if(current.m_heap == nullptr){
    return nullptr;
  }
  return current.m_heap;
}

void Tester::testTimes(MQueue current){
  cout << "Measuring amount of time insertion takes" << endl;
   clock_t start, stop; 
   double T = 0.0;
   Order order10("Lamborgini",1,1,1,20);
   Order order11("Mercedes",5,1,2,23);

   cout << "Inserting 1000 new orders into queue5 making it a large queue" << endl;
    for (int i = 0; i < 1000; i++){
      current.insertOrder(order10);
    }
    
    start = clock();
    current.insertOrder(order10);
    stop = clock();
    T = stop - start;
    cout << "Inserting a new order into queue5 took " << T << " seconds" << endl;

    start = clock();
    current.insertOrder(order11);
    stop = clock();
    T = stop - start;
    cout << "Inserting another new order into queue5 took " << T << " seconds" << endl;

    start = clock();
    current.getNextOrder();
    stop = clock();
    T = stop - start;
    cout << "Extracting top priority order took  " << T << " seconds" << endl;

    start = clock();
    current.getNextOrder();
    stop = clock();
    T = stop - start;
    cout << "Extracting the next top priority order took  " << T << " seconds" << endl;
}