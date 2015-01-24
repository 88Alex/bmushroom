#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <vector>
#include <algorithm>

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
	void push(std::vector<E> elementVector)
	{
		for(int i = 0; i < elementVector.size(); i++)
		{
			push(elementVector.at(i));
		}
	}
	E pop()
	{
		if(elements.size() < 1) return E();
		E result = elements.back();
		elements.pop_back();
		return result;
	}
	E peek()
	{
		if(elements.size() < 1) return E();
		return elements.back();
	}
	std::vector<E> top(int n)
	{
		std::vector<E> result;
		for(int i = 0; i < n; i++)
		{
			result.push_back(pop());
		}
		// the vector now contains the elements top to bottom, so reverse it
		std::reverse(result.begin(), result.end());
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
	void clear()
	{
		elements.clear();
	}
	E second()
	{
		if(elements.size() < 2) return E();
		return elements.at(elements.size() - 2);
	}
	private:
	std::vector<E> elements;
};

#endif
