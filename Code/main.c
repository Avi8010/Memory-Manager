#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void runTestCases();

int main()
{
    runTestCases();
    return 0;
}

void runTestCases()
{
    
    // Test Case 1: All Allocated Blocks
     int* a = (int*)malloc2(4* sizeof(int));
     
    char* b = (char*)calloc2(4, sizeof(char));
    printf("Test Case 1: All Allocated Blocks\n");
    printHeap();

    // Test Case 2: All Freed Blocks
    free2(a);
    free2(b);
    printf("\nTest Case 2: All Freed Blocks\n");
    printHeap();

    // Test Case 3: Mix of Allocated and Freed Blocks
    a = (int*)malloc2(4 * sizeof(int));
    b = (char*)calloc2(30, sizeof(char));
    free2(a);
    printf("\nTest Case 3: Mix of Allocated and Freed Blocks\n");
    printHeap();

    // Test Case 4: No Allocated Blocks
    free2(b);
    printf("\nTest Case 4: No Allocated Blocks\n");
    printHeap();

    // Test Case 5: Single Block Allocation and Deallocation
    
    printf("\nTest Case 5: Single Block Allocation and Deallocation\n");
    int* c = (int*)malloc2(sizeof(int));
    printHeap();
    free2(c);
    printf("\n");
    printHeap();

    // Test Case 6: Large Block Allocation
    int* d = (int*)malloc2(10000 * sizeof(int));
    int* e = (int*)malloc2(10000 * sizeof(int));
    int* g = (int*)malloc2(10000 * sizeof(int));
    printf("\nTest Case 6: Large Block Allocation\n");
    printHeap();

    // Clean up
    free2(d);
}

