#pragma once
#ifndef REGION_H
#define REGION_H
#include "AnyArray.h"
#include "Location.h"
#include "OutputSettings.h"

/*Represents a geographical region containing one or more geographical 
* locations, implemented as a dynamic array of pointers. */

class Region : public AnyArray<Location*> 
{
private:
    int ID;
    std::string name;
public:
    Region();
    Region(std::string);
    std::string getName() const { return name; }
    bool operator==(Region reg) { return (name == reg.getName()); }
    bool operator!=(Region reg) { return (name != reg.getName()); }
    void printLocations() {
        std::cout << "PRINTING " << size << " locations\n";
        for (size_t i = 0; i < size; i++) {
            ScreenBreak(i);
            at(i)->printData();
        }
    }
};

#endif // !REGION_H