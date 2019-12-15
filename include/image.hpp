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

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>
#include <gtk/gtk.h>

//-----------------------------------------------------------------------------------------------------------

class Image
{
public:
    Image();
    ~Image();

    bool loadFromFile(char* path);
    void rescale(int width, int height);

    int   getWidth(void);
    int   getHeight(void);
    void* getPixelAddress(int x, int y);
    int   getPixelSize(void);
    void  getPixel(uint8_t* p, uint8_t* red, uint8_t* green, uint8_t* blue);
    void  setPixel(uint8_t* p, uint8_t red, uint8_t green, uint8_t blue);
    
    GtkWidget* newGtkImage();
    GdkPixbuf* newGdkPixBuf();
    void       saveAsPng(char* path);

private:
    GdkPixbuf* pixbuf;
    int        width;
    int        height;
    int        nrChannels;
    int        bitsPerSample;
    int        colorSpace;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _IMAGE_H_ */