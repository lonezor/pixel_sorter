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

#include <stdio.h>

#include "pixel_sorter.h"

#include <assert.h>

//-----------------------------------------------------------------------------------------------------------

PixelSorter::PixelSorter(Image* image)
{
    int i;
    int x,y;

    this->width     = image->getWidth();
    this->height    = image->getHeight();
    this->nrPixels  = this->width * this->height;
    this->algorithm = ALGORITHM_DEFAULT;
    this->image     = image;

    // Allocate flat array of pixel meta objects for whole image
    this->pixels = (Pixel*) malloc(this->nrPixels * sizeof(Pixel));
    assert(pixels);

    for(i=0, y=0; y<this->height; y++)
    {
        for(x=0; x<this->width; x++, i++)
        {
            this->pixels[i] = Pixel(image->getPixelAddress(x,y), image->getPixelSize(), x, y, this->width);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------

PixelSorter::~PixelSorter()
{
    free(this->pixels);
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::setAlgorithm(algorithm_t algorithm)
{
    this->algorithm = algorithm;
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::randomize(void)
{
    int i,j;

    // Randomize image so that sorting can be performed
    for(i=0; i < this->nrPixels; i++)
    {
        j = rand() % this->nrPixels;
        this->pixels[i].swap(this->pixels[j]);
    }
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::sort()
{
    switch (algorithm)
    {
        case ALGORITHM_BUBBLE_SORT:
            AlgorithmCollection::pixel_BubbleSort.sort(this->pixels, this->nrPixels);
            break;
        case ALGORITHM_INSERTION_SORT:
            AlgorithmCollection::pixel_InsertionSort.sort(this->pixels, this->nrPixels);
            break;
        case ALGORITHM_QUICKSORT:
            AlgorithmCollection::pixel_QuickSort.sort(this->pixels, this->nrPixels);
            break;
        case ALGORITHM_SELECTION_SORT:
            AlgorithmCollection::pixel_SelectionSort.sort(this->pixels, this->nrPixels);
            break;
        case ALGORITHM_SHAKER_SORT:
            AlgorithmCollection::pixel_ShakerSort.sort(this->pixels, this->nrPixels);
            break;
        case ALGORITHM_SHELL_SORT:
            AlgorithmCollection::pixel_ShellSort.sort(this->pixels, this->nrPixels);
            break;
        default:
            break;
    }
}

//-----------------------------------------------------------------------------------------------------------

Image* PixelSorter::getImage()
{
    return this->image;
}

//-----------------------------------------------------------------------------------------------------------

