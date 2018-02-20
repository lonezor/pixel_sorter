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
#include <getopt.h>

#include "image.h"
#include "pixel_sorter.h"
#include "gtk_image_window.h"
#include "thread.h"

#ifdef UNIT_TEST
#include "unit_test.h"
#endif

#define MAX_STRING (255)

#define DEFAULT_INPUT_FILE        "photo.jpg"
#define DEFAULT_DIR               "/tmp"
#define DEFAULT_IMAGE_WIDTH       (1920)
#define DEFAULT_IMAGE_HEIGHT      (1080)
#define DEFAULT_FPS               (60)
#define DEFAULT_FIRST_DELAY       (1)
#define DEFAULT_SCRAMBLE_DURATION (5)
#define DEFAULT_SECOND_DELAY      (2)
#define DEFAULT_SORTING_DURATION  (10)
#define DEFAULT_THIRD_DELAY       (3)

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

enum
{
    OPTION_UNKNOWN,
    OPTION_PNG_SEQUENCE,
    OPTION_HELP_SCREEN,
    OPTION_DIR,
    OPTION_WIDTH,
    OPTION_HEIGHT,
    OPTION_FPS,
    OPTION_FIRST_DELAY,
    OPTION_SCRAMBLE_DURATION,
    OPTION_SECOND_DELAY,
    OPTION_SORTING_DURATION,
    OPTION_THIRD_DELAY,
};

static char optDir[MAX_STRING];
static char optInputFile[MAX_STRING];
static int  optPngSequence      = 0;
static int  optHelpScreen       = 0;
static int  optImageWidth       = DEFAULT_IMAGE_WIDTH;
static int  optImageHeight      = DEFAULT_IMAGE_HEIGHT;
static int  optFps              = DEFAULT_FPS;
static int  optFirstDelay       = DEFAULT_FIRST_DELAY;
static int  optScrambleDuration = DEFAULT_SCRAMBLE_DURATION;
static int  optSecondDelay      = DEFAULT_SECOND_DELAY;
static int  optSortingDuration  = DEFAULT_SORTING_DURATION;
static int  optThirdDelay       = DEFAULT_THIRD_DELAY;

static struct option long_options[] =
{
    { "png-sequence",      no_argument,       &optPngSequence, OPTION_PNG_SEQUENCE      },
    { "help",              no_argument,       &optHelpScreen,  OPTION_HELP_SCREEN       },
    { "dir",               required_argument, 0,               OPTION_DIR               },
    { "width",             required_argument, 0,               OPTION_WIDTH             },
    { "height",            required_argument, 0,               OPTION_HEIGHT            },
    { "fps",               required_argument, 0,               OPTION_FPS               },
    { "first-delay",       required_argument, 0,               OPTION_FIRST_DELAY       },
    { "scramble-duration", required_argument, 0,               OPTION_SCRAMBLE_DURATION },
    { "second-delay",      required_argument, 0,               OPTION_SECOND_DELAY      },
    { "sorting-duration",  required_argument, 0,               OPTION_SORTING_DURATION  },
    { "third-delay",       required_argument, 0,               OPTION_THIRD_DELAY       },
    { 0,                   0,                 0,               0                        },
};

void printHelp()
{
    printf("usage: [OPTIONS] <image>\n");
    printf("\n");
    printf("Overall Options:\n");
    printf(" --png-sequence               Write image sequence instead of displaying live window. To adjust output to certain number of frames, a two-pass approach is used.\n");
    printf(" --dir=PATH                   Output directory (default '/tmp'\n");
    printf("\n");
    printf("Image Options:\n");
    printf(" --width=INTEGER              Image width of output file (default 1920)\n");
    printf(" --height=INTEGER             Image height of output file (default 1080)\n");
    printf("\n");
    printf("Time Options:\n");
    printf(" --fps=INTEGER                Frame rate for video generation (default 60)\n");
    printf(" --first-delay=INTEGER        Initial delay before scrambling (showing original picture). Unit: seconds (default 1)\n");
    printf(" --scramble-duration=INTEGER  Time for disorganising all pixels. Unit: seconds (default 5)\n");
    printf(" --second-delay=INTEGER       Delay before sorting (showing scrambled image). Unit: seconds (default 2)\n");
    printf(" --sorting-duration=INTEGER   Time for sorting all pixels. Unit: seconds (default 10)\n");
    printf(" --third-delay=INTEGER        Delay before finishing (showing sorted picture). Unit: seconds (default 3)\n");
    printf("\n");
    printf("Misc Options\n");
    printf(" --help                       This help screen.\n");
}

