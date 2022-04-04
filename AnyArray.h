#pragma once
#ifndef ANYARRAY_H
#define ANYARRAY_H
#include <stdexcept>
#include <iostream>

// Zenyn Ethridge, Jesse Orozco, Geremias Montano-Izazaga

// Dynamic array implementation similar to std::vector

template<typename T>
class AnyArray 
{
protected:
	static const int START_CAPACITY = 20; // holding capacity of array when first implemented
	static const int GROWTH_FACOTR = 2; // factor of growth when array is reallocated
	T* arr;
	int maxSize; // num elements possible before reallocation (initially START_CAPACITY)
	int size; // num elements present
public:
	AnyArray();
	void Deallocate() { if (arr)  delete[] arr; }
	void reallocate();
	void pushback(T);  // add to end of array
	void set(T t, int i) { arr[i] = t; }
	T at(int) const;
	T at(size_t) const;
	T* atReference(int) const;
	int getSize() const { return size; }
	int find(T);
	void remove(int);
	T operator[](int i) const { return at(i); }
	T operator[](size_t i) const { return at(i); }/*
	T & operator[](int i) { return atReference(i); }
	T & operator[](size_t i) { return atReference(i); }*/
	virtual void printEverything();
	void clear() { reallocate(); } // should probably differentiate this
	                               // so that clear() retains same size
};
// what it looks like
template<typename T>
AnyArray<T>::AnyArray() {
	arr = new T[START_CAPACITY];
	maxSize = START_CAPACITY;
	size = 0;
}
// doesn't actually do anything, exists to be overwritten in child classes
template<typename T>
void AnyArray<T>::printEverything() {
	std::cout << 
		"AnyArray<T>::printEverything() failed! Can't cout template\n"
		"Please override function in base class\n";
}
// deleted the value at specified index and reallocates
template<typename T>
void AnyArray<T>::remove(int index) {
	T* foo = new T[maxSize];
	for (int i = 0; i < index; i++) { foo[i] = arr[i]; }
	for (int i = index + 1; i < size; ++i) { foo[i - 1] = arr[i]; }
	delete[] arr;
	arr = foo;
	size--;
}
// similar to std::vector.clear()
template<typename T>
void AnyArray<T>::reallocate() {
	Deallocate();
	arr = new T[START_CAPACITY];
	maxSize = START_CAPACITY;
	size = 0;
}
// similar to std::vector.push_back(), appends to back of array
template<typename T>
void AnyArray<T>::pushback(T t) {
	if (size == maxSize) {
		T* foo = new T[maxSize * GROWTH_FACOTR];
		for (int i = 0; i < maxSize; i++) { foo[i] = arr[i]; }
		delete[] arr;
		arr = foo;
		maxSize *= GROWTH_FACOTR;
	}
	arr[size] = t;
	size++;
}
// returns value at specified index
template<typename T>
T AnyArray<T>::at(int i) const {
	if (i < size) { return arr[i]; }
	std::cout <<
		"Out of range!\n"
		"Size: " << size <<
		"\nIndex: " << i;
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
// |!| should call at(int(i)) rather than copy paste approach
template<typename T>
T AnyArray<T>::at(size_t i) const {
	if (i < size) { return arr[i]; }
	std::cout << " FUCKUPALERT: " << i << ", " << size << '\n';
	throw std::runtime_error("Unexpected code path!"); // stay in range
}
// like at() but returns a pointer
template<typename T>
T* AnyArray<T>::atReference(int i) const {
	if (i < size) { return &arr[i]; }
	throw std::runtime_error("Unexpected code path!");
}
// slow, stupid, linear search
template<typename T> int AnyArray<T>::find(T t) {
	for (int i = 0; i < size; ++i) {
		if (arr[i] == t) { return i; } // returns index if found
	}
	return -1;                       // -1 if not       
}
#endif // !ANYARRAY_H
