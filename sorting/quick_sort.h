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

#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------------------------------------

template<class T>
class QuickSort
{
  public:

    static void quicksort(T* element, int nrElements)
    {
        if (nrElements < 2) return;

        int lSelector = 0;
        int rSelector = nrElements - 1;

        // Select random pivot. Use copy as reference since the original will be swapped around.
        int pivotIndex = rand() % nrElements;
        uint8_t data[element[0].getSize()];
        T pivot = element[pivotIndex].copy(data, element[pivotIndex].getData());
   
        while (lSelector < rSelector)
        {
            while (element[lSelector] < pivot) lSelector++;
            while (element[rSelector] > pivot) rSelector--;
            element[lSelector].swap(element[rSelector]);
            usleep(1); // Visualization: Align with speed of other algorithms
        }
       quicksort(&element[0], lSelector);
       quicksort(&element[lSelector + 1], nrElements - 1 - lSelector); 
    }

    static void sort(T* element, int nrElements)
    {
        quicksort(element, nrElements);
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _QUICK_SORT_H_ */