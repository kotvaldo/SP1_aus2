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
    NehnutelnostTester() : tree(2) {} // 2 dimenzie pre GPS

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

            Nehnutelnost* nehn = new Nehnutelnost(uid, new GPS(x, y));
            tree.insert(nehn, nehn->gps);
            data_list.push_back(nehn);

            if (desc) cout << "Point " << i + 1 << ": " << *nehn << " added successfully" << endl;
        }

        if (desc) treeSizeCheck();
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

    void findDataWithDuplicates(int x, int y) {
        if (tree.size() != 0) {
            GPS query_obj(x, y);
            vector<Nehnutelnost*> results = tree.find(&query_obj);

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
        Nehnutelnost* target = nullptr;

        for (auto it = data_list.begin(); it != data_list.end(); ++it) {
            if ((*it)->uid == id) {
                target = *it;

                if (tree.removeNode(target)) {
                    cout << "Entry deleted successfully." << endl;

                    if (target->gps) {
                        delete target->gps;
                        target->gps = nullptr;
                    }

                    delete target;
                    target = nullptr;
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

    void findAllPropertiesInArea(int x1, int y1, int x2, int y2) {
        GPS pos1(x1, y1);
        GPS pos2(x2, y2);

        vector<Nehnutelnost*> results1 = tree.find(&pos1);
        vector<Nehnutelnost*> results2 = tree.find(&pos2);

        vector<Nehnutelnost*> unique_results;

        for (auto& property1 : results1) {
            bool is_unique = true;
            for (auto& property2 : unique_results) {
                if (property1->equals(*property2)) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) unique_results.push_back(property1);
        }

        for (auto& property2 : results2) {
            bool is_unique = true;
            for (auto& property1 : unique_results) {
                if (property2->equals(*property1)) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) unique_results.push_back(property2);
        }

        if (unique_results.empty()) {
            cout << "No properties found in the specified area." << endl;
        }
        else {
            cout << "Properties found in area between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << "):" << endl;
            for (const auto& property : unique_results) {
                cout << *property << endl; 
            }
        }
    }


    void printTreeNodes() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        vector<Nehnutelnost*> nodes;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Nehnutelnost>* node) {
            nodes.push_back(node->_data);
            });

        std::sort(nodes.begin(), nodes.end(), [](const Nehnutelnost* a, const Nehnutelnost* b) {
            return a->uid < b->uid;
            });

        for (const auto& node : nodes) {
            if (node != nullptr) {
                cout << *node << endl;
            }
        }

        treeSizeCheck();
    }

    void clearStructure() {
        tree.clear();

        for (Nehnutelnost* obj : data_list) {
            delete obj->gps;  
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
