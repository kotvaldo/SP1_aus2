#pragma once
#include <iostream>
#include <vector>
#include "GPS.h"


using namespace std;

template <typename KeyType, typename DataType>
struct KDTreeNode {
    KDTreeNode* parent;
    KDTreeNode* _left;
    KDTreeNode* _right;
    int _level;
    DataType* _data;
    KeyType* _keyPart;  // všeobecná implementácia 

    KDTreeNode(DataType* data, KeyType* keys, int level = 0)
        : _data(data), _keyPart(keys), _level(level), parent(nullptr), _left(nullptr), _right(nullptr) {}
};

template <typename KeyType, typename DataType>
class GeneralKDTree {
    using KDNodeType = KDTreeNode<KeyType, DataType>;
public:
    GeneralKDTree(size_t dim_count);

    void clear();
    KDNodeType* insert(DataType* data, KeyType* keys);
    size_t size() const;
    KDNodeType* accessRoot();

private:
    int size_;
    KDNodeType* root;
    size_t k; // poèet dimenzií
};

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t dim_count)
{
    if (dim_count < 1) {
        throw invalid_argument("It has to be at least 1D Tree");
    }
    this->k = dim_count;
    this->size_ = 0;
    this->root = nullptr;
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear() {

}

template<typename KeyType, typename DataType>
KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::insert(DataType* data, KeyType* keys)
{
    if (keys == nullptr) {
        throw invalid_argument("Keys cannot be nullptr");
    }

    if (size_ == 0) {
        this->root = new KDNodeType(data, keys, 0);
        this->size_++;
        return this->root;
    }

    KDNodeType* current = this->root;
    KDNodeType* parent = nullptr;

    int level = 0;
    int current_dimension = 0;

    while (current != nullptr) {
        parent = current;

        if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
            current = current->_left;
        }
        else {
            current = current->_right;
        }

        level++;
        current_dimension = level % this->k;
    }

    current_dimension = level % this->k;
    if (parent != nullptr) {
        if (keys->compare(*(parent->_keyPart), current_dimension) <= 0) {
            parent->_left = new KDNodeType(data, keys, level);
            current = parent->_left;
        }
        else {
            parent->_right = new KDNodeType(data, keys, level);
            current = parent->_right;
        }

        current->parent = parent;
        this->size_++;
    }
    
    return current;
}

template<typename KeyType, typename DataType>
size_t GeneralKDTree<KeyType, DataType>::size() const {
    return size_;
}

template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::accessRoot()
{
    if (this->root == nullptr) {
        throw out_of_range("Structure is empty");
    }
    return this->root;
}
