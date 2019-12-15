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

#ifndef _RADIX_SORT_H_
#define _RADIX_SORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h> 
#include <vector>

using namespace std;

template<class T>
class RadixSort
{
  public:

    static void radixsort(T* element, int nrElements)
    {
        int i, j, idx, maxM, value;
        int m        = 10;
        int n        = 1;
        int maxValue = INT_MIN;

        // Calculate maxM, which controls how many iterations are needed
        for(i=0; i < nrElements; i++)
        {
            value = element[i].getPos();
            if (value > maxValue) maxValue = value;
        }

        i = 1;
        while (maxValue > 0)
        {
            maxValue /= 10;
            i++;
        }
        maxM = (int)pow(10, i);

        // Allocate memory for out-of-place sorting buffers
        int       dataSize   = element[0].getSize();
        uint8_t*  dataBuffer = (uint8_t*)malloc(nrElements * dataSize);
        int       dataPos;
        T**       buckets[20];
        int       bucketPos[20];
        for(i=0; i < 20; i++) buckets[i] = (T**)malloc(nrElements * sizeof(T));

        // Sorting loop
        while (m < maxM)
        {
            dataPos = 0;
            for(i=0; i < 20; i++) bucketPos[i] = 0;
            
            // Put each element into suitable bucket
            for(i=0; i < nrElements; i++)
            {
                value = element[i].getPos();
                idx = (value % m) / n;
                idx += 10; // Negative numbers on the first 10 slots
                uint8_t* buf  = dataBuffer + (dataPos++ * dataSize);
                uint8_t* ePtr = (uint8_t*)buckets[idx] + (bucketPos[idx]++ * sizeof(T));
                *((T*)ePtr)   = element[i].copy(buf, element[i].getData());
            }

            // Use new sequence as foundation for next iteration. Overwrite elements[]
            idx = 0;
            for(i=0; i<20;i++)
            {
                for(j=0; j<bucketPos[i]; j++, idx++)
                {
                    uint8_t* ePtr = (uint8_t*)buckets[i] + (j * sizeof(T));
                    element[idx].swap(*((T*)ePtr));
                    //usleep(1); // Visualization: Align with speed of other algorithms
                }
            }
            m *= 10;
            n *= 10;
        }

        for(i=0; i < 20; i++) free(buckets[i]);
        free(dataBuffer);
    }

    static void sort(T* element, int nrElements)
    {
        radixsort(element, nrElements);
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _RADIX_SORT_H_ */

