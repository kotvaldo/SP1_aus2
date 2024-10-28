#include <iostream>
#include "Tester.h"
#include "NehnutelnostTester.h"

using namespace std;

void displayMenu() {
    cout << endl << endl << "---------------------------------------------------------------" << endl;
    cout << endl << "What next? Choose an option:" << endl;
    cout << "1: Generate Points" << endl;
    cout << "2: Test Seeds" << endl;
    cout << "3: Oscillate" << endl;
    cout << "4: Clear Structure" << endl;
    cout << "5: Find Nodes" << endl;
    cout << "6: Remove Node" << endl;
    cout << "7: Print Tree" << endl;
    cout << "8: Insert Node" << endl;  // Nová možnosť pre vloženie uzla
    cout << "9: End" << endl;
}
/*
int main() {
    bool end = false;
    Tester<TestClass>* t = new Tester<TestClass>();

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
        case 5: {
            double A, D;
            string B;
            int C;
            cout << "Enter attribute A (double): ";
            cin >> A;
            cout << "Enter attribute B (string): ";
            cin >> B;
            cout << "Enter attribute C (integer): ";
            cin >> C;
            cout << "Enter attribute D (double): ";
            cin >> D;
            t->findDataWithDuplicates(A, B, C, D);  // Používame hodnoty atribútov `TestClass` na vyh¾adávanie
            break;
        }
        case 6: {
            int id;
            cout << "Enter ID of the node: ";
            cin >> id;
            t->deleteTestWithParams(id);  // Odstránenie uzla pod¾a `uid` v `TestClass`
            break;
        }
        case 7:  // Nová možnos pre výpis stromu
            t->printTreeNodes();  // Zavoláme metódu na výpis uzlov stromu
            break;
        case 8:
            end = true;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    delete t;
    return 0;
}
*/


int main() {
    bool end = false;
    NehnutelnostTester t;  
    vector<tuple<string, int, int>> nodes = {
       {"Senica", 22, 39},
       {"Tlmace - urad", 24, 36},
       {"Tlmace", 24, 34},
       {"Tlmace - parkovisko", 24, 40},
       {"Tlmace – nem.", 24, 35},
       {"Levice", 30, 33},
       {"Bojnice", 29, 46},
       {"Novaky", 27, 43}
    };

    for (const auto& [name, x, y] : nodes) {
        t.insertNode(x, y, name);
    }
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
            t.genPoints(num_points, range_min, range_max, seed, true);
            break;
        }
        case 2: {
            int min_seed, max_seed;
            cout << "Enter min seed: ";
            cin >> min_seed;
            cout << "Enter max seed: ";
            cin >> max_seed;
            t.testSeeds(min_seed, max_seed);
            break;
        }
        case 3: {
            int count;
            cout << "Enter oscillation count: ";
            cin >> count;
            t.oscilate(count);
            break;
        }
        case 4:
            t.clearStructure();
            break;
        case 5: {
            int x, y;
            cout << "Enter x coordinate (int): ";
            cin >> x;
            cout << "Enter y coordinate (int): ";
            cin >> y;
            t.findDataWithCoordinates(x, y);  // Vyhľadávanie podľa súradníc `GPS`
            break;
        }
        case 6: {
            int id;
            cout << "Enter ID of the node: ";
            cin >> id;
            t.deleteNodeById(id);  // Odstránenie uzla podľa `uid` v `Nehnutelnost`
            break;
        }
        case 7:
            t.printTreeNodes();  // Výpis uzlov v KD strome
            break;
        case 8: {  // Nová možnosť pre manuálne vloženie uzla
            int x, y;
            cout << "Enter x coordinate (int): ";
            cin >> x;
            cout << "Enter y coordinate (int): ";
            cin >> y;
            t.insertNode(x, y);  // Vloženie nového uzla s hodnotami x a y
            break;
        }
        case 9:
            end = true;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}
