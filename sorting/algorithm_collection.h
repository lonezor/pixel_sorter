/*
 *  Pixel Sorter
 *  Copyright (C) 2017 Johan Norberg <lonezor@gmail.com>
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

#ifndef _ALGORITHM_COLLECTION_H_
#define _ALGORITHM_COLLECTION_H_

#include "integer.h"
#include "character.h"
#include "pixel.h"

#include "bubble_sort.h"
#include "insertion_sort.h"
#include "quick_sort.h"
#include "selection_sort.h"
#include "shaker_sort.h"
#include "shell_sort.h"
#include "radix_sort.h"

//-----------------------------------------------------------------------------------------------------------

typedef enum
{
  ALGORITHM_UNKNOWN = -1,
  ALGORITHM_BUBBLE_SORT,
  ALGORITHM_INSERTION_SORT,
  ALGORITHM_QUICKSORT,
  ALGORITHM_RADIX_SORT,
  ALGORITHM_SELECTION_SORT,
  ALGORITHM_SHAKER_SORT,
  ALGORITHM_SHELL_SORT,
  ALGORITHM_NR_ENTRIES,
  ALGORITHM_DEFAULT = ALGORITHM_SHELL_SORT,
} algorithm_t;

typedef enum name
{
  SORT_TYPE_UNKNOWN,
  SORT_TYPE_IMAGE,
  SORT_TYPE_NUMBER,
  SORT_TYPE_TEXT,
} sort_type_t;

//-----------------------------------------------------------------------------------------------------------

// Convenient access to all template expanded sorting implementations
class AlgorithmCollection
{
public:
    static BubbleSort<Pixel>        pixel_BubbleSort;
    static InsertionSort<Pixel>     pixel_InsertionSort;
    static QuickSort<Pixel>         pixel_QuickSort;
    static SelectionSort<Pixel>     pixel_SelectionSort;
    static ShakerSort<Pixel>        pixel_ShakerSort;
    static ShellSort<Pixel>         pixel_ShellSort;
    static RadixSort<Pixel>         pixel_RadixSort;
    static BubbleSort<Integer>      integer_BubbleSort;
    static InsertionSort<Integer>   integer_InsertionSort;
    static QuickSort<Integer>       integer_QuickSort;
    static SelectionSort<Integer>   integer_SelectionSort;
    static ShakerSort<Integer>      integer_ShakerSort;
    static ShellSort<Integer>       integer_ShellSort;
    static RadixSort<Integer>       integer_RadixSort;
    static BubbleSort<Character>    character_BubbleSort;
    static InsertionSort<Character> character_InsertionSort;
    static QuickSort<Character>     character_QuickSort;
    static SelectionSort<Character> character_SelectionSort;
    static ShakerSort<Character>    character_ShakerSort;
    static ShellSort<Character>     character_ShellSort;
    static RadixSort<Character>     character_RadixSort;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _ALGORITHM_COLLECTION_H_ */
