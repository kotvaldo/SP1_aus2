#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "GPS.h"
#include "KDTree.h"

using namespace std;

class Tester {
private:
    vector<GPS*> gps_list;
    GeneralKDTree<GPS, GPS> tree;  

public:
    Tester() : tree(2) {}  

    void genPoints(int num_points, int range_min, int range_max, unsigned int seed = 0) {
        if (seed == 0) {
            seed = static_cast<unsigned int>(time(0));
        }
        mt19937 gen(seed);
        uniform_int_distribution<> dis(range_min, range_max);

        cout << "Seed: " << seed << endl;

        for (int i = 0; i < num_points; ++i) {
            int x = dis(gen);
            int y = dis(gen);
            GPS* gps = new GPS(x, y);

            tree.insert(gps, gps);
            gps_list.push_back(gps);

            cout << "GPS point" << i + 1 << ": " << *gps << " successfuly added" << endl;
        }

        cout << "Size of tree: " << tree.size() << endl;
    }

    void clearStructure() {
        for (GPS* gps : gps_list) {
            delete gps;
        }
        gps_list.clear();
    }

    ~Tester() {
        clearStructure();
    }
};