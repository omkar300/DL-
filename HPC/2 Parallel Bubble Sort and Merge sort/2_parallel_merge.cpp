#include <iostream>
#include <omp.h>
#include <cstdlib>  // For generating random numbers
#include <ctime>    // For seeding the random number generator

using namespace std;

void merge(int arr[], int low, int mid, int high) {
    // Create arrays of left and right partitions
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int left[n1];
    int right[n2];

    // Copy all left elements
    for (int i = 0; i < n1; i++) left[i] = arr[low + i];

    // Copy all right elements
    for (int j = 0; j < n2; j++) right[j] = arr[mid + 1 + j];

    // Compare and place elements
    int i = 0, j = 0, k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    // If any elements are left in left array
    while (i < n1) {
        arr[k] = left[i];
        i++;
        k++;
    }

    // If any elements are left in right array
    while (j < n2) {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void parallelMergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, low, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, high);
            }
        }
        merge(arr, low, mid, high);
    }
}

void mergeSort(int arr[], int low, int high) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
}

// Function to print the array
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    int n = 10;
    int arr[n];
    double start_time, end_time;

    // Seed the random number generator
    srand(time(0));

    // Generate a random array
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;  // Random number between 0 and 99
    }

    // Display the unsorted array
    cout << "Unsorted array: ";
    printArray(arr, n);

    // Measure Sequential Time
    int seqArr[n];
    for (int i = 0; i < n; i++) seqArr[i] = arr[i];  // Copy original array to preserve it
    start_time = omp_get_wtime();
    mergeSort(seqArr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Time taken by sequential algorithm: " << end_time - start_time << " seconds\n";

    // Print the sorted array using sequential merge sort
    cout << "Sorted array using sequential merge sort: ";
    printArray(seqArr, n);

    // Reset the array
    for (int i = 0; i < n; i++) arr[i] = rand() % 100;  // Generate new random array

    // Measure Parallel Time
    int parArr[n];
    for (int i = 0; i < n; i++) parArr[i] = arr[i];  // Copy original array to preserve it
    start_time = omp_get_wtime();
    parallelMergeSort(parArr, 0, n - 1);
    end_time = omp_get_wtime();
    cout << "Time taken by parallel algorithm: " << end_time - start_time << " seconds\n";

    // Print the sorted array using parallel merge sort
    cout << "Sorted array using parallel merge sort: ";
    printArray(parArr, n);

    return 0;
}
