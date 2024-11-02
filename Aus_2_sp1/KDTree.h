#pragma once
#include <iostream>
#include <vector>
#include "Models.h"
#include <functional>
#include <stack>
#include <queue>
#include <functional>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"  // Zelen·
#define RED     "\033[31m"  // »erven·
#define UNKNOWN     "\033[33m"  // »erven·

using namespace std;

template <typename KeyType, typename DataType>
struct KDTreeNode {
	KDTreeNode* parent;
	KDTreeNode* _left;
	KDTreeNode* _right;
	int _level;
	DataType* _data;
	KeyType* _keyPart;

	KDTreeNode(DataType* data, KeyType* keys, int level = 0)
		: _data(data), _keyPart(keys), _level(level), parent(nullptr), _left(nullptr), _right(nullptr) {}
};

template <typename KeyType, typename DataType>
class GeneralKDTree {
	using KDNodeType = KDTreeNode<KeyType, DataType>;
public:
	GeneralKDTree(size_t dim_count);
	~GeneralKDTree();
	void clear();
	vector<DataType*> find(KeyType* keys);
	DataType* insert(DataType* data, KeyType* keys);
	bool removeNode(DataType* data);
	size_t size() const;
	KDNodeType* accessRoot();
	bool hasLeftSon(KDNodeType* node);
	bool hasRightSon(KDNodeType* node);
	bool isLeaf(KDNodeType* node);
	bool isLeftSon(KDNodeType* node, KDNodeType* parent);
	bool isRightSon(KDNodeType* node, KDNodeType* parent);
	KDNodeType* findMaxInLeftSubTree(KDNodeType* parent);
	KDNodeType* findMinInRightSubTree(KDNodeType* parent);
	void inOrderTraversal(std::function<void(KDNodeType*)> func, KDNodeType* startNode = nullptr);
	void levelOrderTraversal(std::function<void(KDNodeType*)> func);
	void reverseLevelOrderTraversal(std::function<void(KDNodeType*)> func);
private:
	int size_;
	KDNodeType* root;
	size_t k;
	KDNodeType* findNodeWithData(DataType* data);
	KDTreeNode<KeyType, DataType>* findNodeInRightSubtreeWithDimension(KDNodeType* node, DataType* data, int target_dimension);
	void reinsertNodesWithSameKey(KDNodeType* node);
	bool removeNodeInRightSubtree(KDNodeType* startNode, DataType* data, int targetDimension);
	vector<KDNodeType*> findCorruptedNodes(KDNodeType* startNode);
};

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::GeneralKDTree(size_t dim_count) {
	if (dim_count < 1) {
		throw invalid_argument("It has to be at least 1D Tree");
	}
	this->k = dim_count;
	this->size_ = 0;
	this->root = nullptr;
}

template<typename KeyType, typename DataType>
inline GeneralKDTree<KeyType, DataType>::~GeneralKDTree() {
	this->clear();
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::clear() {
	if (root == nullptr) return;

	std::stack<KDNodeType*> nodeStack;
	nodeStack.push(root);

	while (!nodeStack.empty()) {
		KDNodeType* current = nodeStack.top();
		nodeStack.pop();

		if (current->_left != nullptr) {
			nodeStack.push(current->_left);
		}

		if (current->_right != nullptr) {
			nodeStack.push(current->_right);
		}

		current->_data = nullptr;
		current->_keyPart = nullptr;
		delete current;
	}

	root = nullptr;
	size_ = 0;
}



template<typename KeyType, typename DataType>
DataType* GeneralKDTree<KeyType, DataType>::insert(DataType* data, KeyType* keys) {
	if (keys == nullptr) {
		throw invalid_argument("Keys cannot be nullptr");
	}
	if (size_ == 0) {
		this->root = new KDNodeType(data, keys, 0); // KoreÚ m· ˙roveÚ 0
		this->size_++;
		return this->root->_data;
	}

	KDNodeType* current = this->root;
	KDNodeType* parent = nullptr;

	int level = 0;
	int current_dimension = 0;

	while (current != nullptr) {
		parent = current;
		if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
			if (current->_left == nullptr) break;
			current = current->_left;
		}
		else {
			if (current->_right == nullptr) break;
			current = current->_right;
		}

		level++;
		current_dimension = level % this->k;
	}

	// Pri vytv·ranÌ novÈho uzla zv˝öime ˙roveÚ o 1
	if (keys->compare(*(parent->_keyPart), current_dimension) <= 0) {
		parent->_left = new KDNodeType(data, keys, level + 1); // Nov˝ uzol m· ˙roveÚ o 1 vyööiu neû rodiË
		current = parent->_left;
	}
	else {
		parent->_right = new KDNodeType(data, keys, level + 1); // Nov˝ uzol m· ˙roveÚ o 1 vyööiu neû rodiË
		current = parent->_right;
	}

	current->parent = parent;
	this->size_++;
	return current->_data;
}


