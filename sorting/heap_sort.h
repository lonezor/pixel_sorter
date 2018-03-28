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

#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_

#include <unistd.h>
#include <stdlib.h>

// Zero based index:
#define LEFT_CHILD_INDEX(parentIdx)  (2 * parentIdx + 1)
#define RIGHT_CHILD_INDEX(parentIdx) (2 * parentIdx + 2)
#define PARENT_NODE(childIndex)      ((childIndex-1) / 2) // Integer division acts as floor() around expression

//-----------------------------------------------------------------------------------------------------------

template<class T>
class HeapSort
{
  public:

    static void Heapify(T* element, int nrElements, int idx)
    {
        int leftIndex  = LEFT_CHILD_INDEX(idx);
        int rightIndex = RIGHT_CHILD_INDEX(idx);
        int largestIndex = idx;
        
        if (idx >= nrElements) return;

        T&  parent = element[idx];
        T*  largest = &parent;

        if (leftIndex < nrElements)
        {
            T&  leftChild = element[leftIndex];
            if (leftChild > parent)
            {
                largest = &leftChild;
                largestIndex = leftIndex;
            }
        }

        if (rightIndex < nrElements)
        {
            T&  rightChild = element[rightIndex];
            if (rightChild > *largest)
            {
                largest = &rightChild;
                largestIndex = rightIndex;
            } 
        }

        if (largest != &parent)
        {
            parent.swap(*largest);
            Heapify(element, nrElements, largestIndex);
        }
    }

    static void Insert(T* element, int idx)
    {
        int parentIdx;

        while ((parentIdx = PARENT_NODE(idx)) >= 0)
        {
            
            if (element[idx] > element[parentIdx]) element[idx].swap(element[parentIdx]);
            idx = parentIdx;
            if (parentIdx == 0) break;
        }
    }

    static void sort(T* element, int nrElements)
    {
        int i;

        // Build Heap
        for(i=0; i < nrElements; i++) Insert(element, i);

        // Heap Sort
        while(nrElements)
        {
            nrElements--;
            element[0].swap(element[nrElements]);
            Heapify(element, nrElements, 0);
        }
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _HEAP_SORT_H_ */

