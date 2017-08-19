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

#ifndef _SORTER_H_
#define _SORTER_H_

#include "algorithm_collection.h"

//-----------------------------------------------------------------------------------------------------------

class Sorter
{
public:
    virtual void randomize(void) = 0;
    virtual void sort(void)      = 0;
    virtual void setAlgorithm(algorithm_t algorithm) = 0;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _SORTER_H_ */
