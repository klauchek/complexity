#include <iostream>
#include <vector>
#include <future>
#include <algorithm>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1), rightArr(n2);

    for (int i = 0; i < n1; ++i)
        leftArr[i] = arr[left + i];

    for (int i = 0; i < n2; ++i)
        rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}

void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth = 0) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    if (depth < 4) {
        auto leftHalf = std::async(std::launch::async, parallelMergeSort, std::ref(arr), left, mid, depth + 1);
        auto rightHalf = std::async(std::launch::async, parallelMergeSort, std::ref(arr), mid + 1, right, depth + 1);

        leftHalf.get();
        rightHalf.get();
    }
    else {
        parallelMergeSort(arr, left, mid, depth + 1);
        parallelMergeSort(arr, mid + 1, right, depth + 1);
    }

    merge(arr, left, mid, right);
}

// int main() {
//     std::vector<int> arr = {38, 27, 43, 3, 9, 82, 10, -5, 25, 45, 100};

//     std::cout << "Original array: ";
//     for (int num : arr) std::cout << num << " ";
//     std::cout << std::endl;

//     parallelMergeSort(arr, 0, arr.size() - 1);

//     std::cout << "Sorted array: ";
//     for (int num : arr) std::cout << num << " ";
//     std::cout << std::endl;

//     return 0;
// }
