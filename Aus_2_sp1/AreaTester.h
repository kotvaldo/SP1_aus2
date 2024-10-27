#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include "Models.h"
#include "KDTree.h"

using namespace std;

class AreaTester {
private:
    vector<Area*> data_list;
    GeneralKDTree<GPS, Area> tree;
    vector<int> uid_list;

public:
    AreaTester() : tree(2) {} // 2 dimenzie pre GPS

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

            GPS* shared_gps = new GPS(x, y);

            Nehnutelnost* nehnutelnost = new Nehnutelnost(uid, shared_gps);
            Parcela* parcela = new Parcela(uid, shared_gps);

            Area* area = new Area(uid, shared_gps, nehnutelnost, parcela);


            tree.insert(area, area->gps);
            data_list.push_back(area);

            if (desc) cout << "Point " << i + 1 << ": " << *area << " added successfully" << endl;
        }

        if (desc) treeSizeCheck();
    }
    void treeSizeCheck() {
        int realSize = 0;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Area>* node) {
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
            vector<Area*> results = tree.find(&query_obj);

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

    void findAllAreasInRegion(int x1, int y1, int x2, int y2) {
        GPS pos1(x1, y1);
        GPS pos2(x2, y2);

        vector<Area*> results1 = tree.find(&pos1);
        vector<Area*> results2 = tree.find(&pos2);

        vector<Area*> unique_results;

        for (auto& area1 : results1) {
            bool is_unique = true;
            for (auto& area2 : unique_results) {
                if (area1->equals(*area2)) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) unique_results.push_back(area1);
        }

        for (auto& area2 : results2) {
            bool is_unique = true;
            for (auto& area1 : unique_results) {
                if (area2->equals(*area1)) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) unique_results.push_back(area2);
        }

        if (unique_results.empty()) {
            cout << "No areas found in the specified region." << endl;
        }
        else {
            cout << "Areas found in region between (" << x1 << ", " << y1 << ") and (" << x2 << ", " << y2 << "):" << endl;
            for (const auto& area : unique_results) {
                cout << *area << endl; // Použitie operator<< pre výpis objektu
            }
        }
    }

    void deleteTestWithParams(int id) {
        Area* target = nullptr;

        for (auto it = data_list.begin(); it != data_list.end(); ++it) {
            if ((*it)->uid == id) {
                target = *it;

                if (tree.removeNode(target)) {
                    cout << "Entry deleted successfully." << endl;

                    if (target->gps) {
                        delete target->gps;
                        target->gps = nullptr;
                    }
                    if (target->parcela) {
                        if (target->parcela->gps) {
                            delete target->parcela->gps;
                            target->parcela->gps = nullptr;
                        }
                        delete target->parcela;
                        target->parcela = nullptr;
                    }
                    if (target->nehnutelnost) {
                        if (target->nehnutelnost->gps) {
                            delete target->nehnutelnost->gps;
                            target->nehnutelnost->gps = nullptr;
                        }
                        delete target->nehnutelnost;
                        target->nehnutelnost = nullptr;
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



    void printTreeNodes() {
        if (tree.size() == 0) {
            cout << "The tree is empty." << endl;
            return;
        }

        vector<Area*> nodes;
        tree.inOrderTraversal([&](KDTreeNode<GPS, Area>* node) {
            nodes.push_back(node->_data);
            });

        std::sort(nodes.begin(), nodes.end(), [](const Area* a, const Area* b) {
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

        for (Area* obj : data_list) {
            if (obj->gps) delete obj->gps;
            if (obj->parcela) {
                if (obj->parcela->gps) delete obj->parcela->gps;
                delete obj->parcela;
            }
            if (obj->nehnutelnost) {
                if (obj->nehnutelnost->gps) delete obj->nehnutelnost->gps;
                delete obj->nehnutelnost;
            }
            delete obj;
        }

        uid_list.clear();
        data_list.clear();
        cout << "All records removed! Current tree size: " << tree.size() << endl;
    }


    ~AreaTester() {
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
