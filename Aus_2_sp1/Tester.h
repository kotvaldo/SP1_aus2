#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include "Models.h"
#include "KDTree.h"
#include <unordered_set>
using namespace std;
#define RESET   "\033[0m"
#define GREEN   "\033[32m"  // Zelená
#define RED     "\033[31m"  // Červená
#define UNKNOWN     "\033[33m"


template <typename TestClass>
class Tester {
private:
	vector<TestClass*> data_list;
	GeneralKDTree<TestClass, TestClass> tree;
	vector<int> uid_list;

public:
	Tester() : tree(4) {} 

	void genPoints(int num_points, int range_min, int range_max, unsigned int seed = 0, bool desc = false) {
		if (seed == 0) {
			seed = static_cast<unsigned int>(time(0));
		}
		mt19937 gen(seed);
		uniform_int_distribution<> dis(range_min, range_max);
		uniform_real_distribution<> dis_double(static_cast<double>(range_min), static_cast<double>(range_max));

		if (desc) cout << "Using seed: " << seed << endl;

		for (int i = 0; i < num_points; ++i) {
			int uid = this->getUnicateId();
			double A = static_cast<double>(dis(gen));
			string B = "Name" + std::to_string(dis(gen) % 100); 
			int C = dis(gen);
			double D = static_cast<double>(dis(gen));

			TestClass* test_obj = new TestClass(uid, A, B, C, D);
			tree.insert(test_obj, test_obj);
			data_list.push_back(test_obj);

			if (desc) cout << "Point " << i + 1 << ": " << *test_obj << " added successfully" << endl;
		}

		if (desc) treeSizeCheck();
	}




	void testSeeds(int min_seed, int max_seed) {
		for (int seed = min_seed; seed <= max_seed; ++seed) {
			cout << "Testing seed: " << seed << endl;

			mt19937 gen(seed);
			uniform_int_distribution<> dis(0, 100);
			uniform_real_distribution<> dis_double(0.0, 100.0);

			for (int i = 0; i < 10; ++i) {
				int uid = this->getUnicateId();
				double A = dis_double(gen);
				string B = "Test" + to_string(dis(gen) % 50);
				int C = dis(gen);
				double D = dis_double(gen);

				TestClass* test_obj = new TestClass(uid, A, B, C, D);
				tree.insert(test_obj, test_obj);
			}
			treeSizeCheck();
		}
	}

	void oscilate(int count) {
		for (int i = 1; i <= count; ++i) {
			cout << "Oscillation " << i << " in progress..." << endl;
			genPoints(10, 0, 100, 0);

			treeSizeCheck();

			clearStructure();
			cout << "Oscillation " << i << " complete." << endl;
		}
	}

