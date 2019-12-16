#ifndef ATOMIC_MARKABLE_REFERENCE_H
#define ATOMIC_MARKABLE_REFERENCE_H

#include <iostream>
#include <vector>
#include <atomic>
#include <random>


template<class T>
class Tag {
public:    
	T *next;
	bool tag;

	Tag();
	Tag(T* node, bool tag);

	bool operator==(const Tag<T> &other) {
		return (next == other.next && tag == other.tag);
	}

};

template<class T>
Tag<T>::Tag():
	next(nullptr),
	tag(false)
{}

template<class T>
Tag<T>::Tag(T* node, bool tag):
	next(node),
	tag(tag)
{}


template<class T>
class AtomicTag {
private:
    std::atomic<Tag<T>*> next_tag;
public:
    AtomicTag();
    AtomicTag(T* next_node, bool tag);
    
    T* getReference();
    T* get(bool *tag);
    
    void set(T* new_ref, bool new_tag);
    bool CAS(T* expected, T* new_value, bool expected_tag, bool new_tag);
};



template<class T>
AtomicTag<T>::AtomicTag() {
        next_tag.store(new Tag<T>(nullptr, false));
}

template<class T>
AtomicTag<T>::AtomicTag(T* next_node, bool tag) {
        next_tag.store(new Tag<T>(next_node, tag));
}

template<class T>
T* AtomicTag<T>::getReference() {
        return next_tag.load()->next;
}

template<class T>
T* AtomicTag<T>::get(bool *tag) {
        Tag<T> *temp = next_tag.load();
        *tag = temp->tag;
        return temp->next;
}

template<class T>
void AtomicTag<T>::set(T* new_ref, bool new_tag) {
        Tag<T> *current = next_tag.load();
        if (new_ref != current->next || new_tag != current->tag) {
            next_tag.store(new Tag<T>(new_ref, new_tag));
        }
}

template<class T>
bool AtomicTag<T>::CAS(T* expected, T* new_value, bool expected_tag, bool new_tag) {
	Tag<T> *curr = next_tag.load();
	return (expected == curr->next && expected_tag == curr->tag && 
		((new_value == curr->next && new_tag == curr->tag) ||
		next_tag.compare_exchange_strong(curr, new Tag<T>(new_value, new_tag))));
}

#endif
