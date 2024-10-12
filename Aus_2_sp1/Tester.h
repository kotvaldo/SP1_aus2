#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "GPS.h"
using namespace std;

class Tester {
private:
    int num_points;
    int range_min;
    int range_max;
    vector<GPS*> gps_list;
    unsigned int seed;

public:
    Tester(int num_points, int range_min, int range_max, unsigned int setSeed = 0)
        : num_points(num_points), range_min(range_min), range_max(range_max), seed(setSeed) {
        if (seed == 0) {
            seed = static_cast<unsigned int>(time(0));
        }
    }

    void run() {
        GeneralKDTree<GPS, GPS> tree(2);

        mt19937 gen(seed);
        uniform_int_distribution<> dis(range_min, range_max);

        cout << "Seed: " << seed << endl;

        for (int i = 0; i < num_points; ++i) {
            int x = dis(gen);
            int y = dis(gen);
            GPS* gps = new GPS(x, y);

            tree.insert(gps, gps);
            gps_list.push_back(gps);

            cout << "Bod " << i + 1 << ": " << *gps << " vložený do KD-stromu a pridaný do zoznamu." << endl;
        }

        cout << "Poèet uzlov v strome: " << tree.size() << endl;
    }

    void clearGPSObjects() {
        for (GPS* gps : gps_list) {
            delete gps;
        }
        gps_list.clear();
    }

    ~Tester() {
        clearGPSObjects();
    }
};