template<typename KeyType, typename DataType>
vector<DataType*> GeneralKDTree<KeyType, DataType>::find(KeyType* keys) {
	vector<DataType*> duplicates;

	KDNodeType* current = this->root;
	int level = 0;

	while (current != nullptr) {
		if (keys->equalsByKeys(*(current->_keyPart))) {
			duplicates.push_back(current->_data);
		}

		int current_dimension = level % this->k;
		if (keys->compare(*(current->_keyPart), current_dimension) <= 0) {
			current = current->_left;
		}
		else {
			current = current->_right;
		}

		level++;
	}

	return duplicates;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::removeNode(DataType* data) {
	std::cout << "Attempting to remove node with data: " << *data << std::endl;
	KDNodeType* node = this->findNodeWithData(data);
	if (node == nullptr) {
		std::cout << RED << "Node not found in the tree." << RESET << std::endl;
		return false;
	}

	bool isLeafAfterComing = isLeaf(node);
	std::stack<KDNodeType*> nodesToDelete;
	std::vector<KDNodeType*> nodesToReinsert; // Zoznam na uzly, ktorÈ sa maj˙ reinserovaù

	nodesToDelete.push(node);

	while (!nodesToDelete.empty()) {
		KDNodeType* currentNode = nodesToDelete.top();
		nodesToDelete.pop();

		std::cout << "Processing node with key: " << *(currentNode->_keyPart) << std::endl;

		if (isLeaf(currentNode)) {
			std::cout << "Node is a leaf, deleting it." << std::endl;
			KDNodeType* parent = currentNode->parent;
			if (parent != nullptr) {
				if (isLeftSon(currentNode, parent)) {
					parent->_left = nullptr;
				}
				else {
					parent->_right = nullptr;
				}
			}
			else {
				this->root = nullptr;
			}
			currentNode->_keyPart = nullptr;
			currentNode->_data = nullptr;
			currentNode->parent = nullptr;
			currentNode->_left = nullptr;
			currentNode->_right = nullptr;
			delete currentNode;
			currentNode = nullptr;
			if (isLeafAfterComing) node = nullptr;
			this->size_--;
			continue;
		}

		if (currentNode->_left != nullptr && currentNode->_right != nullptr) {
			std::cout << "Node has two children, replacing it with min node from right subtree." << std::endl;
			KDNodeType* minNode = findMinInRightSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(minNode->_keyPart) << std::endl;
			currentNode->_keyPart = minNode->_keyPart;
			currentNode->_data = minNode->_data;
			nodesToDelete.push(minNode);

			nodesToReinsert.push_back(minNode);
		}
		else if (currentNode->_right != nullptr) {
			std::cout << "Node has only right child, replacing it with min node from right subtree." << std::endl;
			KDNodeType* minNode = findMinInRightSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(minNode->_keyPart) << std::endl;
			currentNode->_keyPart = minNode->_keyPart;
			currentNode->_data = minNode->_data;
			nodesToDelete.push(minNode);


			nodesToReinsert.push_back(minNode);
		}
		else if (currentNode->_left != nullptr) {
			std::cout << "Node has only left child, replacing it with max node from left subtree." << std::endl;
			KDNodeType* maxNode = findMaxInLeftSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(maxNode->_keyPart) << std::endl;
			currentNode->_keyPart = maxNode->_keyPart;
			currentNode->_data = maxNode->_data;
			nodesToDelete.push(maxNode);


			nodesToReinsert.push_back(maxNode);
		}
	}

	if (!nodesToReinsert.empty()) {
		nodesToReinsert.pop_back();
	}
	if (!isLeafAfterComing) {
		reinsertNodesWithSameKey(node);


		/*for (auto it = nodesToReinsert.begin(); it != nodesToReinsert.end(); ++it) {
			std::cout << "Reinserting nodes with the same key after removal for node with key: "
				<< *(*it)->_keyPart << std::endl;
			reinsertNodesWithSameKey(*it);
		}*/


		std::cout << "Reinserting nodes with the same key after removal for original node." << std::endl;
	}
	else {
		std::cout << "Skipping reinsertion as node was initially a leaf." << std::endl;
	}

	std::cout << GREEN << "Node removal completed." << RESET << std::endl;
	return true;
}




template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::accessRoot() {
	if (this->root == nullptr) {
		throw out_of_range("Structure is empty");
	}
	return this->root;
}

template<typename KeyType, typename DataType>
inline void GeneralKDTree<KeyType, DataType>::inOrderTraversal(std::function<void(KDNodeType*)> func, KDNodeType* startNode) {
	if (startNode == nullptr) {
		startNode = root;
	}

	if (startNode == nullptr) return;

	std::stack<KDNodeType*> nodeStack;
	KDNodeType* current = startNode;

	while (!nodeStack.empty() || current != nullptr) {
		while (current != nullptr) {
			nodeStack.push(current);
			current = current->_left;
		}

		current = nodeStack.top();
		nodeStack.pop();

		func(current);

		current = current->_right;
	}
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::levelOrderTraversal(std::function<void(KDNodeType*)> func) {
	if (this->root == nullptr) return;

	std::queue<KDNodeType*> nodeQueue;
	nodeQueue.push(this->root);

	while (!nodeQueue.empty()) {
		KDNodeType* current = nodeQueue.front();
		nodeQueue.pop();

		func(current);

		if (current->_left != nullptr) {
			nodeQueue.push(current->_left);
		}
		if (current->_right != nullptr) {
			nodeQueue.push(current->_right);
		}
	}
}

template<typename KeyType, typename DataType>
void GeneralKDTree<KeyType, DataType>::reverseLevelOrderTraversal(std::function<void(KDNodeType*)> func) {
	if (this->root == nullptr) return;

	std::queue<KDNodeType*> nodeQueue;
	std::stack<KDNodeType*> nodeStack;
	nodeQueue.push(this->root);

	while (!nodeQueue.empty()) {
		KDNodeType* current = nodeQueue.front();
		nodeQueue.pop();

		nodeStack.push(current);

		if (current->_right != nullptr) {
			nodeQueue.push(current->_right);
		}
		if (current->_left != nullptr) {
			nodeQueue.push(current->_left);
		}
	}


	while (!nodeStack.empty()) {
		KDNodeType* current = nodeStack.top();
		nodeStack.pop();
		func(current);
	}
}



template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findNodeWithData(DataType* data) {
	if (this->size_ == 0) return nullptr;

	KDNodeType* current = this->root;

	while (current != nullptr) {

		if (current->_data->equals(*data)) {
			return current;
		}

		int curr_dim = current->_level % this->k;


		int comparison_result = data->compare(*(current->_data), curr_dim);

		if (comparison_result <= 0) {
			current = current->_left;
		}
		else {
			current = current->_right;
		}

	}
	return nullptr;
}


template<typename KeyType, typename DataType>
KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findNodeInRightSubtreeWithDimension(KDNodeType* node, DataType* data, int target_dimension) {
	if (node == nullptr || node->_right == nullptr) {
		return nullptr;
	}

	std::stack<KDNodeType*> nodesToVisit;
	nodesToVisit.push(node->_right);

	while (!nodesToVisit.empty()) {
		KDNodeType* currentNode = nodesToVisit.top();
		nodesToVisit.pop();

		if (currentNode->_data->equals(*data)) {
			return currentNode;
		}

		int current_dimension = currentNode->_level % this->k;

		if (current_dimension != target_dimension) {
			if (currentNode->_left != nullptr) {
				nodesToVisit.push(currentNode->_left);
			}
			if (currentNode->_right != nullptr) {
				nodesToVisit.push(currentNode->_right);
			}
		}
		else {
			if (currentNode->_left != nullptr) {
				nodesToVisit.push(currentNode->_left);
			}

		}
	}

	return nullptr;
}


template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findMaxInLeftSubTree(KDNodeType* parent) {
	if (parent == nullptr || parent->_left == nullptr) {
		std::cout << "Left subtree is empty; no max node found." << std::endl;
		return nullptr;
	}

	KDNodeType* current = parent->_left;
	KDNodeType* maxNode = current;
	int target_dimension = parent->_level % this->k;

	std::stack<KDNodeType*> nodesToVisit;
	nodesToVisit.push(current);

	std::cout << "Starting search for max in left subtree. Initial node key: " << *(current->_keyPart)
		<< ", dimension to compare: " << target_dimension << std::endl;

	while (!nodesToVisit.empty()) {
		KDNodeType* node = nodesToVisit.top();
		nodesToVisit.pop();

		std::cout << "Visiting node with key: " << *(node->_keyPart)
			<< ", level: " << node->_level
			<< ", current dimension: " << (node->_level % this->k) << std::endl;


		if (node->_keyPart->compare(*(maxNode->_keyPart), target_dimension) >= 0) {
			std::cout << "Updating max node to key: " << *(node->_keyPart) << std::endl;
			maxNode = node;
		}


		int current_dimension = node->_level % this->k;
		if (current_dimension != target_dimension) {

			if (node->_left != nullptr) {
				std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack." << std::endl;
				nodesToVisit.push(node->_left);
			}
			if (node->_right != nullptr) {
				std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack." << std::endl;
				nodesToVisit.push(node->_right);
			}
		}
		else {

			if (node->_right != nullptr) {
				std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack (target dimension)." << std::endl;
				nodesToVisit.push(node->_right);
			}
		}
	}

	std::cout << "Max node in left subtree found with key: " << *(maxNode->_keyPart) << std::endl;
	return maxNode;
}




template<typename KeyType, typename DataType>
inline KDTreeNode<KeyType, DataType>* GeneralKDTree<KeyType, DataType>::findMinInRightSubTree(KDNodeType* parent) {
	if (parent == nullptr || parent->_right == nullptr) {
		std::cout << "Right subtree is empty; no min node found." << std::endl;
		return nullptr;
	}

	KDNodeType* current = parent->_right;
	KDNodeType* minNode = current;
	int target_dimension = parent->_level % this->k;

	std::stack<KDNodeType*> nodesToVisit;
	nodesToVisit.push(current);

	std::cout << "Starting search for min in right subtree. Initial node key: " << *(current->_keyPart)
		<< ", dimension to compare: " << target_dimension << std::endl;

	while (!nodesToVisit.empty()) {
		KDNodeType* node = nodesToVisit.top();
		nodesToVisit.pop();

		std::cout << "Visiting node with key: " << *(node->_keyPart)
			<< ", level: " << node->_level
			<< ", current dimension: " << (node->_level % this->k) << std::endl;


		if (node->_keyPart->compare(*(minNode->_keyPart), target_dimension) <= 0) {
			std::cout << "Updating min node to key: " << *(node->_keyPart) << std::endl;
			minNode = node;
		}
		int current_dimension = node->_level % this->k;
		if (current_dimension != target_dimension) {


			if (node->_left != nullptr) {
				std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack." << std::endl;
				nodesToVisit.push(node->_left);
			}
			if (node->_right != nullptr) {
				std::cout << "Pushing right child with key: " << *(node->_right->_keyPart) << " onto stack." << std::endl;
				nodesToVisit.push(node->_right);
			}
		}
		else {

			if (node->_left != nullptr) {
				std::cout << "Pushing left child with key: " << *(node->_left->_keyPart) << " onto stack (target dimension)." << std::endl;
				nodesToVisit.push(node->_left);
			}
		}
	}

	std::cout << GREEN << "Min node in right subtree found with key: " << *(minNode->_keyPart) << RESET << std::endl;
	return minNode;
}


template<typename KeyType, typename DataType>
inline void GeneralKDTree<KeyType, DataType>::reinsertNodesWithSameKey(KDNodeType* node) {
	int target_dimension = node->_level % this->k;

	vector<KDNodeType*> nodes = findCorruptedNodes(node);
	vector<pair<DataType*, KeyType*>> nodesToReinsert;
	if (!nodes.empty()) {

		for (KDNodeType* k : nodes) {
			nodesToReinsert.emplace_back(k->_data, k->_keyPart);
		}
		for (auto it = nodesToReinsert.begin(); it != nodesToReinsert.end(); ++it) {
			const auto& [data, keyPart] = *it;
			std::cout << "Attempting to remove node with data: " << *data << std::endl;

			bool removed = removeNodeInRightSubtree(node, data, target_dimension);
			if (removed) {
			}
			else {
				std::cout << "Node with data " << *data << " not found in the tree while reinsert cycle " << std::endl;
			}
		}


		for (const auto& [data, keyPart] : nodesToReinsert) {

			if(findNodeWithData(data) == nullptr)
			this->insert(data, keyPart);
		}

		std::cout << "Nodes to reinsert: " << std::endl;
		for (const auto& [data, keyPart] : nodesToReinsert) {
			std::cout << "Data: " << *data << ", KeyPart: " << *keyPart << std::endl;
		}
	}
	
}

template<typename KeyType, typename DataType>
inline vector<KDTreeNode<KeyType, DataType>*> GeneralKDTree<KeyType, DataType>::findCorruptedNodes(KDNodeType* startNode)
{
	std::vector<KDNodeType*> nodesToReinsert;
	if (startNode == nullptr || startNode->_right == nullptr) {
		std::cout << "No right subtree for reinsertion." << std::endl;
		return nodesToReinsert;
	}

	KeyType* target_key = startNode->_keyPart;
	int target_dimension = startNode->_level % this->k;

	std::stack<KDNodeType*> nodesToVisit;
	nodesToVisit.push(startNode->_right);

	while (!nodesToVisit.empty()) {
		KDNodeType* currentNode = nodesToVisit.top();
		nodesToVisit.pop();

		if (currentNode->_keyPart->compare(*target_key, target_dimension) <= 0) {
			nodesToReinsert.emplace_back(currentNode);
		}

		if (currentNode->_level % this->k != target_dimension) {
			if (currentNode->_left != nullptr) {
				nodesToVisit.push(currentNode->_left);
			}
			if (currentNode->_right != nullptr) {
				nodesToVisit.push(currentNode->_right);
			}
		}
		else {
			if (currentNode->_left != nullptr) {
				nodesToVisit.push(currentNode->_left);
			}
		}
	}
	return nodesToReinsert;
}


template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::removeNodeInRightSubtree(KDNodeType* startNode, DataType* data, int targetDimension) {
	KDNodeType* node = this->findNodeInRightSubtreeWithDimension(startNode, data, targetDimension);
	if (node == nullptr) {
		return false;
	}

	bool isLeafAfterComing = isLeaf(node);
	std::stack<KDNodeType*> nodesToDelete;
	std::vector<KDNodeType*> nodesToReinsert; // Zoznam na uzly, ktorÈ sa maj˙ reinserovaù

	nodesToDelete.push(node);

	while (!nodesToDelete.empty()) {
		KDNodeType* currentNode = nodesToDelete.top();
		nodesToDelete.pop();

		std::cout << "Processing node with key: " << *(currentNode->_keyPart) << std::endl;

		if (isLeaf(currentNode)) {
			std::cout << "Node is a leaf, deleting it." << std::endl;
			KDNodeType* parent = currentNode->parent;
			if (parent != nullptr) {
				if (isLeftSon(currentNode, parent)) {
					parent->_left = nullptr;
				}
				else {
					parent->_right = nullptr;
				}
			}
			else {
				this->root = nullptr;
			}
			currentNode->_keyPart = nullptr;
			currentNode->_data = nullptr;
			currentNode->parent = nullptr;
			currentNode->_left = nullptr;
			currentNode->_right = nullptr;
			delete currentNode;
			currentNode = nullptr;
			if (isLeafAfterComing) node = nullptr;
			this->size_--;
			continue;
		}

		if (currentNode->_left != nullptr && currentNode->_right != nullptr) {
			std::cout << "Node has two children, replacing it with min node from right subtree." << std::endl;
			KDNodeType* minNode = findMinInRightSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(minNode->_keyPart) << std::endl;
			currentNode->_keyPart = minNode->_keyPart;
			currentNode->_data = minNode->_data;
			nodesToDelete.push(minNode);

			nodesToReinsert.push_back(minNode);
		}
		else if (currentNode->_right != nullptr) {
			std::cout << "Node has only right child, replacing it with min node from right subtree." << std::endl;
			KDNodeType* minNode = findMinInRightSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(minNode->_keyPart) << std::endl;
			currentNode->_keyPart = minNode->_keyPart;
			currentNode->_data = minNode->_data;
			nodesToDelete.push(minNode);


			nodesToReinsert.push_back(minNode);
		}
		else if (currentNode->_left != nullptr) {
			std::cout << "Node has only left child, replacing it with max node from left subtree." << std::endl;
			KDNodeType* maxNode = findMaxInLeftSubTree(currentNode);
			std::cout << "Replacing node with key " << *(currentNode->_keyPart)
				<< " with node having key " << *(maxNode->_keyPart) << std::endl;
			currentNode->_keyPart = maxNode->_keyPart;
			currentNode->_data = maxNode->_data;
			nodesToDelete.push(maxNode);


			nodesToReinsert.push_back(maxNode);
		}
	}

	if (!nodesToReinsert.empty()) {
		nodesToReinsert.pop_back();
	}
	if (!isLeafAfterComing) {
		reinsertNodesWithSameKey(node);

		std::cout << "Reinserting nodes with the same key after removal for original node." << std::endl;
	}
	else {
		std::cout << "Skipping reinsertion as node was initially a leaf." << std::endl;
	}

	std::cout << GREEN << "Node removal completed." << RESET << std::endl;
	return true;
}





template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasLeftSon(KDNodeType* node) {
	if (node == nullptr) {
		return false;
	}
	return node->_left != nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::hasRightSon(KDNodeType* node) {
	if (node == nullptr) {
		return false;
	}
	return node->_right != nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeaf(KDNodeType* node) {
	if (node == nullptr) {
		return false;
	}

	return node->_right == nullptr && node->_left == nullptr;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isLeftSon(KDNodeType* node, KDNodeType* parent) {
	return hasLeftSon(parent) && parent->_left == node;
}

template<typename KeyType, typename DataType>
inline bool GeneralKDTree<KeyType, DataType>::isRightSon(KDNodeType* node, KDNodeType* parent) {
	return hasRightSon(parent) && parent->_right == node;
}

template<typename KeyType, typename DataType>
size_t GeneralKDTree<KeyType, DataType>::size() const {
	return size_;
}
