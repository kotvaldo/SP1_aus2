#pragma once
#include <iostream>
using namespace std;

template<typename T>
class IComparable {
public:
    virtual int compare(const T& other, int cur_level) const = 0;
    virtual bool equals(const T& other) const = 0;
    virtual ~IComparable() = default;
};




class GPS : public IComparable<GPS> {
public:
    int x, y;

    GPS(int x = 0, int y = 0) : x(x), y(y) {}

    int compare(const GPS& other, int cur_level) const override {
        if (cur_level % 2 == 0) {  // 0 je pre x súradnicu
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

    bool equals(const GPS& other) const override {
        return this->x == other.x && this->y == other.y;
    }

};


class Area : public IComparable<Area> {
    int uid;
    GPS* gps;
    Nehnutelnost* nehnutelnost;
    Parcela* parcela;
    int compare(const Area& other, int cur_level) const override {
        if (cur_level % 2 == 0) {  
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }


};

class Nehnutelnost : public IComparable<Nehnutelnost> {
    int uid;
    GPS* gps;
    bool equals(const Nehnutelnost& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }
    int compare(const Nehnutelnost& other, int cur_level) const override {
        if (cur_level % 2 == 0) {  // 0 je pre x súradnicu
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }
};

class Parcela : public IComparable<Parcela> {
    int uid;
    GPS* gps;
    bool equals(const Parcela& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }
    int compare(const Parcela& other, int cur_level) const override {
        if (cur_level % 2 == 0) {  // 0 je pre x súradnicu
            if (this->gps->x < other.gps->x) return -1;
            if (this->gps->x > other.gps->x) return 1;
            return 0;
        }
        else {
            if (this->gps->y < other.gps->y) return -1;
            if (this->gps->y > other.gps->y) return 1;
            return 0;
        }
    }
};