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

typedef enum
{
    PIXEL_POSITION_SEQUENCE,          // Sequence number from first pixel to last
    PIXEL_POSITION_LIGHT_INTENSITY,   // Sequence number based on light intensity (no differentiation between colors)
    PIXEL_POSITION_HUE,               // Hue color separation
} PixelPosition;

//-----------------------------------------------------------------------------------------------------------

class Pixel
{
    public:
        Pixel()
        {

        }
    
        Pixel(void* data, int size)
        {
            this->data     = data;
            this->size     = size;
            this->pixelPos = PIXEL_POSITION_SEQUENCE;
        }

        Pixel(void* data, int size, int x, int y, int maxX)
        {
            this->data     = data;
            this->size     = size;
            this->x        = x;
            this->y        = y;
            this->maxX     = maxX;
            this->pixelPos = PIXEL_POSITION_SEQUENCE;
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

        void setRgb(uint8_t red, uint8_t green, uint8_t blue)
        {
            uint8_t* p = (uint8_t*)data;
            p[0]       = red;
            p[1]       = green;
            p[2]       = blue;
        }

        void getRgb(uint8_t* red, uint8_t* green, uint8_t* blue)
        {
            uint8_t* p = (uint8_t*)data;
            *red       = p[0];
            *green     = p[1];
            *blue      = p[2];
        }

        void SetPixelPosition(PixelPosition pixelPos)
        {
            this->pixelPos = pixelPos;
        }

        int getPos()
        {
            switch (pixelPos)
            {
                case PIXEL_POSITION_LIGHT_INTENSITY:
                {
                    uint8_t red, green, blue;
                    getRgb(&red, &green, &blue);
                    return red + green + blue;
                }
                case PIXEL_POSITION_HUE:
                {
                    uint8_t red, green, blue;
                    getRgb(&red, &green, &blue);

                    double r = (double)red   / 255;
                    double g = (double)green / 255;
                    double b = (double)blue  / 255;
                    double min = 1.0;
                    double max = 0.0;
                    double diff;
                    double hue = 0;

                    typedef enum
                    {
                        RED,
                        GREEN,
                        BLUE,
                    } Color;
                    Color maxColor = RED;

                    if (r < min) min = r;
                    if (g < min) min = g;
                    if (b < min) min = b;

                    if (r > max)
                    {
                        max = r;
                        maxColor = RED;
                    } 
                    if (g > max)
                    {
                        max = g;
                        maxColor = GREEN;
                    }
                    if (b > max) 
                    {
                        max = b;
                        maxColor = BLUE;
                    }

                    diff = max - min;

                    if (diff >= -0.0001 && diff <= 0.0001) return 0; // avoid division by zero

                    if      (maxColor == RED)   hue = (g-b) / diff;
                    else if (maxColor == GREEN) hue = 2.0 + (b-r) / diff;
                    else if (maxColor == BLUE)  hue = 4.0 + (r-g) / diff;

                    hue *= 60; // to degrees
                    if (hue < 0) hue += 360;

                    hue *= 100000;
                    return (int)hue;
                }
                case PIXEL_POSITION_SEQUENCE:
                default:
                    return this->x + (this->y * this->maxX);
            }
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
        int            x;
        int            y;
        int            maxX;
        void*          data;
        int            size;
        PixelPosition  pixelPos;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _PIXEL_H_ */

