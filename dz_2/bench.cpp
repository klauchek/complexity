#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <cassert>
#include <future>

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> leftArr(n1);
    std::vector<int> rightArr(n2);

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        rightArr[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }

    while (i < n1)
        arr[k++] = leftArr[i++];
    while (j < n2)
        arr[k++] = rightArr[j++];
}

void parallelMergeSort(std::vector<int>& arr, int left, int right, int depth, int numThreads) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < numThreads) {
            auto leftFuture = std::async(std::launch::async, parallelMergeSort, std::ref(arr), left, mid, depth + 1, numThreads);
            parallelMergeSort(arr, mid + 1, right, depth + 1, numThreads);
            leftFuture.wait();
        }
        else {
            parallelMergeSort(arr, left, mid, depth, numThreads);
            parallelMergeSort(arr, mid + 1, right, depth, numThreads);
        }
        merge(arr, left, mid, right);
    }
}

std::vector<int> generateRandomArray(int size) {
    std::vector<int> arr(size);
    for (int& x : arr)
        x = rand() % 10000 - 5000;
    return arr;
}

double benchmarkMergeSort(int n, int numThreads) {
    std::vector<int> arr = generateRandomArray(n);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    parallelMergeSort(arr, 0, arr.size() - 1, 0, numThreads);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    return duration.count();
}

void runBenchmark(int arraySize) {
    int maxThreads = std::thread::hardware_concurrency();
    double baseTime = benchmarkMergeSort(arraySize, 1);

    std::cout << "Array size: " << arraySize << ", Base time (1 thread): " << baseTime << "s\n";

    for (int numThreads = 2; numThreads <= maxThreads; numThreads++) {
        double time = benchmarkMergeSort(arraySize, numThreads);
        double speedup = baseTime / time;
        double theoreticalSpeedup = numThreads / std::log2(arraySize);

        std::cout << "Threads: " << numThreads 
                  << ", Time: " << time << "s"
                  << ", Real Speedup: " << speedup 
                  << ", Theoretical Speedup: " << theoreticalSpeedup << "\n";
    }
}

int main() {
    int arraySize = 1000000;
    runBenchmark(arraySize);
    return 0;
}
