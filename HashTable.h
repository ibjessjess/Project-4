#pragma once
#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "RainArray.h"
#include <iostream>

// NOT YET IN USE
class RainHash {
protected:
    int numBuckets;
    RainArray<int>* table;
public:
    RainHash() {
        numBuckets = 0;
        table = new RainArray<int>[numBuckets];
    }
    RainHash(int);
    ~RainHash();
    void Insert(int);
    void Remove(int);
    int Hash(int val) { return val % numBuckets; }
    void Print();
};

#endif // !HASH_TABLE_H
