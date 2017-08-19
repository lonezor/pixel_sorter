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

#ifndef _BUBBLE_SORT_H_
#define _BUBBLE_SORT_H_

#include <unistd.h>

//-----------------------------------------------------------------------------------------------------------

template<class T>
class BubbleSort
{
  public:
    static void sort(T* element, int nrElements)
    {
        int i;
        bool swap;
        do
        {
            swap = 0;
            for(i=0; i<nrElements-1; i++)
            {
                if (element[i] > element[i+1])
                {
                    element[i].swap(element[i+1]);
                    swap = 1;
                }
            }
        } while (swap);
    }
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _BUBBLE_SORT_H_ */

