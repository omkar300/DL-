#include<iostream>
#include<omp.h>
#include<cstdlib>
#include<ctime>

using namespace std;

void bubble(int array[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) 
                swap(array[j], array[j + 1]);
        }
    }
}

void pBubble(int array[], int n) {
    // Sort odd indexed numbers
    for(int i = 0; i < n; ++i) {    
        #pragma omp for
        for (int j = 1; j < n; j += 2) {
            if (array[j] < array[j-1]) {
                swap(array[j], array[j - 1]);
            }
        }

        // Synchronize
        #pragma omp barrier

        // Sort even indexed numbers
        #pragma omp for
        for (int j = 2; j < n; j += 2) {
            if (array[j] < array[j-1]) {
                swap(array[j], array[j - 1]);
            }
        }
    }
}

void printArray(int arr[], int n) {
    for(int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";
}

int main() {
    // Set up variables
    int n = 10;
    int arr[n], brr[n];
    double start_time, end_time;

    // Seed the random number generator
    srand(time(0));

    // Create a random array of size n
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 100; // Random number between 0 and 99
    }

    cout << "Unsorted array: ";
    printArray(arr, n);
    
    // Sequential time
    for(int i = 0; i < n; i++) brr[i] = arr[i]; // Copy to preserve original array
    start_time = omp_get_wtime();
    bubble(brr, n);
    end_time = omp_get_wtime();     
    cout << "Sequential Bubble Sort took : " << end_time - start_time << " seconds.\n";
    cout << "Sorted array using Sequential Bubble Sort: ";
    printArray(brr, n);
    
    // Reset the array
    for(int i = 0; i < n; i++) arr[i] = rand() % 100; // Generate new random values for the array
    
    // Parallel time
    for(int i = 0; i < n; i++) brr[i] = arr[i]; // Copy to preserve original array
    start_time = omp_get_wtime();
    pBubble(brr, n);
    end_time = omp_get_wtime();     
    cout << "Parallel Bubble Sort took : " << end_time - start_time << " seconds.\n";
    cout << "Sorted array using Parallel Bubble Sort: ";
    printArray(brr, n);

    return 0;
}
