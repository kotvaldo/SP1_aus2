#pragma once
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class IComparable {
public:
    virtual int compare(const T& other, int cur_level) const = 0;
    virtual ~IComparable() = default;
};

template <typename KeyType, typename DataType>
struct KDTreeNode {
    KDTreeNode* parent;
    KDTreeNode* _left;
    KDTreeNode* _right;
    size_t _level;
    DataType* _data;
    IComparable<KeyType>* _keys;  //vseobecna implementacia 
    
    KDTreeNode(DataType* data, IComparable<KeyType>* keys, size_t level = 0)
        : data(data), keys(keys), level(level), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename KeyType, typename DataType>
class GeneralKDTree {
    using KDNodeType = KDTreeNode<KeyType, DataType>;
public:
    GeneralKDTree(size_t dim_count);

    void clear();
    KDNodeType* insert(DataType* data, IComparable<KeyType>* keys);
    size_t size() const;

private:
    size_t size_;
    KDNodeType* root;
    size_t k; //pocet dimensii;

};

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t dim_count)
{
    if (k < 1) {
        throw invalid_argument("It has to be at least 1D Tree");
    }
    this->k = dim_count;
    this->size_ = 0;

};

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear() {
    
}

template<typename KeyType, typename DataType>
KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::insert(DataType* data, IComparable<KeyType>* keys)
{
    if (keys == nullptr) {
        throw invalid_argument("Keys cannot be nullptr");
    }
    
    if (size == 0) {
        this->root = new KDNodeType(keys, data, 0);
        this->size_ += 1;
        return this->root;
    }
    
    KDNodeType* current = this->root;
    KDNodeType* parent = nullptr;
    
    size_t level = 0;
    size_t curr_dim = 0; 
    
    while (current != nullptr) {
        if (current->_keys->compare(keys, curr_dim) <= 0) {
            parent = current;
            current = current->_left;
            level++;
            curr_dim = curr_dim % this->k;
        }
        else {
            parent = current;
            current = current->_right;
            level++;
            curr_dim = curr_dim % this->k;
        }

    }
}



template<typename KeyType, typename DataType>
size_t GeneralKDTree<KeyType, DataType>::size() const {
    return size_;
}
