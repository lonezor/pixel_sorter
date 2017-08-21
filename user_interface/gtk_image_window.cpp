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

#include "gtk_image_window.h"
#include "image.h"
#include "thread.h"

#include <stdio.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#define PROGRAM_NAME "Visualization of sorting algorithms"

//-----------------------------------------------------------------------------------------------------------

GtkImageWindow::GtkImageWindow(Image* image)
{
    this->screenRefreshCtx = (screen_refresh_ctx_t*)calloc(1,sizeof(screen_refresh_ctx_t));
    assert(this->screenRefreshCtx);

    this->screenRefreshCtx->image = image;
    this->screenRefreshCtx->stopTimer = FALSE;
}

//-----------------------------------------------------------------------------------------------------------

GtkImageWindow::~GtkImageWindow()
{
    free(this->screenRefreshCtx);
}

//-----------------------------------------------------------------------------------------------------------

void GtkImageWindow::show()
{
    GtkApplication* app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
    assert(app);

    g_signal_connect (app, "activate", G_CALLBACK (activate), (gpointer)this->screenRefreshCtx); // TODO: send data from show() to both activate and refresh so we can free it later
    char* argv[] = {(char*)"no_arguments"};
    g_application_run (G_APPLICATION (app), 1, argv);
    g_object_unref (app);
}

//-----------------------------------------------------------------------------------------------------------

void GtkImageWindow::activate (GtkApplication* app, gpointer user_data) // static: callback function
{
    screen_refresh_ctx_t* ctx = ( screen_refresh_ctx_t*)user_data;
    assert(ctx);

    Image* image = ctx->image;
    assert(image);

    int width  = image->getWidth();
    int height = image->getHeight();

    GtkWidget* gtkImage = image->newGtkImage();
    assert(gtkImage);

    GtkWidget* window = gtk_application_window_new (app);
    assert(window);

    gtk_container_add (GTK_CONTAINER (window), gtkImage);
    gtk_window_set_title (GTK_WINDOW (window), PROGRAM_NAME);
    gtk_window_set_default_size (GTK_WINDOW (window), width, height);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_widget_show_all (window);

    ctx->gtkImage = gtkImage;
    g_timeout_add (1, refresh_image, ctx);
    g_signal_connect (window, "delete-event", G_CALLBACK (window_delete), ctx);
}

//-----------------------------------------------------------------------------------------------------------

gboolean GtkImageWindow::refresh_image(gpointer user_data)
{
    screen_refresh_ctx_t* ctx = ( screen_refresh_ctx_t*)user_data;
    assert(ctx);

    if (ctx->stopTimer)
    {
        return FALSE;
    }

    // After updating pixbuf, it is not enough to queue draw event for the image. The pixmap needs to be replaced.
    GdkPixbuf* buf = ctx->image->newGdkPixBuf();
    assert(buf);

    gtk_image_set_from_pixbuf ((GtkImage*)ctx->gtkImage, (GdkPixbuf*)buf);
    g_object_unref(buf);

    return TRUE;
}

//-----------------------------------------------------------------------------------------------------------

gboolean GtkImageWindow::window_delete(GtkWidget* widget, GdkEvent* event, gpointer data)
{
    screen_refresh_ctx_t* ctx = (screen_refresh_ctx_t*)data;
    assert(ctx);

    ctx->stopTimer = TRUE;
    exitProgram    = TRUE;

    return FALSE;
}

//-----------------------------------------------------------------------------------------------------------
