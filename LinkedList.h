#ifndef LINKED_LIST_H
#define LINKED_LIST_H

template<class T>
class LinkedList;

template<class T>
class LinkedElement;

template<class T>
class ListNode;

template<class T>
class LinkedList
{
	LinkedElement<T>* head;
	LinkedElement<T>* tail;
	int _size = 0;
public:

	LinkedList();
	LinkedList(LinkedList<T>& other);
	LinkedList(LinkedList<T>&& other) noexcept;
	~LinkedList();

	LinkedList<T>& operator=(LinkedList<T>& other);
	LinkedList<T>& operator=(LinkedList<T>&& other) noexcept;

	void add(T element);
	void add(T* element);
	T& get(int index);
	int size();

	ListNode<T> begin();
	ListNode<T> end();

	LinkedList<T> operator+= (LinkedList<T> other);

	LinkedList<T> operator+ (LinkedList<T> other);

	T& operator[](int index);

	friend class LinkedElement<T>;
};

template<class T>
class LinkedElement {
protected:
	T element;
	LinkedElement<T>* next;

	LinkedElement() : next(nullptr) {};
	LinkedElement(T elementIn, LinkedElement<T>* prev = nullptr);

	friend class LinkedList<T>;
	friend class ListNode<T>;
	public: 
		LinkedElement<T> operator++ ();
		LinkedElement<T> operator++ (int);
		
		operator T() {
			return element;
		}
		T& get();
};

template<class T>
class ListNode {

	LinkedElement<T>* linkedElement;

public:

	ListNode(LinkedElement<T>* linkedElement);

	ListNode<T>& operator++();
	ListNode<T>& operator++(int);

	T& operator*();

	bool operator==(ListNode<T>& other);
	bool operator!=(ListNode<T>& other);

};

#include"LinkedList.cpp"

#endif