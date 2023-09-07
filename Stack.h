#ifndef STACK_H
#define STACK_H

#include <stdexcept>

using namespace std;

class UnderflowException { };

template <class Object>
class Stack
{
public:
	Stack( );
	Stack( const Stack & rhs );
	~Stack( );

	bool isEmpty( ) const;
	bool isFull( ) const;
	void makeEmpty( );

	void pop( );
	void push( const Object & x );
	Object topAndPop( );
    Object & top( );

	const Stack & operator=( const Stack & rhs );
private:
	struct ListNode
	{
		Object   element;
		ListNode *next;

		ListNode( const Object & theElement, ListNode * n = nullptr )
			: element( theElement ), next( n ) { }
	};

	ListNode *topOfStack;  // list itself is the stack
};

//////////////////////////////////////////////////////////////////////////////////////
///		Constructor, Destructor, Deep Copy, Shallow Copy(copy constructor)        ////						
//////////////////////////////////////////////////////////////////////////////////////

//Construct the stack
template <class Object>
Stack<Object>::Stack( )
{
	topOfStack = nullptr;
}

//Deep copy
template <class Object>
const Stack<Object> & Stack<Object>::
	operator=( const Stack<Object> & rhs )
{
	if ( this != &rhs )
	{
		makeEmpty( );
		if ( rhs.isEmpty( ) )
			return *this;

		ListNode *rptr = rhs.topOfStack;
		ListNode *ptr  = new ListNode( rptr->element );
		topOfStack = ptr;

		for ( rptr = rptr->next; rptr != nullptr; rptr = rptr->next )
			ptr = ptr->next = new ListNode( rptr->element );
	}
	return *this;
}

// Copy constructor
template <class Object>
Stack<Object>::Stack( const Stack<Object> & rhs )
{
	topOfStack = nullptr;
	*this = rhs; // deep copy 
}

//Destructor
template <class Object>
Stack<Object>::~Stack( )
{
	makeEmpty( );
}


//////////////////////////////////////////////////////////////////////////////////////
///								Member Functions							      ////						
//////////////////////////////////////////////////////////////////////////////////////

//Test if the stack is logically full
template <class Object>
bool Stack<Object>::isFull( ) const
{
	return false;
}

//Test if the stack is logically empty
template <class Object>
bool Stack<Object>::isEmpty( ) const
{
	return topOfStack == nullptr;
}

//Return the most recently inserted item in the stack or throw an exception if empty
//just gets the element, does not delete anything
template <class Object>
Object & Stack<Object>::top( ) 
{
	if ( isEmpty( ) )
		throw UnderflowException();
	return topOfStack->element;
}

//Remove the most recently inserted item from the stack
template <class Object>
void Stack<Object>::pop( ) 
{
	if ( isEmpty( ) )
		throw UnderflowException();
	ListNode *oldTop = topOfStack;
	topOfStack = topOfStack->next;
	delete oldTop;
}


//Insert x into the stack
template <class Object>
void Stack<Object>::push( const Object & x )
{
	topOfStack = new ListNode( x, topOfStack );
}

//Return and remove the most recently inserted item from the stack, top and pop together
template <class Object>
Object Stack<Object>::topAndPop( )
{
	Object topItem = top( );
	pop( );
	return topItem;
}

//Make the stack logically empty
template <class Object>
void Stack<Object>::makeEmpty( )
{
	while ( ! isEmpty( ) )
		pop( );
}

#endif 
