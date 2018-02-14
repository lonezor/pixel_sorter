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

#include "pixel_sorter.h"
#include <stdio.h>
#include <assert.h>
#include <chrono>

using std::chrono::steady_clock;

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

void PixelSorter::whiteNoise()
{
    int i,j;

    // Disorganise pixels completely random
    for(i=0; i < this->nrPixels; i++)
    {
        j = rand() % this->nrPixels;
        this->pixels[i].swap(this->pixels[j]);
    }
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::scatter(int scatter)
{
    int i,j;

    // Disorganise pixels by scattering pixels close to each other randomly
    for(i=scatter; i < this->nrPixels && scatter; i++)
    {
        j = i - scatter + (rand() % scatter);
        this->pixels[i].swap(this->pixels[j]);
    }
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::sortByPosition(PixelPosition pixelPos)
{
    int i;

    // Sort pixels according to light intensity
    for(i=0; i < nrPixels; i++)
    {
        pixels[i].SetPixelPosition(pixelPos);
    }
    AlgorithmCollection::pixel_RadixSort.sort(pixels, nrPixels);

    // Reset position type so that sorting will be done on original sequence
    for(i=0; i < nrPixels; i++)
    {
        pixels[i].SetPixelPosition(PIXEL_POSITION_SEQUENCE);
    }
}

//-----------------------------------------------------------------------------------------------------------

void PixelSorter::sort()
{
    auto start = steady_clock::now();

    switch (algorithm)
    {
        // In-place comparative algorithms
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

        // Out-of-place non-comparative sorting algorithms
        case ALGORITHM_RADIX_SORT:
            AlgorithmCollection::pixel_RadixSort.sort(this->pixels, this->nrPixels);
            break;
        default:
            break;
    }

    auto end = steady_clock::now();
    double elapsedTime = ((end-start).count()) * steady_clock::period::num / static_cast<double>(steady_clock::period::den);
    if (elapsedTime < 1)
    {
        printf("Sorting time: %d ms\n", (int)(elapsedTime*1000));
    }
    else
    {
        long elapsedTimeMs, hours, minutes, seconds, ms;
        elapsedTimeMs  = (long)(elapsedTime * 1000);
        hours          = elapsedTimeMs / (3600 * 1000);
        elapsedTimeMs -= hours * 3600 * 1000;
        minutes        = elapsedTimeMs / (60*1000);
        elapsedTimeMs -= minutes * 60 * 1000;
        seconds        = elapsedTimeMs  / 1000;
        ms             = elapsedTimeMs % 1000;
        printf("Sorting time: %02ld:%02ld:%02ld:%03ld\n", hours, minutes, seconds, ms);
    }
}

//-----------------------------------------------------------------------------------------------------------

Image* PixelSorter::getImage()
{
    return this->image;
}

//-----------------------------------------------------------------------------------------------------------

