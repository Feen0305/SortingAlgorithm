#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Every sorting algorithm are respect to lab and slide

// Bubble Sort
long long int bubbleSort(int arr[], int n, long long int *swapCount) {
    long long int comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparisons++;
            if (arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                (*swapCount)++;
            }
        }
    }
    return comparisons;
}

// Selection Sort
long long int selectionSort(int arr[], int n, long long int *swapCount) {
    long long int comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        // Swap
        int temp = arr[minIdx];
        arr[minIdx] = arr[i];
        arr[i] = temp;
        (*swapCount)++;
    }
    return comparisons;
}

// Insertion Sort
long long int insertionSort(int arr[], int n, long long int *swapCount) {
    long long int comparisons = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            comparisons++;
            arr[j + 1] = arr[j];
            j--;
            (*swapCount)++;
        }
        comparisons++;
        arr[j + 1] = key;
    }
    return comparisons;
}

// Quick sort
int HoarePartition(int arr[], int l, int r, long long int *comparisonCount, long long int *swapCount) {
    int pivot = arr[l];
    int i = l;
    int j = r + 1;

    while (i < j) {
        do {
            i++;
            (*comparisonCount)++; // Count comparison for `arr[i] < pivot`
        } while (i <= r && arr[i] < pivot);

        do {
            j--;
            (*comparisonCount)++; // Count comparison for `arr[j] > pivot`
        } while (j >= l && arr[j] > pivot);

        if (i < j) {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            (*swapCount)++; // Count swap
        }
    }

    // Swap pivot into its correct position
    int temp = arr[l];
    arr[l] = arr[j];
    arr[j] = temp;
    (*swapCount)++; // Count swap

    return j;
}

void QuickSort(int arr[], int l, int r, long long int *comparisonCount, long long int *swapCount) {
    if (l < r) {
        int s = HoarePartition(arr, l, r, comparisonCount, swapCount);

        // Recursively apply QuickSort on the partitions
        QuickSort(arr, l, s - 1, comparisonCount, swapCount);
        QuickSort(arr, s + 1, r, comparisonCount, swapCount);
    }
}
// -- Wrapper Function for Measurement
long long int quickSort(int arr[], int n, long long int *swapCount) {
    long long int comparisonCount = 0;
    QuickSort(arr, 0, n - 1, &comparisonCount, swapCount);
    return comparisonCount;
}

// Merge Sort 
void merge(int arr[], int left, int mid, int right, long long int *comparisonCount, long long int *swapCount) {
    int range1 = mid - left;
    int range2 = right - mid;

    int L[range1], R[range2];

    // Copy elements to temporary arrays L and R
    for (int i = 0; i < range1; i++) {
        L[i] = arr[left + i];
    }
    for (int i = 0; i < range2; i++) {
        R[i] = arr[mid + i];
    }

    int i = 0, j = 0, k = left;

    // Merge the temporary arrays back into arr
    while (i < range1 && j < range2) {
        (*comparisonCount)++; // Count comparison
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        (*swapCount)++; // Count each assignment as a "swap"
    }

    // Copy remaining elements of L, if any
    while (i < range1) {
        arr[k] = L[i];
        i++;
        k++;
        (*swapCount)++; // Count assignment
    }

    // Copy remaining elements of R, if any
    while (j < range2) {
        arr[k] = R[j];
        j++;
        k++;
        (*swapCount)++; // Count assignment
    }
}

void BottomUpMergeSort(int arr[], int n, long long int *comparisonCount, long long int *swapCount) {
    if (n < 2) {
        return;
    }

    int i = 1;
    while (i < n) {
        int j = 0;
        while (j < n - i) {
            if (n < j + (2 * i)) {
                merge(arr, j, j + i, n, comparisonCount, swapCount);
            } else {
                merge(arr, j, j + i, j + (2 * i), comparisonCount, swapCount);
            }
            j += 2 * i;
        }
        i *= 2;
    }
}
// -- Wrapper Function for Measurement
long long int mergeSort(int arr[], int n, long long int *swapCount) {
    long long int comparisonCount = 0;
    BottomUpMergeSort(arr, n, &comparisonCount, swapCount);
    return comparisonCount;
}

