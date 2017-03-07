/** A basic hash map used for variables and functions 
  * uses bitwise operations on chars to keep track of 
  * which buckets are in use */

#ifndef MAP_H
#define MAP_H

#include "Hashes.h"
#include "Array.h"
#include <iostream>
#include <string>

template <typename K, typename V>
class Map{
	class Node{
	public:
		K key;
		V val;
		Node *next = nullptr;

		Node(){}

		Node(K Key, V Val): key(Key), val(Val) {}

		Node(Node &other){
			key = other.key;
			val = other.val;
			if(other.next) next = new Node(*(other.next));
		}

		int add(K k, V v){
			if(key == k) return 0;
			else if(next) return next->add(k, v);
			next = new Node(k, v);
			return 1;
		}

		void set(K k, V v){
			key = k;
			val = v;
		}

		bool contains(K k){
			if(key == k) return true;
			else if(next) return next->contains(k);
			else return false;
		}

		V* get(K k){
			if(key == k) return &val;
			else if(next) return next->get(k);
			else return nullptr;
		}

		void toArray(Array<V> &arr){
			arr.add(val);
			if(next) next->toArray(arr);
		}

		~Node(){
			delete next;
		}
	};

	void addFirst(K key, V val){
		int index = Hashes::hash(key)%width;
		if(active[index/8]&(1<<index%8)){
			Node *temp = new Node(buckets[index]);
			buckets[index].set(key, val);
			buckets[index].next = temp;
		}
		else{
			active[index/8] |= 1<<index%8;
			buckets[index].set(key, val);
		}
	}

	void expand(){
		Node *old = buckets;
		char *oldActive = active;
		int oldWidth = width;
		width*=2;
		buckets = new Node[width];
		active = new char[(int)width/8];
		for(int i=0; i<width/8; ++i) active[i] = 0;
		Node *temp = nullptr;
		for(int i=0; i<oldWidth; ++i){
			if(oldActive[i/8]&(1<<i%8)){
				temp = &old[i];
				while(temp){
					if(&temp->key && &temp->val) addFirst(temp->key, temp->val);
					temp = temp->next;
				}
			}
		}
		delete[] old;
		delete[] oldActive;
	}

	Node *buckets;
	char *active;
	int width;
	std::string text;

public:

	int size;

	Map(): width(16), size(0){
		buckets = new Node[16];
		active = new char[2] {0, 0};
	}

	Map(std::string Text): width(16), size(0), text(Text){
		buckets = new Node[16];
		active = new char[2] {0, 0};
	}

	/*Map(Map<K, V> &other){
		width = other.width;
		buckets = new Node[width];
		active = new char[(int)width/8];
		size = other.size;
		for(int i=0; i<width/8; ++i) active[i] = other.active[i];
		for(int i=0; i<width; ++i) if(active[i/8]&(i<<i%8)) buckets[i] = other.buckets[i];
	}*/

	void add(K key, V val){
		int index = Hashes::hash(key)%width;
		if(active[(int)index/8]&(1<<index%8)) size += buckets[index].add(key, val);
		else{
			buckets[index].set(key, val);
			active[(int)index/8] |= 1<<index%8;
			++size;
		}
		if(size >= width*0.75) expand();
	}

	bool contains(K key){
		int index = Hashes::hash(key)%width;
		if(!active[(int)index/8]&(1<<index%8)) return false;
		else return buckets[index].contains(key);
	}

	V& get(K key){
		return *buckets[Hashes::hash(key)%width].get(key);
	}

	void toArray(Array<V> &arr){
		for(int i=0; i<width; ++i) if(active[(int)i/8]&(1<<i%8)) buckets[i].toArray(arr);
	}

	~Map(){
		//std::cout << "Deleting Map " << text << std::endl; 
		delete[] buckets;
		//std::cout << "Done with Buckets\n";
		delete[] active;
		//std::cout << "Done with Map\n";
	}
};

#endif
