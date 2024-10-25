#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "Models.h"
#include "KDTree.h"

using namespace std;

template <typename DataType>
class Tester {
private:
	vector<DataType*> gps_list;
	GeneralKDTree<GPS, DataType> tree;
	vector<int> uid_list;
public:
	Tester() : tree(2) {}

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
			GPS* gps = new GPS(x, y);
			DataType* n = new DataType(uid, gps);
			tree.insert(n, gps);
			gps_list.push_back(n);

			if (desc) cout << "Point " << i + 1 << ": " << *n << " added successfully" << endl;
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
				GPS* gps = new GPS(x, y);
				DataType* n = new DataType(uid, gps);
				tree.insert(n, gps);
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
		tree.inOrderTraversal([&](KDTreeNode<GPS, DataType>* node) {
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
			GPS gps(x, y);
			vector<DataType*> datas = tree.find(&gps);
			if (datas.empty()) {
				cout << "No data found at GPS: (" << x << ", " << y << ")" << endl;
			}
			else {
				cout << "Found " << datas.size() << " entries at GPS: (" << x << ", " << y << ")" << endl;
				for (auto d : datas) {
					cout << *d << endl; 
				}
			}
		}
		else {
			cout << "Tree is empty. Please generate data first." << endl;
		}
	}

	void clearStructure() {
		tree.clear();
		for (DataType* n : gps_list) {
			delete n;
		}

		uid_list.clear();
		gps_list.clear();
		cout << "All records removed! Current tree size: " << tree.size() << endl;
	}

	~Tester() {
		clearStructure();
	}

    int getUnicateId() {
		if (uid_list.empty()) {
			uid_list.push_back(1);
			return uid_list.size(); 
		}
		uid_list.push_back(uid_list.size() + 1);
		return uid_list.size();
	}
};
