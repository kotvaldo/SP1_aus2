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

    bool equals(const GPS& other) const override {
        return this->x == other.x && this->y == other.y;
    }

    friend ostream& operator<<(ostream& os, const GPS& gps) {
        os << "(" << gps.x << ", " << gps.y << ")";
        return os;
    }
};

class Nehnutelnost;
class Parcela;



class Nehnutelnost : public IComparable<Nehnutelnost> {
public:
    int uid;
    GPS* gps;

    Nehnutelnost(int id, GPS* gpsCoord) : uid(id), gps(gpsCoord) {}

    bool equals(const Nehnutelnost& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    int compare(const Nehnutelnost& other, int cur_level) const override {
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

    friend ostream& operator<<(ostream& os, const Nehnutelnost& nehnutelnost) {
        os << "Nehnutelnost(uid: " << nehnutelnost.uid << ", GPS: " << *nehnutelnost.gps << ")";
        return os;
    }
};

class Parcela : public IComparable<Parcela> {
public:
    int uid;
    GPS* gps;

    Parcela(int id, GPS* gpsCoord) : uid(id), gps(gpsCoord) {}

    bool equals(const Parcela& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    int compare(const Parcela& other, int cur_level) const override {
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

    friend ostream& operator<<(ostream& os, const Parcela& parcela) {
        os << "Parcela(uid: " << parcela.uid << ", GPS: " << *parcela.gps << ")";
        return os;
    }
};

class Area : public IComparable<Area> {
public:
    int uid;
    GPS* gps;
    Nehnutelnost* nehnutelnost;
    Parcela* parcela;

    Area(int id, GPS* gpsCoord, Nehnutelnost* nehnut = nullptr, Parcela* parc = nullptr)
        : uid(id), gps(gpsCoord), nehnutelnost(nehnut), parcela(parc) {}

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

    bool equals(const Area& other) const override {
        return this->gps->x == other.gps->x && this->gps->y == other.gps->y && this->uid == other.uid;
    }

    friend ostream& operator<<(ostream& os, const Area& area) {
        os << "Area(uid: " << area.uid << ", GPS: " << *area.gps;
        if (area.nehnutelnost) os << ", Nehnutelnost: " << *area.nehnutelnost;
        if (area.parcela) os << ", Parcela: " << *area.parcela;
        os << ")";
        return os;
    }
};


