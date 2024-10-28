#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include "Models.h"
#include "KDTree.h"

using namespace std;

class NehnutelnostTester {
private:
    vector<Nehnutelnost*> data_list;
    GeneralKDTree<GPS, Nehnutelnost> tree;
    vector<int> uid_list;

public:
    NehnutelnostTester() : tree(2) {} // Dvojrozmerný KD strom pre x a y ako KeyType s dátami typu Nehnutelnost*

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
            cout << "Tree size verification successful: " << realSize << " nodes in tree." << endl;
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

        vector<Nehnutelnost*> nodes;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            if (node != nullptr && node->_data != nullptr) {
                nodes.push_back(node->_data);
            }
            });

        std::sort(nodes.begin(), nodes.end(), [](const Nehnutelnost* a, const Nehnutelnost* b) {
            return a->uid < b->uid;
            });

        for (const auto& node : nodes) {
            cout << *node << endl;
        }

        treeSizeCheck();
    }

    void insertNode(int x, int y, string name = "") {
        int uid = this->getUnicateId();  

        GPS* gps_point = new GPS(x, y);
        Nehnutelnost* property = new Nehnutelnost(uid, gps_point, name);

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
