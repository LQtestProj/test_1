#include <memory.h>  
#include <stdio.h>


template<class K, class V>
class Node {
public:
	V value;
	K key;
	int hash; 
};
template<class K,class V>
class Map {
	int miSize;
	Node<K, V>* mTable;



};














