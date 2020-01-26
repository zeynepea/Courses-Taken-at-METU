#ifndef MYSTACK_HPP
#define MYSTACK_HPP
#include "Node.hpp"

/*You are free to add private/public members inside this template..*/
template <class T>
class MyStack{
    private:
    Node<T> *top;                
    public:
    /*Default constructor*/
    MyStack();                     					
    /*copy constructor*/
    MyStack(const MyStack<T>& rhs);  				
    /*destructor*/
    ~MyStack(); 
    /*overloaded = operator*/
    MyStack<T>& operator=(const MyStack<T>& rhs);  	
    /*returns true if stack is empty*/
    bool isEmpty() const;
    /*push newItem to stack*/
    void push(const T& newItem);
    /*pop item from stack*/
    void pop();
    /*return top item of the stack*/
    Node<T>* Top() const;
	/*Prints the stack entries. This method was already implemented. Do not modify.*/
    void print() const;

    void setTop(const T& newItem);

};

template <class T>
void MyStack<T>::print() const{
    const Node<T>* node = top;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/* TO-DO: method implementations below */
template <class T>
MyStack<T>::MyStack():top(NULL){}
template <class T>
MyStack<T>::~MyStack(){
    while(!isEmpty())
        pop();
}
template <class T>
void MyStack<T>::setTop(const T& newItem){
    Node<T>* x=new Node<T>(newItem);
    top=x;
}
template <class T>
Node<T>* MyStack<T>::Top() const{
    return top;
}
template <class T>
void MyStack<T>::pop() {
    Node<T> *tmp = top;
    top = top->getNext();           // update the stack top
    delete tmp;
}

template <class T>
bool MyStack<T>::isEmpty() const {
   return top == NULL;          // if topPtr  is Null then empty
}
template <class T>
void MyStack<T>::push(const T& newItem) {
   Node<T> *newPtr = new Node<T>(newItem);   // create a new node              
   newPtr->setNext(top);               // link this node to the stack
   top = newPtr;                     // update the stack top
}
template <class T>
MyStack<T>& MyStack<T>::operator=(const MyStack& rhs) {
    if (this != &rhs) {               // if not assigned to itself
        while (!isEmpty()) pop();       // delete former elements of lhs
        if (!rhs.top)                // if rhs is empty
            top = NULL;              // then just make lhs empty
        else {                          // else
            top = new Node<T>;          // create a new node
            setTop(rhs.top->getData());    // copy first item of rhs to this
            Node<T>* q = rhs.top->getNext(); // q shows second item of rhs
            Node<T>* p = top;           // p shows top item of lhs
            while (q) {                     // while more item in rhs
                p->setNext( new Node<T>(q->getData()));
                p = p->getNext();         
                q = q->getNext();                  
            }
            p->setNext(NULL);                // put NULL to end of lhs
        }
    }
    return *this;                      // return lhs for cascaded op. 
}
template <class T>
MyStack<T>::MyStack(const MyStack& rhs) {
    *this = rhs;       // reuse assignment operator
} 
#endif /* MYSTACK_HPP */

