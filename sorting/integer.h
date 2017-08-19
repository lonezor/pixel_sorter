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

#ifndef _INTEGER_H_
#define _INTEGER_H_

#include <string.h>

extern void integerSwapCb(void);

//-----------------------------------------------------------------------------------------------------------

class Integer
{
  public:
    Integer()
    {

    }
  
    Integer(void* data, int size)
    {
        this->data = data;
        this->size = size;
    }

    void setData(void* data)
    {
        this->data = data;
    }
    
    void setSize(int size)
    {
        this->size = size;
    }

    void* getData(void)
    {
        return this->data;
    }

    int getSize(void)
    {
        return this->size;
    }

    Integer copy(void* dst, void* src)
    {
        memcpy(dst, src, this->size);
        return Integer(dst, this->size);
    }

    void swap(Integer& other)
    {
        int temp = *((int*)this->data);
        memcpy(this->data, other.data, this->size);
        memcpy(other.data, &temp, this->size);
    }

    bool operator==(const Integer& rhs)
    {
        const int* pLhs = (const int*)this->data;
        const int* pRhs = (const int*)rhs.data;
        return (*pLhs == *pRhs);
    }

    bool operator!=(const Integer& rhs)
    {
        return  !(*this == rhs);
    }

    bool operator< (const Integer& rhs)
    {
        const int* pLhs = (const int*)this->data;
        const int* pRhs = (const int*)rhs.data;
        return (*pLhs < *pRhs);
    }

    bool operator> (const Integer& rhs)
    {
        const int* pLhs = (const int*)this->data;
        const int* pRhs = (const int*)rhs.data;
        return (*pLhs > *pRhs);
    }

    bool operator<=(const Integer& rhs)
    {
        return  !(*this > rhs);
    }

    bool operator>=(const Integer& rhs)
    {
        return  !(*this < rhs);
    }
    
  private:
    void* data;
    int   size;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _INTEGER_H_ */

