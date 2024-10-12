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
    KDTreeNode* left;
    KDTreeNode* right;
    size_t level;
    DataType* data;
    IComparable<KeyType>* keys;  //vseobecna implementacia 

    KDTreeNode(DataType* data, IComparable<KeyType>* keys, size_t level = 0)
        : data(data), keys(keys), level(level), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename KeyType, typename DataType>
class GeneralKDTree {
public:
    using KDNodeType = KDTreeNode<KeyType, DataType>;

    GeneralKDTree(size_t);

    void clear();
    KDNodeType* insert(DataType* data, IComparable<KeyType>* keys);
    size_t size() const;

private:
    size_t size_;
    KDNodeType* root;
    size_t k; //pocet dimensii;

};

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t k)
{
    if (k < 1) {
        throw exception()
    }

};

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear() {
    
}



template<typename KeyType, typename DataType>
KDTreeNode* GeneralKDTree<KeyType, DataType>::insert(DataType* data, IComparable<KeyType>* keys) {
    
}


template<typename KeyType, typename DataType>
inline size_t GeneralKDTree<KeyType, DataType>::size() const {
    return size_;
}
