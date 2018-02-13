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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <iconv.h>
#include <errno.h>
#include <gtk/gtk.h>
#include <ctype.h>

#include "image.h"
#include "pixel_sorter.h"
#include "gtk_image_window.h"
#include "thread.h"

#ifdef UNIT_TEST
#include "unit_test.h"
#endif

static const char* algorithmName[ALGORITHM_NR_ENTRIES] =
{
    "Bubble Sort",
    "Insertion Sort",
    "Quicksort",
    "Radix Sort",
    "Selection Sort",
    "Shaker Sort",
    "Shell Sort",
};

static void* sort_thread_main(void* data);

int main(int argc, char* argv[])
{

#ifdef UNIT_TEST
    return RunUnitTests();
#endif

    //----------------- Command argument handling --------------------------
    if (argc == 1)
    {
        printf("Please specify path to an image file!\n");
        return 1;
    }

    char*  path = argv[1];
    bool   success;
    Image* image = new Image();
    assert(image);

    success = image->loadFromFile(path);
    if (!success)
    {
        printf("Error: The provided image '%s' cannot be loaded!\n", path);
        return 1;
    }

    //----------------- Configurable settings --------------------------
    printf("Welcome to Pixel sorter!\n");
    printf("\n");
    printf("Image file: %s\n", path);
    printf("\n");
    printf("Available algorithms:\n");

    int i;
    int algorithm = (int)ALGORITHM_DEFAULT;
    for(i=0; i<ALGORITHM_NR_ENTRIES; i++)
    {
        printf("%d. %s\n",i, algorithmName[i]);
    }
    printf("\n");
    printf("Select algorithm (default %d): ", algorithm);
    char input[20];
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &algorithm);
    if (algorithm < 0 || algorithm >= ALGORITHM_NR_ENTRIES)
    {
        printf("Incorrect input, using default.\n");
        algorithm = ALGORITHM_DEFAULT;
    }

    printf("\n");
    printf("Using %s as sorting algorithm.\n", algorithmName[algorithm]);

    printf("\n");
    printf("Scatter settings:\n");
    printf(" - Scatter pixels (2-2000)\n");
    printf(" - Random pixels  (0)\n");
    printf("Select scatter (default %d): ", 0);
    int scatter;
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &scatter);
    if (scatter < 2 || scatter > 2000)
    {
        scatter = 0;
    }
    printf("\n");

    //----------------- Resizing image to manageable size --------------------------
    #define WIDTH 1280
    float width = (float)image->getWidth();
    float height = (float)image->getHeight();
    float aspectRatio = width / height;
    int newHeight = (int)(((float)WIDTH) / aspectRatio);
    printf("Resizing image to %dx%d\n", WIDTH, newHeight);
    image->rescale(WIDTH, newHeight);

    //----------------- Scramble pixels and start sorting in separate thread --------------------------
    PixelSorter* pixelSorter = new PixelSorter(image);
    assert(pixelSorter);
    pixelSorter->setAlgorithm((algorithm_t)algorithm);

    printf("Scambling pixels\n");
    pixelSorter->randomize(scatter);

    Thread* thread = new Thread(sort_thread_main, pixelSorter);
    assert(thread);

    //----------------- Show results in GTK window --------------------------
    GtkImageWindow* imageWindow = new GtkImageWindow(image);
    assert(imageWindow);
    imageWindow->show();

    //----------------- User has closed window, exit program --------------------------
    printf("Exiting\n");
    thread->join();

    delete imageWindow;
    delete thread;
    delete pixelSorter;
    delete image;
    
    return 0;
}

//-----------------------------------------------------------------------------------------------------------

static void* sort_thread_main(void* data)
{
    PixelSorter* pixelSorter = (PixelSorter*)data;

    if (!pixelSorter) return NULL;

    printf("Sorting pixels\n");
    pixelSorter->sort();
    printf("Done!\n");

    return NULL;
}

//-----------------------------------------------------------------------------------------------------------

void pixelSwapCb() 
{
    if (exitProgram)
    {
        int ret = 1;
        pthread_exit(&ret);
    }
}

//-----------------------------------------------------------------------------------------------------------
