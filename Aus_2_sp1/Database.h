#pragma once
#include <vector>
#include "KDTree.h"

template <typename KeyType, typename DataType>
class DatabaseLoader {
protected:
    GeneralKDTree<KeyType, DataType>* tree;
    std::vector<DataType*> databaseRecords;

public:
    DatabaseLoader(GeneralKDTree<KeyType, DataType>* tree) : tree(tree) {}

    virtual void loadDatabaseFromFile(const std::string& filename) = 0;

    void fillTreeWithDatabaseRecords() {
        for (auto record : databaseRecords) {
            tree->insert(record, record);
        }
    }

    void insertRecord(DataType* record) {
        tree->insert(record, record);
        databaseRecords.push_back(record);
    }

    void deleteRecord(DataType* record) {
        tree->removeNode(record);
    }

    void clearDatabase() {
        tree->clear();
    }

    virtual ~DatabaseLoader() {
        for (auto record : databaseRecords) {
            delete record;
        }
    }

protected:
    virtual DataType* parseRecord(const std::string& line) = 0;
};


