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

#ifndef _PIXEL_H_
#define _PIXEL_H_

#include <string.h>
#include <stdio.h>
#include <stdint.h>

extern void pixelSwapCb(void);

//-----------------------------------------------------------------------------------------------------------

class Pixel
{
    public:
        Pixel()
        {

        }
    
        Pixel(void* data, int size)
        {
            this->data = data;
            this->size = size;
        }

        Pixel(void* data, int size, int x, int y, int maxX)
        {
            this->data = data;
            this->size = size;
            this->x = x;
            this->y = y;
            this->maxX = maxX;
        }

        Pixel copy(void* dst, void* src)
        {
            memmove(dst, src, this->size);
            return Pixel(dst, this->size, this->x, this->y, this->maxX);
        }

        int getSize(void)
        {
            return this->size;
        }

        void setSize(int size)
        {
            this->size = size;
        }

        void* getData(void)
        {
            return this->data;
        }

        void setData(void* data)
        {
            this->data = data;
        }

        void setXY(int x, int y)
        {
            this->x = x;
            this->y = y;   
        }

        void getXY(int* x, int* y)
        {
            if (!x || !y) return;

            *x = this->x;
            *y = this->y;
        }

        int getPos()
        {
            return this->x + (this->y * this->maxX);
        }

        void swap(Pixel& other)
        {
            if (!this->data || !other.data) 
            {
                printf("no data\n");
                return;
            }

            uint8_t temp[this->size];
            int x = this->x;
            int y = this->y;

            memmove(temp, this->data, this->size);
            memmove(this->data, other.data, other.size);
            memmove(other.data, temp, sizeof(temp));
            
            this->x = other.x;
            this->y = other.y;
            other.x = x;
            other.y = y;

            pixelSwapCb();
        }

        bool operator==(const Pixel& rhs)
        {

            return (this->x == rhs.x && this->y == rhs.y);
        }

        bool operator!=(const Pixel& rhs)
        {
            return  !(*this == rhs);
        }

        bool operator< (const Pixel& rhs)
        {
            int pos = getPos();
            int otherPos = rhs.x + (rhs.y * rhs.maxX);
            return (pos < otherPos);
        }

        bool operator> (const Pixel& rhs)
        {
            int pos = getPos();
            int otherPos = rhs.x + (rhs.y * rhs.maxX);
            return (pos > otherPos);
        }

        bool operator<=(const Pixel& rhs)
        {
            return  !(*this > rhs);
        }

        bool operator>=(const Pixel& rhs)
        {
            return  !(*this < rhs);
        }

    private:
        int   x;
        int   y;
        int   maxX;
        void* data;
        int   size;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _PIXEL_H_ */

