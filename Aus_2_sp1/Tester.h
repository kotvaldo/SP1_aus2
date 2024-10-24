#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "DataRes.h"
#include "KDTree.h"

using namespace std;

class Tester {
private:
	vector<GPS*> gps_list;
	GeneralKDTree<GPS, GPS> tree;

public:
	Tester() : tree(2) {}

	void genPoints(int num_points, int range_min, int range_max, unsigned int seed = 0, bool desc = false) {
		if (seed == 0) {
			seed = static_cast<unsigned int>(time(0));
		}
		mt19937 gen(seed);
		uniform_int_distribution<> dis(range_min, range_max);

		if(desc) cout << "Seed: " << seed << endl;

		for (int i = 0; i < num_points; ++i) {
			int x = dis(gen);
			int y = dis(gen);
			GPS* gps = new GPS(x, y);

			tree.insert(gps, gps);
			gps_list.push_back(gps);

			if(desc == true) cout <<  i + 1 << ".GPS point " << ": " << *gps << " successfuly added" << endl;
		}

		if(desc == true) treeSizeCheck();
	}


	void testSeeds(int min_seed, int max_seed) {
		for (int seed = min_seed; seed <= max_seed; ++seed) {
			cout << "Testing seeds " << seed << endl;

			mt19937 gen(seed);
			uniform_int_distribution<> dis(0, 100);

			for (int i = 0; i < 10; ++i) {
				int x = dis(gen);
				int y = dis(gen);
				GPS* gps = new GPS(x, y);
				tree.insert(gps, gps);
			}
			treeSizeCheck();
			

		}
	}

	void oscilate(int count) {
		for (int i = 1; i <= count; ++i) {
			genPoints(10, 0, 100, 0);

			treeSizeCheck();

			clearStructure();
		}
	}

	void treeSizeCheck() {
		int realSize = 0;
		tree.inOrderTraversal([&](KDTreeNode<GPS, GPS>* node) {
			realSize++;
			});

		if (tree.size() == realSize) {
			cout << "Everything ok with size: " << realSize << " = "  << tree.size() << endl;
		}
		else {
			cout << "Something went wrong, size did not match! >" << realSize << " != " << tree.size() << endl;

		}

	}

	void findDataWithDuplicates(int x, int y) {
		if (tree.size() != 0) {
			GPS gps(x, y);
			vector<GPS*> datas = tree.find(&gps);
			if (datas.size() == 0) {
				cout << "No data found" << endl;
			}	
			else {
				for (GPS* d : datas) {
					cout << "GPS point" << *d << endl;
				}
			}
			
		}
		else {
			cout << "Tree is empty, fill it.";
		}
		
	}

	void clearStructure() {

		tree.clear();
		for (GPS* gps : gps_list) {
			delete gps;
		}
		gps_list.clear();
		cout << "All records removed! Size: " << tree.size();
	}

	~Tester() {
		clearStructure();

	}


};



