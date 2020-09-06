#ifndef ATOMIC_NODE_H
#define ATOMIC_NODE_H

int max_level = 5;


template<class T>
class atomicNode {
public:
	atomicNode();
	atomicNode(int n_key, T n_value, int height);
	~atomicNode();


	T value_;
	int key_;
	int max_level_;

	std::vector<AtomicTag<atomicNode<T>>> next_;

};

template<class T>
atomicNode<T>::atomicNode() :
	key_(0),
	value_(0),
	max_level_(max_level),
	next_(std::vector<AtomicTag<atomicNode<T>>>(max_level + 1))
{}


template<class T>
atomicNode<T>::atomicNode(int n_key, T n_value, int height = max_level) : 
        key_(n_key),
        value_(n_value),
        max_level_(height),
        next_(std::vector<AtomicTag<atomicNode<T>>>(height + 1))
{}

template<class T>
atomicNode<T>::~atomicNode() {
	key_ = 0;
	value_ = 0;
	max_level_ = 0;
	next_.clear();
} 

#endif
