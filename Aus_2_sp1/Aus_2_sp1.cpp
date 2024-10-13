#include <iostream>
#include "Tester.h"

using namespace std;

void displayMenu() {

    cout << endl << endl << "---------------------------------------------------------------" << endl;
    cout << endl << "What next? Choose an option:" << endl;

    cout << "1: Generate Points" << endl;
    cout << "2: Test Seeds" << endl;
    cout << "3: Oscillate" << endl;
    cout << "4: Clear Structure" << endl;
    cout << "5: End" << endl;
}

int main() {
    bool end = false;
    Tester* t = new Tester();

    while (!end) {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int num_points, range_min, range_max, seed;
            cout << "Enter number of points: ";
            cin >> num_points;
            cout << "Enter min range: ";
            cin >> range_min;
            cout << "Enter max range: ";
            cin >> range_max;
            cout << "Enter seed (0 for random): ";
            cin >> seed;
            t->genPoints(num_points, range_min, range_max, seed, true);
            break;
        }
        case 2: {
            int min_seed, max_seed;
            cout << "Enter min seed: ";
            cin >> min_seed;
            cout << "Enter max seed: ";
            cin >> max_seed;
            t->testSeeds(min_seed, max_seed);
            break;
        }
        case 3: {
            int count;
            cout << "Enter oscillation count: ";
            cin >> count;
            t->oscilate(count);
            break;
        }
        case 4:
            t->clearStructure();
            break;
        case 5:
            end = true;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    delete t;
    return 0;
}