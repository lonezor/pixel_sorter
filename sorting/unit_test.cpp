/*
 *  Pixel Sorter
 *  Copyright (C) 2018 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#include "unit_test.h"

#include "algorithm_collection.h"

#include <assert.h>
#include <limits.h>


static const int intArray_01[] = {1};
static const int intArray_02[] = {2,1};
static const int intArray_03[] = {3,2,1};
static const int intArray_04[] = {4,3,2,1};
static const int intArray_05[] = {5,4,3,2,1};
static const int intArray_06[] = {5,2,4,10,3,1,7,9,6,8};
static const int intArray_07[] = {5,11,19,2,4,14,15,10,3,20,1,7,9,12,16,6,8,17,13,18};
static const int intArray_08[] = {53,11,190,2,4,134,154,10,3,2000,1,7,990,12,176,688,8,17,13,0,128};

//#define VERBOSE (1)

static int UT_integer_test(algorithm_t algorithm, int* pass, int* total)
{
    int      result = 1;
    int      nrEntries;
    int*     ints;
    int      i,j;
    Integer* integers;

    const int* input[] = { intArray_01,
                           intArray_02,
                           intArray_03,
                           intArray_04,
                           intArray_05,
                           intArray_06,
                           intArray_07,
                           intArray_08};

    const int  sizes[] = { sizeof(intArray_01),
                           sizeof(intArray_02),
                           sizeof(intArray_03),
                           sizeof(intArray_04),
                           sizeof(intArray_05),
                           sizeof(intArray_06),
                           sizeof(intArray_07),
                           sizeof(intArray_08)};

    int nrTests = sizeof(input) / sizeof(int*);

    *total = nrTests;
    *pass = 0;

    for(i = 0; i < nrTests; i++)
    {
        // Prepare integer buffer of actual data
        nrEntries = sizes[i] / sizeof(int);
        ints    = (int*)malloc(nrEntries * sizeof(int));
        assert(ints);

        // Copy from reference
        memcpy(ints, input[i], sizes[i]);

        // Prepare meta data objects
        integers = (Integer*)malloc(nrEntries * sizeof(Integer));
        assert(integers);

        for(j=0; j < nrEntries; j++)
        {
            integers[j] = Integer(&ints[j], sizeof(int));
        }

        #ifdef VERBOSE
        printf("Input:\n");
        for(j=0; j < nrEntries; j++)
        {
        printf(" %d\n", ints[j]);
        }
        printf("\n");
        #endif

        // Sort
        switch (algorithm)
        {
            // In-place comparative algorithms
            case ALGORITHM_BUBBLE_SORT:
                #ifdef VERBOSE
                printf("Algorithm: Bubble Sort\n");
                #endif
                AlgorithmCollection::integer_BubbleSort.sort(integers, nrEntries);
                break;
            case ALGORITHM_INSERTION_SORT:
                #ifdef VERBOSE
                printf("Algorithm: Insertion Sort\n");
                #endif
                AlgorithmCollection::integer_InsertionSort.sort(integers, nrEntries);
                break;
            case ALGORITHM_QUICKSORT:
                #ifdef VERBOSE
                printf("Algorithm: Quicksort\n");
                #endif
                AlgorithmCollection::integer_QuickSort.sort(integers, nrEntries);
                break;
            case ALGORITHM_SELECTION_SORT:
                #ifdef VERBOSE
                printf("Algorithm: Selection Sort\n");
                #endif
                AlgorithmCollection::integer_SelectionSort.sort(integers, nrEntries);
                break;
            case ALGORITHM_SHAKER_SORT:
                 #ifdef VERBOSE
                printf("Algorithm: Shaker Sort\n");
                #endif
                AlgorithmCollection::integer_ShakerSort.sort(integers, nrEntries);
                break;
            case ALGORITHM_SHELL_SORT:
                #ifdef VERBOSE
                printf("Algorithm: Shell Sort\n");
                #endif
                AlgorithmCollection::integer_ShellSort.sort(integers, nrEntries);
                break;

             // Out-of-place non-comparative sorting algorithms
            case ALGORITHM_RADIX_SORT:
                #ifdef VERBOSE
                printf("Algorithm: Radix Sort\n");
                #endif
                AlgorithmCollection::integer_RadixSort.sort(integers, nrEntries, 1000000);
                break;
            default:
                break;
        }

        #ifdef VERBOSE
        printf("\nOutput:\n");
        for(j=0; j < nrEntries; j++)
        {
            printf(" %d\n", ints[j]);
        }
        printf("\n");
        #endif

        // Check that numbers are in ascending order
        bool testCase = true;
        for(j=0; j < nrEntries-1; j++)
        {
            if (ints[j+1] < ints[j])
            {
                result = 0;
                testCase = false;
            }
        }
        if (testCase)
        {
            *pass += 1;
        }

        #ifdef VERBOSE
        printf("Conclusion: %s\n\n", testCase ? "OK" : "FAIL");
        #endif

        free(ints);
        free(integers);
    }

    return result;
}

static int UT_Integer_BubbleSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_BUBBLE_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Bubble Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}

static int UT_Integer_InsertionSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_INSERTION_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Insertion Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}

static int UT_Integer_QuickSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_QUICKSORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Quicksort:", result ? "OK" : "FAIL", pass, total);
    return result;
}


static int UT_Integer_SelectionSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_SELECTION_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Selection Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}


static int UT_Integer_ShakerSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_SHAKER_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Shaker Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}


static int UT_Integer_ShellSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_SHELL_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Shell Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}


static int UT_Integer_RadixSort()
{
    int pass, total;
    int result = UT_integer_test(ALGORITHM_RADIX_SORT, &pass, &total);
    printf(" - %-23s %s (%d/%d)\n", "Integer Radix Sort:", result ? "OK" : "FAIL", pass, total);
    return result;
}


int RunUnitTests()
{
    int res = 1;

    printf("===========\n");
    printf("Unit Tests:\n");
    printf("===========\n");

    res &= UT_Integer_BubbleSort();
    res &= UT_Integer_InsertionSort();
    res &= UT_Integer_QuickSort();
    res &= UT_Integer_SelectionSort();
    res &= UT_Integer_ShakerSort(); // TODO: fix! (case with very few entries in list)
    res &= UT_Integer_ShellSort();
    res &= UT_Integer_RadixSort();

    return res == 1 ? 0 : 1;
}

