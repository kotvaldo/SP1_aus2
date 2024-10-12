// Aus_2_sp1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Tester.h"

int main()
{
    Tester* t = new Tester(100, 100, 300, 0);
    t->run();
}
