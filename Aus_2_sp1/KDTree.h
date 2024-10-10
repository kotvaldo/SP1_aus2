#pragma once
#include <iostream>
#include <vector>
using namespace std;


template<typename T>
class IComparable {
public:
	virtual int compare(const T& other) const = 0;
	virtual ~IComparable() = default; 
};


template <typename K, typename DataType>
struct KDTreeNode {
	KDTreeNode* parent;
	KDTreeNode* _left;
	KDTreeNode* _right;
	// k - pocet dimenzii
	size_t level;
	DataType* _data;
	IComparable* _keys;

};

template <typename K, typename DataType>
class GeneralKDTree {
	

public:
	using KDNodeType = typename KDTreeNode<K, DataType>;
	void clear();
	void insert(KDNodeType* node);
	size_t size();


private:
	size_t size_;
	KDNodeType* root;

};

template<typename K, typename DataType>
void GeneralKDTree<K, DataType>::clear()
{
}

template<typename K, typename DataType>
void GeneralKDTree<K, DataType>::insert(KDNodeType* node)
{

}