	void treeSizeCheck() {
		int realSize = 0;
		tree.inOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
			realSize++;
			});

		if (tree.size() == realSize) {
			cout << GREEN << "Tree size verification successful: " << realSize << " nodes in tree." << RESET << endl;
		}
		else {
			cout << "Tree size mismatch! Actual: " << realSize << " vs Expected: " << tree.size() << endl;
		}
	}

	void treeSizeCheck(int count) {
		int realSize = 0;
		tree.inOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
			realSize++;
			});

		if (count == realSize) {
			cout << UNKNOWN << "Tree size verification successful: " << realSize << " nodes in tree." << RESET << endl;
		}
		else {
			cout << "Tree size mismatch! Actual: " << realSize << " vs Expected: " << tree.size() << endl;
		}
	}

	void findDataWithDuplicates(double A, string B, int C, double D) {
		if (tree.size() != 0) {
			TestClass query_obj(-1, A, B, C, D); 
			vector<TestClass*> results = tree.find(&query_obj);

			if (results.empty()) {
				cout << "No data found for specified attributes." << endl;
			}
			else {
				cout << "Found " << results.size() << " entries with specified attributes:" << endl;
				for (auto d : results) {
					cout << *d << endl;
				}
			}
		}
		else {
			cout << "Tree is empty. Please generate data first." << endl;
		}
	}



	void deleteTestWithParams(int id) {
		TestClass* target = nullptr;

		for (auto it = data_list.begin(); it != data_list.end(); ++it) {
			if ((*it)->uid == id) {
				target = *it;

				if (tree.removeNode(target)) {
					cout << "Entry deleted successfully." << endl;
					data_list.erase(it);
				}
				else {
					cout << "Failed to delete the entry from the tree." << endl;
				}
				break;
			}
		}

		if (target == nullptr) {
			cout << "No entry found with the given ID: " << id << endl;
		}
	}

	void deleteAll() {
		while (tree.size() != 0) { 
			auto root = tree.accessRoot(); 

			if (tree.removeNode(root->_data)) {
				cout << "Root deleted successfully." << endl;
				
			}
			else {
				cout << "Failed to delete the root node." << endl;
				break; 
			}

			treeSizeCheck();
		}

		cout << "All nodes have been deleted." << endl;
	}


	void deleteRandomNodes(int count) {
		if (count > data_list.size()) {
			cout << "Počet uzlov na vymazanie je väčší ako počet dostupných uzlov." << endl;
			return;
		}

		std::random_device rd;
		std::mt19937 gen(rd());

		for (int i = 0; i < count; ++i) {
			std::uniform_int_distribution<> dis(0, data_list.size() - 1);
			int randomIndex = dis(gen);

			auto it = data_list.begin();
			std::advance(it, randomIndex);

			auto target = *it;

			if (tree.removeNode(target)) {
				cout << "Node with key " << target->uid << " deleted successfully." << endl;
				data_list.erase(it);
				treeSizeCheck();
			}
			else {
				cout << "Failed to delete node with key " << target->uid << endl;
			}
		}

		cout << count << " random nodes have been deleted." << endl;
	}


	void printTreeNodes() {
		if (tree.size() == 0) {
			cout << "The tree is empty." << endl;
			return;
		}

		std::string colors[] = { "\033[31m", "\033[32m", "\033[0m" };  // RED, GREEN, RESET

		tree.levelOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
			if (node != nullptr && node->_data != nullptr) {
				std::string color = colors[node->_level % 3];  
				cout << color << "Level " << node->_level << " - Key: " << *(node->_keyPart)
					<< ", Data: " << *(node->_data) << RESET << endl;
			}
			});
		cout << colors[2];
		treeSizeCheck();
	}


	void clearStructure() {
		tree.clear();
		for (TestClass* obj : data_list) {
			delete obj;
		}

		uid_list.clear();
		data_list.clear();
		cout << "All records removed! Current tree size: " << tree.size() << endl;
	}

	~Tester() {
		clearStructure();
	}

	void checkReferences() {
		if (tree.size() == 0) {
			cout << "The tree is empty." << endl;
			return;
		}

		bool isConsistent = true; 

		tree.levelOrderTraversal([&](KDTreeNode<TestClass, TestClass>* node) {
			if (node != nullptr) {
				
				cout << "Checking node with key: " << *(node->_keyPart) << ", Level: " << node->_level << endl;

				if (node->_right != nullptr) {
					if (node->_right->parent != node) {
						cout << "Inconsistency detected at node with key: " << *(node->_keyPart) << " (Level " << node->_level << ") - "
							<< "Right child's parent does not match current node." << endl;
						cout << "Right child's key: " << *(node->_right->_keyPart) << ", Right child's parent key: "
							<< *(node->_right->parent->_keyPart) << endl;
						isConsistent = false;
					}
				}

				if (node->_left != nullptr) {
					if (node->_left->parent != node) {
						cout << "Inconsistency detected at node with key: " << *(node->_keyPart) << " (Level " << node->_level << ") - "
							<< "Left child's parent does not match current node." << endl;
						cout << "Left child's key: " << *(node->_left->_keyPart) << ", Left child's parent key: "
							<< *(node->_left->parent->_keyPart) << endl;
						isConsistent = false;
					}
				}
			}
			});

		if (isConsistent) {
			cout << "All references are consistent." << endl;
		}
	}

	void randomOperations(int num_operations) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> prob_dist(0.0, 1.0);
		std::uniform_int_distribution<> range_dist(0, 100);

		int internalCount = tree.size(); // Začiatočný počet uzlov v strome

		for (int i = 0; i < num_operations; ++i) {
			double prob = prob_dist(gen);

			if (prob < 0.5) { // Insert operation
				int uid = getUnicateId();
				double A = range_dist(gen);
				std::string B = "Name" + std::to_string(range_dist(gen) % 100);
				int C = range_dist(gen);
				double D = range_dist(gen);
				TestClass* test_obj = new TestClass(uid, A, B, C, D);
				tree.insert(test_obj, test_obj);
				data_list.push_back(test_obj);
				internalCount++; 
				std::cout << "Inserted: " << *test_obj << std::endl;
			}
			else if (prob < 0.75) { 
				if (data_list.empty()) {
					std::cout << "No data to find." << std::endl;
					continue;
				}
				int randomIndex = range_dist(gen) % data_list.size();
				TestClass* target = data_list[randomIndex];
				std::cout << "Finding: " << *target << std::endl;
				std::vector<TestClass*> results = tree.find(target);
				if (!results.empty()) {
					std::cout << "Found: " << results.size() << " matches." << std::endl;
				}
				else {
					std::cout << "No match found." << std::endl;
				}
			}
			else { 
				if (data_list.empty()) {
					std::cout << "No data to delete." << std::endl;
					continue;
				}
				int randomIndex = range_dist(gen) % data_list.size();
				auto it = data_list.begin() + randomIndex;
				TestClass* target = *it;
				std::cout << "Deleting: " << *target << std::endl;
				if (tree.removeNode(target)) {
					data_list.erase(it);
					internalCount--; 
					std::cout << "Deleted successfully." << std::endl;
				}
				else {
					std::cout << "Failed to delete." << std::endl;
				}
			}

			treeSizeCheck(internalCount);
		}
	}


	int getUnicateId() {
		if (uid_list.empty()) {
			uid_list.push_back(1);
			return 1;
		}
		uid_list.push_back(uid_list.size() + 1);
		return uid_list.size();
	}
};