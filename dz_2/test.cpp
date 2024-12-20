#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <limits.h>

void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth = 0);

void runTests() {
    auto isSorted = [](const std::vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) return false;
        }
        return true;
    };

    // Test 1: Sorting an empty array
    {
        std::vector<int> arr;
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(arr.empty());
        std::cout << "Test 1 passed: Empty array.\n";
    }

    // Test 2: Sorting an array with one element
    {
        std::vector<int> arr = {5};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 2 passed: Single element array.\n";
    }

    // Test 3: Sorting an already sorted array
    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 3 passed: Already sorted array.\n";
    }

    // Test 4: Sorting a reverse sorted array
    {
        std::vector<int> arr = {5, 4, 3, 2, 1};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 4 passed: Reverse sorted array.\n";
    }

    // Test 5: Sorting an array with all identical elements
    {
        std::vector<int> arr = {7, 7, 7, 7, 7};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 5 passed: Array with identical elements.\n";
    }

    // Test 6: Sorting an array with negative and positive numbers
    {
        std::vector<int> arr = {3, -1, 4, -5, 2, 0, -3};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 6 passed: Array with negative and positive numbers.\n";
    }

    // Test 7: Sorting a large random array
    {
        std::vector<int> arr(10000);
        for (int i = 0; i < 10000; ++i) {
            arr[i] = rand() % 10000 - 5000;  // Random numbers between -5000 and 5000
        }
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 7 passed: Large random array.\n";
    }

    // Test 8: Sorting an array with duplicates
    {
        std::vector<int> arr = {5, 3, 3, 5, 1, 1, 2, 2, 4, 4};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 8 passed: Array with duplicate values.\n";
    }

    // Test 9: Sorting an array with very large numbers
    {
        std::vector<int> arr = {INT_MAX, INT_MIN, 0, INT_MAX - 1, INT_MIN + 1};
        parallelMergeSort(arr, 0, arr.size() - 1);
        assert(isSorted(arr));
        std::cout << "Test 9 passed: Array with very large numbers.\n";
    }
}

int main() {
    runTests();
    std::cout << "All tests passed!\n";
    return 0;
}
