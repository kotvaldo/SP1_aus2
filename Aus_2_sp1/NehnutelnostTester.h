#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include "Models.h"
#include "KDTree.h"
#define UNKNOWN     "\033[33m"
using namespace std;

class NehnutelnostTester {
private:
    vector<Nehnutelnost*> data_list;
    GeneralKDTree<GPS, Nehnutelnost> tree;
    vector<int> uid_list;

public:
    NehnutelnostTester() : tree(2) {} 

    void genPoints(int num_points, int range_min, int range_max, unsigned int seed = 0, bool desc = false) {
        if (seed == 0) {
            seed = static_cast<unsigned int>(time(0));
        }
        mt19937 gen(seed);
        uniform_int_distribution<> dis(range_min, range_max);

        if (desc) cout << "Using seed: " << seed << endl;

        for (int i = 0; i < num_points; ++i) {
            int uid = this->getUnicateId();
            int x = dis(gen);
            int y = dis(gen);

            GPS* gps_point = new GPS(x, y);
            Nehnutelnost* property = new Nehnutelnost(uid, gps_point);
            tree.insert(property, gps_point);
            data_list.push_back(property);

            if (desc) cout << "Point " << i + 1 << ": " << *property << " added successfully" << endl;
        }

        if (desc) treeSizeCheck();
    }

    void testSeeds(int min_seed, int max_seed) {
        for (int seed = min_seed; seed <= max_seed; ++seed) {
            cout << "Testing seed: " << seed << endl;
            mt19937 gen(seed);
            uniform_int_distribution<> dis(0, 100);

            for (int i = 0; i < 10; ++i) {
                int uid = this->getUnicateId();
                int x = dis(gen);
                int y = dis(gen);

                GPS* gps_point = new GPS(x, y);
                Nehnutelnost* property = new Nehnutelnost(uid, gps_point);
                tree.insert(property, gps_point);
                data_list.push_back(property);
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
        tree.inOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            realSize++;
            });

        if (tree.size() == realSize) {
            cout << UNKNOWN << "Tree size verification successful: " << realSize << " nodes in tree." << RESET << endl;
        }
        else {
            cout << "Tree size mismatch! Actual: " << realSize << " vs Expected: " << tree.size() << endl;
        }
    }

    void findDataWithCoordinates(int x, int y) {
        if (tree.size() != 0) {
            GPS query_obj(x, y);
            vector<Nehnutelnost*> results = tree.find(&query_obj);

            if (results.empty()) {
                cout << "No data found for specified coordinates." << endl;
            }
            else {
                cout << "Found " << results.size() << " entries with specified coordinates:" << endl;
                for (auto d : results) {
                    cout << *d << endl;
                }
            }
        }
        else {
            cout << "Tree is empty. Please generate data first." << endl;
        }
    }
    void deleteRandomNodes(int count) {
        if (count > data_list.size()) {
            cout << "Po�et uzlov na vymazanie je v��� ako po�et dostupn�ch uzlov." << endl;
            return;
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i < count; ++i) {
            std::uniform_int_distribution<> dis(0, data_list.size() - 1);
            int randomIndex = dis(gen);

            auto it = data_list.begin();
            std::advance(it, randomIndex);

            Nehnutelnost* target = *it;

            if (tree.removeNode(target)) {
                cout << "Node with key " << target->uid << " deleted successfully." << endl;
                data_list.erase(it);
                delete target;
            }
            else {
                cout << "Failed to delete node with key " << target->uid << endl;
            }


        }

        cout << count << " random nodes have been deleted." << endl;
    }



