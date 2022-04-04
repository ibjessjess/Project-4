#pragma once
#ifndef SORTANDSWAP_H
#define SORTANDSWAP_H
#include <iostream>
#include <string>
#include <cmath>
#include <stdio.h>
#include <assert.h>
#include "AnyArray.h"
#include "LocationArray.h"

/* Algorithms from week seven sorting. Many of these are not
*  actually used, mostly wrote them for understanding's sake */

// no template sorts
int RadixGetMaxLength(AnyArray<int>& arr);
int RadixGetLength(int);
int findMin(AnyArray<int>);
int findMax(AnyArray<int>);
int getLowestDigit(int);
void shellSort(AnyArray<int>& vec);
void RadixSort(AnyArray<int>& arr);
void VectorToBuckets(AnyArray<int>, int, AnyArray<int> (&)[10]);
void BucketsToVector(AnyArray<int>&, AnyArray<int>[10]);
bool equivalentPrefixes(std::string, std::string);

// no template unit tests
void SelectionSearchUnitTest();
void getLowestDigitUnitTest();

void SelectionSortAllTests();

// template sorts


template <typename AbstractType>
void swap(AbstractType& x, AbstractType& y) {
	AbstractType foo = x;
	x = y;
	y = foo;
};
template<typename T>
void swap(AnyArray<T>& array, int i1, int i2) {
	T foo = array.at(i1);
	array.set(array.at(i2), i1);
	array.set(foo, i2);
}
template<typename T>
void BubbleSort(
    AnyArray<T>& arr,
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
)
{
    bool done = false;
    for (size_t j = 0; j < arr.getSize() - 1; j++)
    {
        done = true;
        for (int i = 0; i < arr.getSize() - j - 1; ++i)
        {
            if (!sortCriteria(arr[i], arr[i+1]))
            {
                swap(arr, i, i + 1);
                done = false;
            }
        }
    }
}
template <typename T>
int Partition(
    AnyArray<T>& arr,
    bool(*sortCriteria)(T, T),
    int low, int high)
{
    T pivot = arr.at(high);
    int i = (low - 1);
    int stop = high - 1;
    for (int j = low; j <= stop; j++)
    {
        if (sortCriteria(arr.at(j), pivot))
        {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i + 1, high);
    return i + 1;
}
template<typename T>
void QuickSort(
    AnyArray<T>& arr,
    bool(*sortCriteria)(T, T),
    int low, int high
)
{
    if (low < high)
    {
        int pivot = Partition(arr, sortCriteria, low, high);
        QuickSort(arr, sortCriteria, low, pivot - 1);
        QuickSort(arr, sortCriteria, pivot + 1, high);
    }
}
template<typename T>
void QuickSort(
    AnyArray<T>& arr,
    bool (*sortCriteria)(T, T))
{
    QuickSort(arr, sortCriteria, 0, arr.getSize() - 1);
}
inline void QuickSortTest()
{
    std::cout << "Test one:\n";
    int testSet1[] = { 53, 14, 64, 23, 86, 64, 234, 23 };
    AnyArray<int> testArr1;
    for (int i : testSet1) { testArr1.pushback(i); }
    bool(*testCriteria1)(int, int) = [](int x, int y) {return (x >= y); };
    QuickSort(testArr1, testCriteria1);
    for (size_t i = 0; i < testArr1.getSize(); i++)
    {
        std::cout << testArr1[i] << ", ";
    }

    std::cout << "\ntest two:\n";
    int testSet2[] = { 53, 14, 64, 23, 86, 64, 234, 23 };
    AnyArray<int> testArr2;
    for (int i : testSet2) { testArr2.pushback(i); }
    bool(*testCriteria2)(int, int) = [](int x, int y) {return (x <= y); };
    QuickSort(testArr2, testCriteria2);
    for (size_t i = 0; i < testArr2.getSize(); i++)
    {
        std::cout << testArr2[i] << ", ";
    }

    std::cout << "\ntest three:\n";
    int testSet3[] = { 53, -14, 64, 23, -86, 64, -234, 23 };
    AnyArray<int> testArr3;
    for (int i : testSet3) { testArr3.pushback(i); }
    bool(*testCriteria3)(int, int) = [](int x, int y) {return (x >= y); };
    QuickSort(testArr3, testCriteria3);
    for (size_t i = 0; i < testArr3.getSize(); i++)
    {
        std::cout << testArr3[i] << ", ";
    }

    std::cout << "\ntest four:\n";
    std::string testSet4[] = { "dog", "cat", "snake", "frog", "rat", "bird"};
    AnyArray<std::string> testArr4;
    for (std::string i : testSet4) { testArr4.pushback(i); }
    bool(*testCriteria4)(std::string, std::string) = 
        [](std::string x, std::string y) {return (x >= y); };
    QuickSort(testArr4, testCriteria4);
    for (size_t i = 0; i < testArr4.getSize(); i++)
    {
        std::cout << testArr4[i] << ", ";
    }

}
template <typename T>
void Merge(
    AnyArray <T>& arr, int left, int mid, int right, 
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
){
    int arrOneLen = mid - left + 1, arrTwoLen = right - mid;
    T* arrOne = new T[arrOneLen];
    T* arrTwo = new T[arrTwoLen];
    for (size_t i = 0; i < arrOneLen; i++)
    {
        arrOne[i] = arr[left + i];
    }
    for (size_t i = 0; i < arrTwoLen; i++)
    {
        arrTwo[i] = arr[mid + i+ 1];
    }
    int arrOneIndex = 0, arrTwoIndex = 0, mergedIndex = left;
    while (arrOneIndex < arrOneLen && arrTwoIndex < arrOneLen)
    {
        if (sortCriteria(arrOne[arrOneIndex], arrTwo[arrTwoIndex]))
        {
            arr.set(arrOne[arrOneIndex], mergedIndex);
            ++arrOneIndex;
        }
        else
        {
            arr.set(arrTwo[arrTwoIndex], mergedIndex);
            ++arrTwoIndex;
        }
        ++mergedIndex;
    }
	while (arrOneIndex < arrOneLen) 
	{
		arr.set(arrOne[arrOneIndex], mergedIndex);
		arrOneIndex++;
        mergedIndex++;
	}
	while (arrTwoIndex < arrTwoLen)
	{
		arr.set(arrTwo[arrTwoIndex], mergedIndex);
		arrTwoIndex++;
        mergedIndex++;
	}
    delete arrOne;
    delete arrTwo;
}
template <typename T>
void MergeSort(
    AnyArray<T>& arr, int low, int high,
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
){
    if (low >= high) { return; }
    int mid = low + ((high - low) / 2);
    MergeSort(arr, low, mid, sortCriteria);
    MergeSort(arr, mid + 1, high, sortCriteria);
    Merge(arr, low, mid, high, sortCriteria);

}
template <typename T>
void Merge(
    T arr[], int left, int mid, int right,
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
)
{
    int arrOneLen = mid - left + 1, arrTwoLen = right - mid;
    T* arrOne = new T[arrOneLen];
    T* arrTwo = new T[arrTwoLen];
    for (size_t i = 0; i < arrOneLen; i++)
    {
        arrOne[i] = arr[left + i];
    }
    for (size_t i = 0; i < arrTwoLen; i++)
    {
        arrTwo[i] = arr[mid + i + 1];
    }
    int arrOneIndex = 0, arrTwoIndex = 0, mergedIndex = left;
    while (arrOneIndex < arrOneLen && arrTwoIndex < arrOneLen)
    {
        if (sortCriteria(arrOne[arrOneIndex], arrTwo[arrTwoIndex]))
        {
            arr[mergedIndex] = arrOne[arrOneIndex];
            ++arrOneIndex;
        } else
        {
            arr[mergedIndex] = arrTwo[arrTwoIndex];
            ++arrTwoIndex;
        }
        ++mergedIndex;
    }
    while (arrOneIndex < arrOneLen)
    {
        arr[mergedIndex] = arrOne[arrOneIndex];
        arrOneIndex++;
        mergedIndex++;
    }
    while (arrTwoIndex < arrTwoLen)
    {
        arr[mergedIndex] = arrTwo[arrTwoIndex];
        arrTwoIndex++;
        mergedIndex++;
    }
    delete[] arrOne;
    delete[] arrTwo;
}
template <typename T>
void MergeSort(
    T arr[], int low, int high,
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
)
{
    if (low >= high) { return; }
    int mid = low + ((high - low) / 2);
    MergeSort(arr, low, mid, sortCriteria);
    MergeSort(arr, mid + 1, high, sortCriteria);
    Merge(arr, low, mid, high, sortCriteria);

}
template <typename T>
void MergeSort(
    AnyArray<T>& arr,
    bool(*sortCriteria)(T, T) = [](T t1, T t2) { return (t1 < t2); }
)
{
    int n = arr.getSize();
    T* temp = new T[n];
    MergeSort(temp, 0, n - 1, sortCriteria);
    arr.clear();
    for (size_t i = 0; i < n; i++) { arr.pushback(temp[i]); }
    delete[] temp;
}

template <typename T, typename V>
int SelectionSearch(
    AnyArray<T> arr, V value,
    int low, int high,
    bool(*isEqual)(T, V) =
    [](T t1, V t2) { return (t1 == t2); },
    bool(*isOrdered)(T,V) =
    [](T t1, V t2) { return (t1 <= t2); }
) {
    int mid = (low + high) / 2;
	if (isEqual(arr.at(mid), value)) { return mid; }
    else if (low >= high) { return -1; } // value does not exist
    else if (isOrdered(arr.at(mid), value)) {
        return SelectionSearch(arr, value, mid + 1, high, isEqual, isOrdered);
    }
    else {
        return SelectionSearch(arr, value, low, mid - 1, isEqual, isOrdered);
    }
}
template <typename T>
void selectionSort(AnyArray<T>& vec, 
    bool(*isOrdered)(T, T) = 
    [](T t1, T t2) { return (t1 <= t2); }
) {
    int size = vec.getSize();
    for (int i = 0; i < size; ++i) {
        int min = i;
        for (int j = i; j < size; ++j) {
            if (isOrdered(vec.at(j), vec.at(min))) { min = j; }
        }
        swap(vec, i, min);
    }
}

template <typename T>
int SelectionSortUnitTest(
    size_t setLength,
    T testSet[],
    T keySet[],
    bool(*sortCriteria)(T, T) =
    [](T t1, T t2) { return (t1 < t2); }
) {
    AnyArray<T> arr;
    for (size_t i = 0; i < setLength; i++) {
        arr.pushback(testSet[i]);
    }
    selectionSort(arr, sortCriteria);
    std::cout << '\n';
    for (size_t i = 0; i < arr.getSize(); i++) {
        std::cout << arr.at(i) << " ";
        if (arr.at(i) != keySet[i]) {
            std::cout << "Failed!\n";
            return 1;
        }
    }
    std::cout << "Passed!\n";
    return 0;
}

#endif //  SORTANDSWAP_H

//template <typename T>
//void Merge(AnyArray<T>& arr, int low, int mid, int high)
//{
//    std::cout << "Calling merge with i: " << low << ", j: " << mid << ", k: " << high << '\n';
//    int mergedSize = high - low + 1;
//    int mergePos = 0;
//    int leftPos;
//    int rightPos = 0;
//    T* mergedNumbers = new T[mergedSize];
//    leftPos = low;
//    rightPos = mid + 1;
//    // fill in merged numbers
//    while (leftPos <= mid && rightPos <= high)
//    {
//        if (nums[leftPos] <= nums[rightPos])
//        {
//            mergedNumbers[mergePos] = nums[leftPos];
//            ++leftPos;
//        } else
//        {
//            mergedNumbers[mergePos] = nums[rightPos];
//            ++rightPos;
//        }
//        ++mergePos;
//    }
//    while (leftPos <= mid)
//    {
//        mergedNumbers[mergePos] = nums[leftPos];
//        ++leftPos;
//        ++mergePos;
//    }
//    while (rightPos <= high)
//    {
//        mergedNumbers[mergePos] = nums[rightPos];
//        ++rightPos;
//        ++mergePos;
//    }
//    for (mergePos = 0; mergePos < mergedSize; ++mergePos)
//    {
//        nums[low + mergePos] = mergedNumbers[mergePos];
//    }
//}
//
//
//template <typename T>
//void MergeSort(AnyArray<T>& arr, int low, int high)
//{
//    int mid;
//    if (low < high)
//    { // if i >= k, nothing to sort
//        mid = (low + high) / 2;  // Find the midpoint in the partition
//
//        // Recursively sort left and right partitions
//        MergeSort(numbers, low, mid);
//        MergeSort(numbers, mid + 1, high);
//
//        // Merge left and right partition in sorted order
//        Merge(numbers, low, mid, high);
//    }
//}