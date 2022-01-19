

#include "mqueue.h"
using namespace std;
using std::domain_error;
const int ZERO = 0;

MQueue::MQueue(prifn_t priFn){
  m_heap = nullptr;
  m_priorFunc = priFn;
  m_size = ZERO;
}

MQueue::~MQueue(){
  // prevent deleting an already empty queue
  if(m_size == ZERO){
    return;
  }
  m_size = ZERO;
  m_priorFunc = nullptr;
  deleteQueue(m_heap);
}

MQueue::MQueue(const MQueue& rhs){
    // copy over properties of rhs
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heap = deepCopyHelper(rhs.m_heap);
  
}

MQueue& MQueue::operator=(const MQueue& rhs){
  if(this != &rhs){
    // case where heap to assign is empty
    if(rhs.m_heap == nullptr){
      cout << "Can not assign an empty queue to current queue" << endl;
    }else{
      // clearing current heap so no leaks
      clear();
      m_size = rhs.m_size;
      m_priorFunc = rhs.m_priorFunc;
      m_heap = deepCopyHelper(rhs.m_heap);
    }
    }
  return *this;
}

void MQueue::insertOrder(const Order& input) {
  Node* newNode = new Node(input);
  // case where queue is currently empty
  if(m_heap == nullptr){
    m_size++;
    m_heap = newNode;
  }else{
    m_size++;
   m_heap = mergeQueueHelper(m_heap, newNode);
  }
}

Order MQueue::getNextOrder() {
  if(m_heap == nullptr){
    throw std::domain_error("Heap is empty can not extract");
  }else{
    Node *temp = m_heap;
    m_heap = mergeQueueHelper(m_heap -> m_left, m_heap -> m_right);
    Order tempOrder = temp -> getOrder();
    delete temp;
    m_size--;
    return tempOrder;
  }
}

void MQueue::mergeWithQueue(MQueue& rhs){
  // case where you try to merge heap with empty heap
  if(rhs.m_size == ZERO){
    return;
  }else if(rhs.m_priorFunc != m_priorFunc){
    throw std::domain_error("Can not merge queues with different priorities");
  }else{
     Node *temp = rhs.m_heap;
    m_size += rhs.m_size;
    // since now merge you can set rhs' size to 0 
    rhs.m_size = ZERO;
    m_heap = mergeQueueHelper(m_heap, temp);
  }
}

void MQueue::clear() {
  deleteQueue(m_heap);
}

int MQueue::numOrders() const{
  return m_size;
}

void MQueue::printOrderQueue() const {
  if(m_heap == nullptr){
   cout << "Can not print an empty heap" << endl;
  }else{
    printPreOrder(m_heap);
  }
}

prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;
}

void MQueue::setPriorityFn(prifn_t priFn) {
  m_priorFunc = priFn;
  // create a new queue with new priority
  MQueue temp(m_priorFunc);
  while(m_size > ZERO){
      // get the nodes in current queue and insert them into new queue
      Order newOrder = getNextOrder();
      temp.insertOrder(newOrder);
  }
  // assignemnt operator to update to new queue with new priority 
  *this = temp;
}

// overloaded insertion operator for Order class
ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order: " << order.getCustomer() << ", PM priority: " << order.getPMPriority()
       << ", quantity: " << order.getQuantityValue() 
       << ", material availability: " << order.getAvailabilityProbablity() << "%"
       << ", arrival time: " << order.getArrivalTime();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getOrder();
  return sout;
}

// for debugging
void MQueue::dump() const
{
  if (m_size == ZERO) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

// for debugging
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
    dump(pos->m_right);
    cout << ")";
  }
}

void MQueue::deleteQueue(Node *node){
  if (node == nullptr){
        return;
    }
    // first delete both subtrees 
    deleteQueue(node -> m_left);
    deleteQueue(node -> m_right);
     
    // then delete the node 
    delete node;
}

void MQueue::printPreOrder(Node *node) const{
    if(node == nullptr){
      return;
    }
    // pre order traversal to print heap
    cout << node -> getOrder() << endl;
    printPreOrder(node -> m_left);
    printPreOrder(node -> m_right);
}

 Node* MQueue::mergeQueueHelper(Node* q1, Node* q2){
    if (q1 == nullptr){
      return q2;
    }
		if (q2 == nullptr){
      return q1;
    }
		// make sure that q1 has smaller priority swap if q2 has smaller priority because min heap
		if (m_priorFunc(q1 -> getOrder()) > m_priorFunc(q2 -> getOrder())){
      swap(q1,q2);
    }
		// swap q1 -> left and q1 -> right
    swap(q1->m_left, q1->m_right);

		q1 -> m_left = mergeQueueHelper(q1 -> m_left, q2);
    // return new root
    return q1;
 }

 Node* MQueue::deepCopyHelper(Node* node){
   if(node == nullptr){
     return nullptr;
   }
   // create new node
    Node* insertNode = new Node(node->getOrder());
    // copy node's left and right
   insertNode -> m_left = deepCopyHelper(node->m_left);
   insertNode -> m_right = deepCopyHelper(node -> m_right);
   // return new root of heap
   return insertNode;
 }