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

#ifndef _GTK_IMAGE_WINDOW_H_
#define _GTK_IMAGE_WINDOW_H_

#include "image.hpp"

#include <gtk/gtk.h>

typedef struct
{
    GtkWidget* gtkImage;
    Image*     image;
    gboolean   stopTimer;
} screen_refresh_ctx_t;

//-----------------------------------------------------------------------------------------------------------

class GtkImageWindow
{
public:
    GtkImageWindow(Image* image);
    ~GtkImageWindow();

    void show(void);

private:
    // Callback functions
    static void     activate(GtkApplication* app, gpointer user_data);
    static gboolean refresh_image(gpointer user_data);
    static gboolean window_delete(GtkWidget *widget, GdkEvent* event, gpointer data);
    
    screen_refresh_ctx_t* screenRefreshCtx;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _GTK_IMAGE_WINDOW_H_ */