/** A basic dynamic array */

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

template <typename T>
class Array{
	T *arr;
	int capacity;

	void expand(){
		T *old = arr;
		capacity*=2;
		arr = new T[capacity];
		for(int i=0; i<size; ++i) arr[i] = old[i];
		delete[] old;
	}

public:

	int size;

	Array(): size(0), capacity(16){
		arr = new T[16];
	}

	Array(int Capacity): size(0), capacity(Capacity){
		arr = new T[Capacity];
	}

	Array(Array<T> &other){
		delete[] arr;
		arr = new T[other.capacity];
		capacity = other.capacity;
		size = other.size;
		for(int i=0; i<other.size; ++i) arr[i] = other.arr[i];
	}

	void add(T x){
		arr[size++] = x;
		if(size == capacity) expand();
	}

	T& operator[](int index){
		return arr[index];
	}

	T& get(int index){
		return arr[index];
	}

	int operator()(){
		return size;
	}

	~Array(){
		//std::cout << "Deleting Array\n";
		delete[] arr;
		//std::cout << "Done with Array\n";
	}
};

#endif
