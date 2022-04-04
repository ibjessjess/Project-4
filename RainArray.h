#pragma once
#ifndef RAINARRAY_H
#define RAINARRAY_H
#include "AnyArray.h"
#include <iostream>


/* 
This child class was created to allow for more specific functionality
of the rainChart member inside of location objects, but so far,
none has been needed. To see better example of inheritance in this
project, please view RegionArray.h and Region.h */

// |!| this class requires explicit deallocation | !|
// |!| see RainArray::Deallocate() before using | !|
template<typename T>
class RainArray : public AnyArray<T> {
public:
	using AnyArray<T>::AnyArray;
};
#endif // !RAINARRAY_H