void parseArguments(int argc, char* argv[])
{
    int option_index = 0;
    int c;

    snprintf(optInputFile, sizeof(optInputFile), "%s", DEFAULT_INPUT_FILE);
    snprintf(optDir,       sizeof(optDir),       "%s", DEFAULT_DIR);

    while (true)
    {
        c = getopt_long (argc, argv, "", long_options, &option_index);
        if (c == -1) break;

        switch (c)
        {
            case OPTION_DIR:
                snprintf(optDir, sizeof(optDir), "%s", optarg);
                break;
            case OPTION_WIDTH:
                optImageWidth = atoi(optarg);
                if (optImageWidth < 80 || optImageWidth > 16384)
                {
                    printf("Invalid image width %d. Defaulting to %d\n", optImageWidth, DEFAULT_IMAGE_WIDTH);
                    optImageWidth = DEFAULT_IMAGE_WIDTH;
                }
                break;
            case OPTION_HEIGHT:
                optImageHeight = atoi(optarg);
                if (optImageHeight < 45 || optImageHeight > 9216)
                {
                    printf("Invalid image height %d. Defaulting to %d\n", optImageHeight, DEFAULT_IMAGE_HEIGHT);
                    optImageHeight = DEFAULT_IMAGE_WIDTH;
                }
                break;
            case OPTION_FPS:
                optFps = atoi(optarg);
                if (optFps < 10 || optFps > 120)
                {
                    printf("Invalid FPS %d. Defaulting to %d\n", optFps, DEFAULT_FPS);
                    optFps = DEFAULT_FPS;
                }
                break;
            case OPTION_FIRST_DELAY:
                optFirstDelay = atoi(optarg);
                if (optFirstDelay < 0 || optFirstDelay > 30)
                {
                    printf("Invalid first delay %d. Defaulting to %d\n", optFirstDelay, DEFAULT_FIRST_DELAY);
                    optFirstDelay = DEFAULT_FIRST_DELAY;
                }
                break;
            case OPTION_SCRAMBLE_DURATION:
                optScrambleDuration = atoi(optarg);
                if (optScrambleDuration < 0 || optScrambleDuration > 3600)
                {
                    printf("Invalid scramble duration %d. Defaulting to %d\n", optScrambleDuration, DEFAULT_SCRAMBLE_DURATION);
                    optScrambleDuration = DEFAULT_SCRAMBLE_DURATION;
                }
                break;
            case OPTION_SECOND_DELAY:
                optSecondDelay = atoi(optarg);
                if (optSecondDelay < 0 || optSecondDelay > 30)
                {
                    printf("Invalid second delay %d. Defaulting to %d\n", optSecondDelay, DEFAULT_SECOND_DELAY);
                    optSecondDelay = DEFAULT_SECOND_DELAY;
                }
                break;
            case OPTION_SORTING_DURATION:
                optSortingDuration = atoi(optarg);
                if (optSortingDuration < 0 || optSortingDuration > 3600)
                {
                    printf("Invalid sorting duration %d. Defaulting to %d\n", optSortingDuration, DEFAULT_SORTING_DURATION);
                    optSortingDuration = DEFAULT_SCRAMBLE_DURATION;
                }
                break;
            case OPTION_THIRD_DELAY:
                optThirdDelay = atoi(optarg);
                if (optThirdDelay < 0 || optThirdDelay > 30)
                {
                    printf("Invalid third delay %d. Defaulting to %d\n", optThirdDelay, DEFAULT_THIRD_DELAY);
                    optThirdDelay = DEFAULT_THIRD_DELAY;
                }
                break;
            default:
                break;
        }
    }

    if (optind < argc)
    {
        snprintf(optInputFile, sizeof(optInputFile), "%s", argv[optind]);
    }
}

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

    parseArguments(argc, argv);

    if (optHelpScreen)
    {
        printHelp();
        return 0;
    }

    char*  path = argv[argc-1];
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

    enum 
    {
        METHOD_WHITE_NOISE,
        METHOD_LIGHT_INTENSITY,
        METHOD_HUE,
    };

    printf("\n");
    printf("Method for disorganising pixels:\n");
    printf(" 0. Completely Random\n");
    printf(" 1. Light Intensity\n");
    printf(" 2. Hue\n");
    printf("Select method (default 2): ");
    int method = -1;
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &method);
    if (method < 0 || method > 2)
    {
        method = METHOD_HUE;
    }
    printf("\n");
    printf("Using %d as scrambling method.\n", method);

    if (optPngSequence)
    {

        printf("optDir: %s\n", optDir);
        printf("optInputFile: %s\n", optInputFile);
        printf("optImageWidth: %d\n", optImageWidth);
        printf("optImageHeight: %d\n", optImageHeight);
        printf("optFps: %d\n", optFps);
        printf("optFirstDelay: %d\n", optFirstDelay);
        printf("optScrambleDuration: %d\n", optScrambleDuration);
        printf("optSecondDelay: %d\n", optSecondDelay);
        printf("optSortingDuration: %d\n", optSortingDuration);
        printf("optThirdDelay: %d\n", optThirdDelay);

        return 0; // todo
    }

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
    switch(method)
    {
        case METHOD_WHITE_NOISE:
            pixelSorter->whiteNoise();
            break;
        case METHOD_LIGHT_INTENSITY:
            pixelSorter->sortByPosition(PIXEL_POSITION_LIGHT_INTENSITY);
            pixelSorter->scatter(5000);
            break;
        case METHOD_HUE:
            pixelSorter->sortByPosition(PIXEL_POSITION_HUE);
            pixelSorter->scatter(5000);
        default:
            break;
    }

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
