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

#ifndef _THREAD_H_
#define _THREAD_H_

#include <stdio.h>
#include <pthread.h>

typedef void* thread_prototype(void* data);
extern bool exitProgram;

//-----------------------------------------------------------------------------------------------------------

class Thread
{
public:
    Thread(thread_prototype* fn, void* data);

    int join();

private:
    pthread_t thread;
};

//-----------------------------------------------------------------------------------------------------------

#endif /* _THREAD_H_ */
