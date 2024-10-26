#include <vector>
#include "KDTree.h"
#include "Models.h"  // Nehnutelnost, Area, Parcela, TestClass a �al�ie modely s implementovan�m copy constructorom

template <typename KeyType, typename DataType>
class Controller {
private:
    GeneralKDTree<KeyType, DataType>* tree;
    std::vector<DataType*> guiData;  // Kopie pre GUI

public:
    Controller(GeneralKDTree<KeyType, DataType>* tree) : tree(tree) {}

    // Met�da na kop�rovanie uzlov stromu do guiData pre zobrazenie v GUI
    void copyTreeToGUI() {
        // Vy�isti existuj�ce d�ta
        clearGUIData();

        // In-order prehliadka stromu a vytv�ranie k�pi� uzlov
        tree->inOrderTraversal([this](KDTreeNode<KeyType, DataType>* node) {
            DataType* copy = new DataType(*node->_data);  // Pou�itie copy constructor
            guiData.push_back(copy);  // Pridanie k�pie do zoznamu pre GUI
            });

        std::cout << "Strom �spe�ne skop�rovan� do GUI d�t." << std::endl;
    }

    // Met�da na z�skanie d�t pre GUI (napr�klad getter)
    const std::vector<DataType*>& getGUIData() const {
        return guiData;
    }

    // Met�da na vy�istenie star�ch k�pi� v guiData
    void clearGUIData() {
        for (DataType* data : guiData) {
            delete data;  // Uvo�nenie dynamickej pam�te pre ka�d� uzol
        }
        guiData.clear();
    }

    ~Controller() {
        clearGUIData();
    }
};