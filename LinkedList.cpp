#ifndef LINKED_LIST_CPP
#define LINKED_LIST_CPP

#include "LinkedList.h"

template<class T>
LinkedList<T>::LinkedList() : head(0), tail(0), _size(0) {}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>& other) : head(0), tail(0), _size(0) {
	for (T& element : other) {
		add(element);
	}
}

template<class T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept : head(0), tail(0), _size(0) {
	for (T& element : other) {
		add(std::move(element));
	}
}

template<class T>
LinkedList<T>::~LinkedList() {
	LinkedElement<T>* temp = head;
	while (temp != nullptr) {
		temp = temp->next;
		delete head;
		head = temp;
	}
	tail = nullptr;
	_size = 0;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator= (LinkedList<T>& other) {
	this->~LinkedList();
	for (T element : other) {
		add(element);
	}
	return *this;
}

template<class T>
LinkedList<T>& LinkedList<T>::operator= (LinkedList<T>&& other) noexcept {
	this->~LinkedList();
	for (T element : other) {
		add(element);
	}
	return *this;
}

template<class T>
ListNode<T> LinkedList<T>::begin() {
	return ListNode<T>(head);
}

template<class T>
ListNode<T> LinkedList<T>::end() {
	return ListNode<T>(nullptr);
}

template<class T>
LinkedList<T> LinkedList<T>::operator+= (LinkedList<T> other) {
	for (T element : other) {
		add(element);
	}
	return *this;
}

template<class T>
LinkedList<T> LinkedList<T>::operator+ (LinkedList<T> other) {
	LinkedList<T> newList;
	newList += *this;
	newList += other;
	return newList;
}

template<class T>
T& LinkedList<T>::operator[](int index) {
	return get(index);
}


template<class T>
void LinkedList<T>::add(T elementIn) {
	if (head == nullptr) {
		head = new LinkedElement<T>(std::move(elementIn));
		tail = head;
	}
	else {
		tail = new LinkedElement<T>(std::move(elementIn), tail);
	}
	_size++;
}

template<class T>
void LinkedList<T>::add(T* elementIn) {
	if (head == nullptr) {
		head = new LinkedElement<T>(*elementIn);
		tail = head;
	}
	else {
		tail = new LinkedElement<T>(*elementIn, tail);
	}
	_size++;
}

template<class T>
T& LinkedList<T>::get(int index) {
	LinkedElement<T>* temp;
	int i;
	for (temp = head, i = 0; temp != nullptr && i < index; temp = temp->next, i++);
	return temp == nullptr ? tail->element : temp->element;
}

template<class T>
int LinkedList<T>::size() {
	return _size;
}

template<class T>
LinkedElement<T>::LinkedElement(T elementIn, LinkedElement<T>* prev) : element(std::move(elementIn)), next(nullptr) {
	if (prev != nullptr) {
		prev->next = this;
	}
}

template<class T>
LinkedElement<T> LinkedElement<T>::operator++ () {
	if (next != nullptr) {
		*this = *next;
	}
	return *this;

}

template<class T>
LinkedElement<T> LinkedElement<T>::operator++ (int) {
	LinkedElement<T>* temp = this;
	if (next != nullptr) {
		*this = *next;
	}
	return *temp;
}

template<class T>
T& LinkedElement<T>::get() {
	return element;
}

template<class T>
ListNode<T>::ListNode(LinkedElement<T>* linkedElement): linkedElement(linkedElement) {}

template<class T>
ListNode<T>& ListNode<T>::operator++() {
	linkedElement = linkedElement->next;
	return *this;
}

template<class T>
ListNode<T>& ListNode<T>::operator++(int) {
	return ++*this;
}

template<class T>
T& ListNode<T>::operator*() {
	return linkedElement->get();
}

template<class T>
bool ListNode<T>::operator==(ListNode<T>& other) {
	return this->linkedElement == other.linkedElement;
}

template<class T>
bool ListNode<T>::operator!=(ListNode<T>& other) {
	return this->linkedElement != other.linkedElement;
}

#endif