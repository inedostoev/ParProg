#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include "atomictag.hpp"
#include "node.hpp"
#include <bits/stdc++.h> 


extern int max_level;

int get_number_of_levels() {
    int levels = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    float result = dis(gen);
    while (result < 0.5 && levels < max_level) {
        levels += 1;
        result = dis(gen);
    }
    return levels;
}

template<class T> class SkipList {
public:
	SkipList() {
		head = new atomicNode<T>(INT_MIN, 0);
		tail = new atomicNode<T>(INT_MAX, 0);
		
		for(int i = 0; i <= max_level; i++) {
			head->next_[i].set(tail, false);
		}
	}

	bool search(int n_value, std::vector<atomicNode<T>*>& pred, std::vector<atomicNode<T>*>& after) {
		int bottom_level = 0;
		bool *mark = new bool;
		bool snip;

		atomicNode<T> *a = new atomicNode<T>();
		atomicNode<T> *cur = new atomicNode<T>();
		atomicNode<T> *b = new atomicNode<T>();

        while(true) {
            a = head;
            for(int i = max_level; i >= bottom_level; i--) {
                cur = a->next_[i].getReference();
                while(true) {
                    b = cur->next_[i].get(mark);
                    while(mark[0]) {
                        snip = a->next_[i].CAS(cur, b, false, false);
                        if (!snip) {
                            continue;
                        }
                        cur = a->next_[i].getReference();
			b = cur->next_[i].get(mark);
                    }
                    if (cur->key_ < n_value) {
                        a = cur;
                        cur = b;
                    } else {
                        break;
                    }
                }
                pred[i] = a;
                after[i] = cur;
            }
            return (cur->key_ == n_value);
        }

    }

    bool insert(int key, T element = 0) {
        int level = get_number_of_levels();
        int bottom_level = 0;
        std::vector<atomicNode<T>*> pred(max_level + 1);
        std::vector<atomicNode<T>*> after(max_level + 1);
        bool found = search(key, pred, after);

        while(true) {
            bool found = search(key, pred, after);

            if (found) {
                return false;
            } 

            atomicNode<T> *new_node = new atomicNode<T>(key, element, level);

            for (int i = bottom_level; i <= level; i++) {
                new_node->next_[i].set(after[i], false);
            }

            atomicNode<T> *a = pred[bottom_level];
            atomicNode<T> *b = after[bottom_level];

            if (!a->next_[bottom_level].CAS(b, new_node, false, false)) {
                continue;
            }

            bool *mark = new bool;
            new_node->next_[bottom_level].get(mark);
            for(int i = bottom_level + 1; i <= level; i++) {
                while(true) {
                    a = pred[i];
                    b = after[i];
                    if (a->next_[i].CAS(b, new_node, false, false)) {
                        break;
                    }
                    search(key, pred, after);
                }
            }
            return true;
        }
    }

    bool remove(T x) {
        int bottom_level = 0;

        std::vector<atomicNode<T>*> pred(max_level + 1);
        std::vector<atomicNode<T>*> after(max_level + 1);
        atomicNode<T>* b = nullptr;

        while(true) {
            bool found = search(x, pred, after);
            if (!found) {
                return false;
            }

            atomicNode<T>* node_to_remove = after[bottom_level];
            for (int i = node_to_remove->max_level_; i >= bottom_level + 1; i--) {
                bool *tag = new bool(false);
                b = node_to_remove->next_[i].get(tag);
                while(!tag[0]) {
                    node_to_remove->next_[i].CAS(b, b, false, true);
                    b = node_to_remove->next_[i].get(tag);
                }
            }
            bool *tag = new bool(false);
            b = node_to_remove->next_[bottom_level].get(tag);
            while (true) {
                bool tag_it = node_to_remove->next_[bottom_level].CAS(b, b, false, true);
                b = after[bottom_level]->next_[bottom_level].get(tag);
                if (tag_it) {
                    search(x, pred, after);
                    return true;
                } else if (tag[0]) {
                    return false;
                }
            }
        }
    }

    void print() {
        std::cout << "print skiplist" << std::endl;
        for (int i = max_level; i >= 0; i--) {
            atomicNode<T> *temp = head->next_[i].getReference();
            std::cout << "Level " << i << ":   head -> ";
            while(temp->next_[i].getReference() != NULL)
            {
                std::cout << temp->key_ << " -> ";
                temp = temp->next_[i].getReference();
            }
            std::cout << "tail" << std::endl;
        }
        std::cout << "print end" << std::endl;
    }

private:
    	atomicNode<T> *head;
	atomicNode<T> *tail;


};

#endif