    void synchronizeDataList() {
        vector<Nehnutelnost*> newDataList;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            if (node && node->_data) {
                newDataList.push_back(node->_data);
            }
            });
        data_list = std::move(newDataList);
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

    void treeSizeCheck(int count) {
        int realSize = 0;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            realSize++;
            });

        if (count == realSize) {
            cout << UNKNOWN << "Tree size verification successful: " << realSize << " nodes in tree." << RESET << endl;
        }
        else {
            cout << "Tree size mismatch! Actual: " << realSize << " vs Expected: " << tree.size() << endl;
        }
    }

    void deleteNodeById(int id) {
        Nehnutelnost* target = nullptr;

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



    void printTreeNodes() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        checkReferences();

        tree.levelOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            if (node != nullptr && node->_data != nullptr) {
                string color;
                string branchIndicator;

                if (node->parent == nullptr) {
                    color = RESET;
                    branchIndicator = "";
                }
                else if (node->parent->_left == node) {
                    color = GREEN;
                    branchIndicator = "|--";
                }
                else if (node->parent->_right == node) {
                    color = RED;
                    branchIndicator = "`--";
                }

                cout << color
                    << string(node->_level * 4, ' ')
                    << branchIndicator << "Key: " << *(node->_data)
                    << RESET << endl;
            }
            });

        treeSizeCheck();
    }


    int treeSize() {
        return tree.size();
    }

    void checkReferences() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        bool isConsistent = true;

        tree.levelOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
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
       
        clearStructure();
        
        
        genPoints(20000, 1, 50);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> prob_dist(0.0, 1.0);  
        std::uniform_int_distribution<> range_dist(1, 50);     

        int internalCount = tree.size();
        int countOfInsert = 0;
        int countOfDelete= 0;
        int countOfFind = 0;

        for (int i = 0; i < num_operations; ++i) {
            double prob = prob_dist(gen);

            if (prob < 0.5) {  
                int uid = getUnicateId();
                int x = range_dist(gen);
                int y = range_dist(gen);

                GPS* gps_point = new GPS(x, y);
                Nehnutelnost* property = new Nehnutelnost(uid, gps_point);
                tree.insert(property, gps_point);
                data_list.push_back(property);

                internalCount++;
                cout << "Inserted: " << *property << endl;
                countOfInsert++;
            }
            else if (prob < 0.75) {  // Find operation
                countOfFind++;
                if (data_list.empty()) {
                    cout << "No data to find." << endl;
                    continue;
                }

                int randomIndex = range_dist(gen) % data_list.size();
                Nehnutelnost* target = data_list[randomIndex];
                cout << "Finding: " << *target << endl;

                vector<Nehnutelnost*> results = tree.find(target->gps);
                if (!results.empty()) {
                    cout << "Found: " << results.size() << " matches." << endl;
                }
                else {
                    cout << "No match found." << endl;
                }
            }
            else {  // Delete operation
                countOfDelete++;
                if (data_list.empty()) {
                    cout << "No data to delete." << endl;
                    continue;
                }

                int randomIndex = range_dist(gen) % data_list.size();
                auto it = data_list.begin() + randomIndex;
                Nehnutelnost* target = *it;

                cout << "Deleting: " << *target << endl;
                if (tree.removeNode(target)) {
                    data_list.erase(it);
                    internalCount--;
                    cout << "Deleted successfully." << endl;
                }
                else {
                    cout << "Failed to delete." << endl;
                }
            }

            
        }
        treeSizeCheck(internalCount);
        std::cout << "Operations count: " << num_operations << endl;
        std::cout << "Insert operations count: " << countOfInsert << endl;
        std::cout << "Find operations count: " << countOfFind << endl;
        std::cout << "Delete operations count: " << countOfDelete << endl;


    }

    void insertNode(int x, int y, string name = "") {
        int uid = this->getUnicateId();

        GPS* gps_point = new GPS(x, y);
        Nehnutelnost* property = new Nehnutelnost(uid, gps_point);

        tree.insert(property, gps_point);
        data_list.push_back(property);

        cout << "Node inserted successfully: " << *property << endl;
    }
    void clearStructure() {
        tree.clear();
        for (Nehnutelnost* obj : data_list) {
            delete obj;
        }

        uid_list.clear();
        data_list.clear();
        cout << "All records removed! Current tree size: " << tree.size() << endl;
    }

    ~NehnutelnostTester() {
        clearStructure();
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