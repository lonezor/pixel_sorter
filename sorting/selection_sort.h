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

#ifndef _SELECTION_SORT_H_
#define _SELECTION_SORT_H_

#include <unistd.h>

//-----------------------------------------------------------------------------------------------------------

template<class T>
class SelectionSort
{
  public:
    static void sort(T* element, int nrElements)
    {
        int i,j;
        bool swap;
        T* min;

        for(i=0; i<nrElements; i++)
        {
            swap = 0;
            min = &element[i];
            for(j=i; j<nrElements; j++)
            {
                if (element[j] < *min)
                {
                    min = &element[j];
                    swap = 1;
                }
            }
            if (swap) 
            {
                element[i].swap(*min);
            }
        }
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _SELECTION_SORT_H_ */

