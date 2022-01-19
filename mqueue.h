


#ifndef _MQUEUE_H
#define _MQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;
using std::ostream;
using std::string;
using std::out_of_range;

class Grader;   // forward declaration (for grading purposes)
class Tester;   //forward declaration
class MQueue;   //forward declaration
// Constant parameters, min and max values
// The probability of material availability
enum Material {M100,M75,M50,M25,M10};
// the number of products ordered
enum Quantity {Q100000,Q75000,Q50000,Q25000,Q10000};
#define DEFAULTCUSTOMER "NONAME"
const int HIGHESTPRIO = 1;//one is the highest priority
// the lowest priority would be M10+Q10000+50 which is 58
const int LOWESTPRIO = 58;
//
// order class
//
class Order {
  public:
  friend class Grader; // for grading purposes
  friend class Tester; // for testing purposes
  // Default constructor
  Order() {
    m_customer = DEFAULTCUSTOMER; m_overWritten = 8; 
    m_material = M10; m_quantity = Q10000; m_arrivalTime = 50;
  }

  // Alternative constructor
  Order(string customer, int priority, int material, int quantity, int arrival) {
    if ( (priority < HIGHESTPRIO || priority > LOWESTPRIO) || 
          (quantity < Q100000 || quantity > Q10000) || 
          (material < M100 || material > M10) || 
          (arrival < 1 || arrival > 50)){
      m_customer = DEFAULTCUSTOMER; m_overWritten = 8; 
      m_material = M10; m_quantity = Q10000; m_arrivalTime = 50; 
      throw out_of_range("Bad value to Order constructor.");
    }
    else{
      m_customer = customer; m_overWritten = priority; 
      m_material = material; m_quantity = quantity; 
      m_arrivalTime = arrival;
    }
  }

  string getCustomer() const {return m_customer;}
  int getPMPriority() const {return m_overWritten;}
  int getMaterialAvailability() const {return m_material;}
  int getQuantity() const {return m_quantity;}
  int getArrivalTime() const {return m_arrivalTime;}
  int getQuantityValue() const{
    int result = 0;
    switch (m_quantity)
    {
    case Q100000:result = 100000;break;
    case Q75000:result = 75000;break;
    case Q50000:result = 50000;break;
    case Q25000:result = 25000;break;
    default:result = 10000;break;
    }
    return result;
  }
  int getAvailabilityProbablity() const{
    int result = 0;
    switch (m_material)
    {
    case M100:result = 100;break;
    case M75:result = 75;break;
    case M50:result = 50;break;
    case M25:result = 25;break;
    default:result = 10;break;
    }
    return result;
  }
  private:
  string m_customer;//customer name
  // m_overWritten is a value specified by production manager,
  // a production manager can overwrite the calculated priority
  int m_overWritten;
  // m_material is a value indicating the probability of material availability
  // for valid values refer to enum type Material
  int m_material;//material availability, for valid values refer to enum type Material
  int m_quantity;//quantity of the order, for valid values refer to enum type Quantity
  // m_arrivalTime is an integer indicating in what order customers placed their order
  // it can be between 1 and 50, at any time the plant only accepts 50 orders at max
  // therefore this value cannot go beyond 50
  // a value of 1 means this was the first customer who placed the order
  // a value of 50 means this was the 50th customer who placed the order
  int m_arrivalTime;
};

class Node {
  public:
  friend class Grader; // for grading purposes
  friend class Tester; // for testing purposes
  friend class MQueue;
  Node(Order order) {  
    m_order = order;
    m_right = nullptr;
    m_left = nullptr;
  }
  Order getOrder() const {return m_order;}
  private:
  Order m_order;   // order information
  Node * m_right;  // right child
  Node * m_left;   // left child
};

// Overloaded insertion operators for Order and Node
ostream& operator<<(ostream& sout, const Order& order);
ostream& operator<<(ostream& sout, const Node& node);

// Priority function pointer type
typedef int (*prifn_t)(const Order&);  

class MQueue {
public:

  friend class Grader; // for grading purposes
  friend class Tester; // for testing purposes
  
  // Constructor.  Create a queue with given priority function.
  MQueue(prifn_t priFn);

  // Destructor
  ~MQueue();

  // Copy constructor
  MQueue(const MQueue& rhs);

  // Assignment Operator
  MQueue& operator=(const MQueue& rhs);
  
  // Add an order to the queue
  void insertOrder(const Order& input);

  // Extract the highest priority order
  Order getNextOrder();

  // Merge two queues
  void mergeWithQueue(MQueue& rhs);

  // Remove all orders from the queue
  void clear();
  
  // Return number of orders in queue
  int numOrders() const;
  
  // Print the queue using preorder traversal.  Although the first order
  // printed should have the highest priority, the remaining orders will
  // not necessarily be in priority order.
  void printOrderQueue() const;

  // Get the current priority function.
  prifn_t getPriorityFn() const;

  // Set a new priority function.  Must rebuild the heap!!!
  void setPriorityFn(prifn_t priFn);
  
  // Dump the heap.  For debugging purposes.
  void dump() const;

private:
  Node * m_heap;        // Pointer to root of skew heap
  int m_size;           // Current size of the heap
  // Function pointer; function to compute priority
  prifn_t m_priorFunc;

  // Private functions
  void dump(Node *pos) const;

  /******************************************
   * Private function declarations go here! *
   ******************************************/
  void deleteQueue(Node* node);
  void printPreOrder(Node* node) const;
  Node* mergeQueueHelper(Node *q1, Node *q2);
  Node* deepCopyHelper(Node* node);
};

#endif
