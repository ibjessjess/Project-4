#include "Sorting.h"

// returns true if shorter str is prefix of longer str
bool equivalentPrefixes(std::string s1, std::string s2)
{
	size_t min = (s1.size() < s2.size()) ? s1.size() : s2.size();
	for (size_t i = 0; i < min; i++)
	{
		if (s1[i] != s2[i]) { return false; }
	}
	return true;
}
// shell sorts an int array
void shellSort(AnyArray<int>& vec)
{
	int size = vec.getSize();
	for (int i = 0; i < size; ++i)
	{
		int min = i;
		for (int j = i; j < size; ++j)
		{
			if (vec.at(j) < vec.at(min)) { min = j; }
		}
		swap(vec, i, min);
	}
}
int RadixGetLength(int value)
{
	if (value == 0) { return 1; }
	int digits = 0;
	while (value != 0)
	{
		digits++;
		value /= 10;
	}
	return digits;
}
int RadixGetMaxLength(AnyArray<int>& arr)
{
	int max = arr[0], size = arr.getSize();
	for (int i = 1; i < size; i++)
	{
		if (arr[i] > max) { max = arr[i]; }
	}
	return max;
}
int findMin(AnyArray<int>& arr)
{
	int min = arr[0];
	for (size_t i = 0; i < arr.getSize(); i++)
	{
		if (arr[i] < min) { min = arr[i]; }
	}
	return min;
}
int findMax(AnyArray<int>& arr)
{
	int min = arr[0];
	for (size_t i = 0; i < arr.getSize(); i++)
	{
		if (arr[i] > min) { min = arr[i]; }
	}
	return min;
}
int getLowestDigit(int n)
{
	int figures = RadixGetLength(n), highestDigit;
	while (figures > 1)
	{
		highestDigit = (int)(n / pow(10, figures - 1));
		n -= (int)(highestDigit * pow(10, figures - 1));
		figures = RadixGetLength(n);
	}
	return abs(n);
}
void getLowestDigitUnitTest()
{
	int numbers[] = { 0,1,12,123,-1,-12,-123 };
	int key[] = { 0,1,2,3,1,2,3 };
	size_t size = sizeof(numbers) / sizeof(numbers[0]);
	for (size_t i = 0; i < size; i++)
	{
		if (getLowestDigit(numbers[i]) != key[i])
		{
			std::cout
				<< "Failed unit test for int getLowestDigit(int)\n"
				<< "Test case: " << numbers[i] << "\nResult: "
				<< getLowestDigit(numbers[i]) << "\nExpected: "
				<< key[i] << '\n';
			return;
		}
	}
	std::cout << "Passed unit test for int getLowestDigit(int)\n";
}
void VectorToBuckets(
	AnyArray<int> arr, int pow10,
	AnyArray<int>(&buckets)[10])
{
	size_t i, bucketIndex, size = arr.getSize();
	for (i = 0; i < size; i++)
	{ // put values from arr into buckets
		bucketIndex = getLowestDigit(arr[i] / pow10);
		buckets[bucketIndex].pushback(arr[i]);
	}
}
void BucketsToVector(
	AnyArray<int>& arr,
	AnyArray<int> buckets[10])
{
	int i, j, bucketSize, arrCount = 0;
	for (i = 0; i < 10; i++)
	{
		bucketSize = buckets[i].getSize();
		for (j = 0; j < bucketSize; j++)
		{
			arr.set(buckets[i][j], arrCount++);
		}
	}
}
void ClearBuckets(AnyArray<int>(&buckets)[10])
{
	for (size_t i = 0; i < 10; i++)
	{
		buckets[i].clear();
	}
}
void RadixSort(AnyArray<int>& arr)
{
	int maxDigits = RadixGetMaxLength(arr),
		digitIndex;
	size_t i;

	AnyArray<int> buckets[10];
	digitIndex = 0;
	int pow10 = 1; // start with least significant digit
	for (i = 0; i < maxDigits; i++)
	{
		VectorToBuckets(arr, pow10++, buckets);
		BucketsToVector(arr, buckets);
		ClearBuckets(buckets);
	}
}
void SelectionSortAllTests()
{
	// test 1
	int testSet1[] = { 1, 8, 3, 5, 8, 2, 9 };
	int keySet1[] = { 1, 2, 3, 5, 8, 8, 9 };
	size_t size1 = sizeof(testSet1) / sizeof(testSet1[0]);
	SelectionSortUnitTest(size1, testSet1, keySet1);

	// test 2
	int keySet2[] = { 9, 8, 8, 5, 3, 2, 1 };
	bool (*lambda2)(int, int) =
		[](int t1, int t2) { return ((t1 >= t2)); };
	SelectionSortUnitTest(size1, testSet1, keySet2, lambda2);

	// test 3
	char testSet3[] = { 'q','w','e','r','t','y' };
	char keySet3[] = { 'e','q','r','t','w','y' };
	size_t size3 = sizeof(testSet3) / sizeof(testSet3[0]);
	SelectionSortUnitTest(size3, testSet3, keySet3);

	// test 4
	int testSet4[] = { -1,-8,-4,-2 };
	int keySet4[] = { -8,-4,-2,-1 };
	size_t size4 = sizeof(keySet4) / sizeof(keySet4[0]);
	SelectionSortUnitTest(size4, testSet4, keySet4);

	std::cout << "All tests passed for selection sort!\n";
}
void SelectionSearchUnitTest()
{
	// test 1
	int testSet1[] = { -10,-8,-4,0,4,5,6,20,100 };
	AnyArray<int> arr;
	int key = 0;
	int size, lastIndex;
	for (int i : testSet1)
	{
		std::cout << i << '\n';
		arr.pushback(i);
	}
	size = arr.getSize();
	lastIndex = size - 1;
	int result;
	for (int i : testSet1)
	{
		result = (int)SelectionSearch(arr, i, 0, lastIndex);
		if (result != key)
		{
			std::cout
				<< "Failed!\n" << "Expected: " << key
				<< "\nResult: " << result << "\nValue: "
				<< i << '\n';
			return;
		}
		key++;
	}
	std::cout << "Passed!\n";
}