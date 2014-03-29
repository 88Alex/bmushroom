#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <vector>

template <class E>
class Stack
{
	public:
	Stack()
	{
		elements = std::vector<E>();
	}
	void push(E element)
	{
		elements.push_back(element);
	}
	E pop()
	{
		E result = elements.back();
		elements.pop_back();
		return result;
	}
	void swap()
	{
		E e1 = pop();
		E e2 = pop();
		push(e1);
		push(e2);
	}
	void drop()
	{
		elements.pop_back();
	}
	void duplicate()
	{
		E top = pop();
		push(top);
		push(top);
	}
	private:
	std::vector<E> elements;
};

#endif
