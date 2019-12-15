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

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <wchar.h> // Supporting unicode we cannot use variable length UTF-8 to
                   // swap two characters since they must have the same size
                   // Therefore wchar_t is used. It can be translated to UTF-8
                   // just before displaying the string.
#include <unistd.h>
#include <string.h>

extern void characterSwapCb(void);

//-----------------------------------------------------------------------------------------------------------

class Character
{
  public:
    Character()
    {

    }
  
    Character(void* data, int size)
    {
        this->data     = (wchar_t*)data;
        this->size     = size;
        this->cmpIndex = -1; // sort in ascending order
    }

    Character(void* data, int size, int cmpIndex)
    {
        this->data     = (wchar_t*)data;
        this->size     = size;
        this->cmpIndex = cmpIndex; // sort according to reference
    }

    void setData(void* data)
    {
        this->data = (wchar_t*) data;
    }

    void setCmpIndex(int cmpIndex)
    {
        this->cmpIndex = cmpIndex;
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

    Character copy(void* dst, void* src)
    {
        memmove(dst, src, this->size);
        return Character(dst, this->size, this->cmpIndex);
    }

    void swap(Character& other)
    {
        wchar_t temp = *((wchar_t*)this->data);
        int cmpIndex = this->cmpIndex;
        memmove(this->data, other.data, this->size);
        memmove(other.data, &temp, this->size);
        this->cmpIndex = other.cmpIndex;
        other.cmpIndex = cmpIndex;

        characterSwapCb();
    }

    bool operator==(const Character& rhs)
    {
        if (this->cmpIndex != -1)
        {
            int lhsIndex = this->cmpIndex;
            int rhsIndex = rhs.cmpIndex;
            return lhsIndex == rhsIndex;
        }
        else
        {
            const wchar_t* pLhs = (const wchar_t*)&this->data;
            const wchar_t* pRhs = (const wchar_t*)rhs.data;
            return (*pLhs == *pRhs);
        }
    }

    bool operator!=(const Character& rhs)
    {
        return  !(*this == rhs);
    }

    bool operator< (const Character& rhs)
    {
        if (this->cmpIndex != -1)
        {
            int lhsIndex = this->cmpIndex;
            int rhsIndex = rhs.cmpIndex;
            return lhsIndex < rhsIndex;
        }
        else
        {
            const wchar_t* pLhs = (const wchar_t*)this->data;
            const wchar_t* pRhs = (const wchar_t*)rhs.data;
            return (*pLhs < *pRhs);
        }
    }

    bool operator> (const Character& rhs)
    {
        if (this->cmpIndex != -1)
        {
            int lhsIndex = this->cmpIndex;
            int rhsIndex = rhs.cmpIndex;
            return lhsIndex > rhsIndex;
        }
        else
        {
            const wchar_t* pLhs = (const wchar_t*)this->data;
            const wchar_t* pRhs = (const wchar_t*)rhs.data;
            return (*pLhs > *pRhs);
        }
    }

    bool operator<=(const Character& rhs)
    {
        return  !(*this > rhs);
    }

    bool operator>=(const Character& rhs)
    {
        return  !(*this < rhs);
    }
    
  private:
    wchar_t* data;
    int      size;
    int      cmpIndex;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _CHARACTER_H_ */
