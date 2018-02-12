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
#include <vector>

using namespace std;

template<class T>
class RadixSort
{
  public:

    typedef struct
    {
        uint8_t* buffer;
        T        entry;
    } Element;

    static void radixsort(T* element, int nrElements, int maxValue) // TODO: handle maxValue near INT_MAX
    {
        int    i;
        size_t j;
        int    maxM;
        int    m = 10;
        int    n = 1;

        // Calculate maxM, which controls how many iterations are needed
        i = 1;
        while (maxValue > 0)
        {
            maxValue /= 10;
            i++;
        }
        maxM = (int)pow(10, i);

        while (m < maxM)
        {
            int idx;
            int dataSize = element[0].getSize();
            vector<Element*> buckets[10];

            // Put each element into suitable bucket
            for(i=0; i<nrElements;i++)
            {
                Element* e = (Element*) malloc(sizeof(Element));
                e->buffer = (uint8_t*)  malloc(dataSize);
                e->entry = element[i].copy(e->buffer, element[i].getData());
                idx = (element[i].getPos() % m) / n;
                buckets[idx].push_back(e);
            }

            // Use new sequence as foundation for next iteration. Overwrite elements[]
            idx = 0;
            for(i=0; i<10;i++)
            {
                for(j=0; j<buckets[i].size(); j++)
                {
                    Element* e = buckets[i][j];
                    element[idx].swap(e->entry);
                    //usleep(1); // Visualization: Align with speed of other algorithms
                    free(e->buffer);
                    free(e);
                    idx++;
                }
            }
            m *= 10;
            n *= 10;
        }
    }

    static void sort(T* element, int nrElements, int maxValue)
    {
        radixsort(element, nrElements, maxValue);
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _QUICK_SORT_H_ */
