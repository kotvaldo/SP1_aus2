#include <vector>
#include "KDTree.h"
#include "Models.h"  // Nehnutelnost, Area, Parcela, TestClass a ïalšie modely s implementovaným copy constructorom

template <typename KeyType, typename DataType>
class Controller {
private:
    GeneralKDTree<KeyType, DataType>* tree;
    std::vector<DataType*> guiData;  // Kopie pre GUI

public:
    Controller(GeneralKDTree<KeyType, DataType>* tree) : tree(tree) {}

    // Metóda na kopírovanie uzlov stromu do guiData pre zobrazenie v GUI
    void copyTreeToGUI() {
        // Vyèisti existujúce dáta
        clearGUIData();

        // In-order prehliadka stromu a vytváranie kópií uzlov
        tree->inOrderTraversal([this](KDTreeNode<KeyType, DataType>* node) {
            DataType* copy = new DataType(*node->_data);  // Použitie copy constructor
            guiData.push_back(copy);  // Pridanie kópie do zoznamu pre GUI
            });

        std::cout << "Strom úspešne skopírovaný do GUI dát." << std::endl;
    }

    // Metóda na získanie dát pre GUI (napríklad getter)
    const std::vector<DataType*>& getGUIData() const {
        return guiData;
    }

    // Metóda na vyèistenie starých kópií v guiData
    void clearGUIData() {
        for (DataType* data : guiData) {
            delete data;  // Uvo¾nenie dynamickej pamäte pre každý uzol
        }
        guiData.clear();
    }

    ~Controller() {
        clearGUIData();
    }
};