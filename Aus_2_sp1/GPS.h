#pragma once
#include "KDTree.h"

class GPS : public IComparable<GPS> {
public:
    int x, y;

    GPS(int x = 0, int y = 0) : x(x), y(y) {}

    int compare(const GPS& other, int cur_level) const override {
        if (cur_level % 2 == 0) {  
            if (this->x < other.x) return -1;
            if (this->x > other.x) return 1;
            return 0;
        }
        else {  
            if (this->y < other.y) return -1;
            if (this->y > other.y) return 1;
            return 0;
        }
    }

    friend ostream& operator<<(ostream& os, const GPS& gps) {
        os << "(" << gps.x << ", " << gps.y << ")";
        return os;
    }
    
};