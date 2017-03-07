#ifndef STACK_H
#define STACK_H

#include <iostream>

template <typename T>
class Stack{
	class Node{
	public:
		T element;
		Node *next = nullptr;
		Node(){}
		Node(T x): element(x){}
		Node(T x, Node *n): element(x), next(n) {}

		Node(Node &other){
			element = other.element;
			if(other.next) next = new Node(*(other.next));
		}

		void Delete(){
			if(next) next->Delete();
			delete next;
		}
	};

public:

	int size;
	Node *first = nullptr;

	Stack(): size(0) {};

	Stack(Stack<T> &other){
		size = other.size;
		if(other.first) first = new Node(*(other.first));
	}

	void push(T x){
		if(first){
			Node *temp = first;
			first = new Node(x, temp);
		}
		else first = new Node(x);
		++size;
	}

	T peek(){
		return first->element;
	}

	T pop(){
		if(!first) return (T)0; 
		T el = first->element;
		Node *temp = first;
		first = first->next;
		delete temp;
		--size;
		return el;
	}

	~Stack(){
		//std::cout << "Deleting stack\n";
		if(first) first->Delete();
		delete first;
		//std::cout << "Done with Stack\n";
	}
};

#endif