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

#ifndef _SHELL_SORT_H_
#define _SHELL_SORT_H_

#include <unistd.h>

template<class T>
class ShellSort
{
  public:
    static void sort(T* element, int nrElements)
    {
        int gap;
        int i,j; // forward
        int k,l; // backward

        for(gap = nrElements / 2; gap > 0; gap /= 2 )
        {
            for(i=0, j = gap; j<nrElements; i++, j=i+gap)
            {
                if (element[i] > element[j])
                {
                    element[i].swap(element[j]);

                    l = i-gap;
                    for(k=i; l>=0; k-=gap, l-=gap)
                    {
                        if (element[l] > element[k])
                        {
                            element[l].swap(element[k]);
                        }
                    }
                }
            }
        }
    }
};

#endif /* _SHELL_SORT_H_ */