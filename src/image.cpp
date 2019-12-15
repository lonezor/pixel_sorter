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

#include "image.hpp"

//-----------------------------------------------------------------------------------------------------------

Image::Image()
{
    this->pixbuf     = NULL;
    this->width      = 0;
    this->height     = 0;
    this->nrChannels = 0;
}

Image::~Image()
{
    g_clear_object(&this->pixbuf);
}

//-----------------------------------------------------------------------------------------------------------

bool Image::loadFromFile(char* path)
{
    GError* err  = NULL;
    bool success = TRUE;

    this->pixbuf        = gdk_pixbuf_new_from_file(path, &err);
    if (!this->pixbuf) return FALSE;

    this->width         = gdk_pixbuf_get_width(this->pixbuf);
    this->height        = gdk_pixbuf_get_height(this->pixbuf);
    this->nrChannels    = gdk_pixbuf_get_n_channels(this->pixbuf);
    this->colorSpace    = gdk_pixbuf_get_colorspace(this->pixbuf);
    this->bitsPerSample = gdk_pixbuf_get_bits_per_sample(this->pixbuf);

    success &= (this->colorSpace    == GDK_COLORSPACE_RGB);
    success &= (this->bitsPerSample == 8);
    success &= (this->nrChannels    == 3);

    // used as buffer for raw pixel access, ensure it is allocated even though GTK does not reference it.
    g_object_ref(this->pixbuf); 

    return success;
}

//-----------------------------------------------------------------------------------------------------------

void Image::rescale(int width, int height)
{
    GdkPixbuf* pxBuf;

    pxBuf = gdk_pixbuf_scale_simple(this->pixbuf, width, height, GDK_INTERP_HYPER);

    g_clear_object(&this->pixbuf);
    this->pixbuf = pxBuf;
    this->width  = gdk_pixbuf_get_width(this->pixbuf);
    this->height = gdk_pixbuf_get_height(this->pixbuf);
}

//-----------------------------------------------------------------------------------------------------------

void* Image::getPixelAddress(int x, int y)
{
    int rowstride;
    guchar *pixels, *p;

    g_assert(x >= 0 && x < this->width);
    g_assert(y >= 0 && y < this->height);

    rowstride = gdk_pixbuf_get_rowstride(this->pixbuf);
    pixels    = gdk_pixbuf_get_pixels(this->pixbuf);
    
    p = pixels + y * rowstride + x * this->nrChannels; 

    return p;
}

//-----------------------------------------------------------------------------------------------------------

int Image::getPixelSize(void)
{
    int bytesPerSample = gdk_pixbuf_get_bits_per_sample (this->pixbuf) / 8;

    return this->nrChannels * bytesPerSample;
}

//-----------------------------------------------------------------------------------------------------------

void Image::setPixel(uint8_t* p, uint8_t red, uint8_t green, uint8_t blue)
{
    p[0] = red;
    p[1] = green;
    p[2] = blue;
}

//-----------------------------------------------------------------------------------------------------------

void Image::getPixel(uint8_t* p, uint8_t* red, uint8_t* green, uint8_t* blue)
{
    *red   = p[0];
    *green = p[1];
    *blue =  p[2];
}

//-----------------------------------------------------------------------------------------------------------

int Image::getWidth(void)
{
    return this->width;
}

//-----------------------------------------------------------------------------------------------------------

int Image::getHeight(void)
{
    return this->height;
}

//-----------------------------------------------------------------------------------------------------------

GtkWidget* Image::newGtkImage()
{
    return gtk_image_new_from_pixbuf(this->pixbuf);
}

//-----------------------------------------------------------------------------------------------------------

GdkPixbuf* Image::newGdkPixBuf()
{
    return gdk_pixbuf_copy(this->pixbuf);
}

//-----------------------------------------------------------------------------------------------------------

void Image::saveAsPng(char* path)
{
    GError* err = NULL;

    printf("Writing file %s\n", path);

    gdk_pixbuf_save(this->pixbuf,
                    path,
                    "png",
                    &err,
                    "compression", "6",
                    NULL);

    if (err != NULL) printf("gdk_pixbuf_save failed: %s\n", __FUNCTION__);
}

//-----------------------------------------------------------------------------------------------------------