// Random Array Generator
void generateRandomArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;  // Generate a random number between 0 and 999
    }
}

void generateSortedArray(int arr[], int size) {
    for (int i = 0; i < size; i++) arr[i] = i;
}

void generateReversedArray(int arr[], int size) {
    for (int i = 0; i < size; i++) arr[i] = size - i - 1;
}

// Measure Sorting Performance and Write to CSV
void writeCSVHeader(FILE *file) {
    fprintf(file, "Algorithm,Array Size,Input Type,Avg Time (seconds),Avg Comparisons,Avg Swaps\n");
}

// Measure Sorting Performance and Write to CSV
void measureSortingPerformance(FILE *file, const char *name, long long int (*sortFunc)(int[], int, long long int*), int n, const char *inputType) {
    long long int totalComparisons = 0, totalSwaps = 0;
    double totalTime = 0.0;

    for (int r = 0; r < 10; r++) {
        int *arr = malloc(n * sizeof(int));  // Allocate a new array for each round
        if (arr == NULL) {
            printf("Memory allocation failed!\n");
            return;
        }

        // Generate a new array based on the input type
        if (strcmp(inputType, "Sorted") == 0) {
            generateSortedArray(arr, n);
        } else if (strcmp(inputType, "Reversed") == 0) {
            generateReversedArray(arr, n);
        } else {
            generateRandomArray(arr, n);
        }

        struct timespec start, end;
        long long int swapCount = 0;

        clock_gettime(CLOCK_MONOTONIC, &start);
        long long int comparisons = sortFunc(arr, n, &swapCount);
        clock_gettime(CLOCK_MONOTONIC, &end);

        double timeTaken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

        totalComparisons += comparisons;
        totalSwaps += swapCount;
        totalTime += timeTaken;

        free(arr);  // Free the array after each round
    }

    // Compute averages
    double avgTime = totalTime / 10;
    long long avgComparisons = totalComparisons / 10;
    long long avgSwaps = totalSwaps / 10;

    printf("[result] %-15s - Avg Time: %.6f sec, Avg Comparisons: %lld, Avg Swaps: %lld\n", name, avgTime, avgComparisons, avgSwaps);
    fprintf(file, "%s,%d,%s,%.6f,%lld,%lld\n", name, n, inputType, avgTime, avgComparisons, avgSwaps);
}

// Main Function
int main() {
    srand(time(NULL));

    FILE *file = fopen("sorting_results.csv", "w");
    if (file == NULL) {
        printf("Error opening file for writing\n");
        return 1;
    }
    writeCSVHeader(file);

    int sizes[] = {200, 400, 600, 800, 1000,
                 2000, 4000, 6000, 8000, 10000,
                 20000, 40000, 60000, 80000, 100000};
    const char *inputTypes[] = {"Sorted", "Reversed", "Random"};
    const char *sortNames[] = {"Bubble Sort", "Selection Sort", "Insertion Sort", "Quick Sort", "Merge Sort"};
    long long int (*sortFuncs[])(int[], int, long long int*) = {bubbleSort, selectionSort, insertionSort, quickSort, mergeSort};

    for (int s = 0; s < 15; s++) {
        printf("\n==========================\n");
        printf("Array Size: %d\n", sizes[s]);
        printf("==========================\n");

        for (int t = 0; t < 3; t++) {
            printf("\nProcessing Input Type: %s\n", inputTypes[t]);

            for (int f = 0; f < 5; f++) {
                measureSortingPerformance(file, sortNames[f], sortFuncs[f], sizes[s], inputTypes[t]);
            }

            printf("\n");
        }
    }

    fclose(file);
    printf("\nResults written to sorting_results.csv\n");

    return 0;
}


