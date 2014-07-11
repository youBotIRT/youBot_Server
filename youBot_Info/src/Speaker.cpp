/****************************************************************
 *
 * Copyright (c) 2014
 * All rights reserved.
 *
 * RWTH Aachen
 * Institut für Regelungstechnik
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Author:
 * Fabian Buse
 * Supervised by:
 * Andre Duffe
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * This sofware is published under a dual-license: GNU Lesser General Public
 * License LGPL 2.1 and BSD license. The dual-license implies that users of this
 * code may choose which terms they prefer.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the RWTH Aachen nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License LGPL as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version or the BSD license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License LGPL and the BSD license for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License LGPL and BSD license along with this program.
 *
 ****************************************************************/

#include "Speaker.hpp"

Speaker::Speaker()
{


}

Speaker::~Speaker()
{
	mtx_.lock();
	int fd = open("/dev/console", O_WRONLY);
	ioctl(fd, KIOCSOUND, 0);
	close(fd);
	mtx_.unlock();
}

void Speaker::Beep(int freq, int duration_msec)
{
	mtx_.lock();
	int fd = open("/dev/console", O_WRONLY);
	ioctl(fd, KIOCSOUND, 1193180 / freq);
	usleep(duration_msec * 1000);
	ioctl(fd, KIOCSOUND, 0);
	close(fd);
	mtx_.unlock();
}

void Speaker::Delay(int duration_msec)
{
	usleep(duration_msec * 1000);
}

void Speaker::beep_1()
{
	boost::thread t(&Speaker::beep_1_t, this);
}

void Speaker::beep_2()
{
	boost::thread t(&Speaker::beep_2_t, this);
}

void Speaker::beep_3()
{
	boost::thread t(&Speaker::beep_3_t, this);
}

void Speaker::beep_1_t()
{
	Beep(600, 200);
	Beep(900, 400);
}

void Speaker::beep_2_t()
{
	Beep(600, 200);
	Beep(400, 400);
}

void Speaker::beep_3_t()
{
	Beep(600, 200);
	Delay(50);
	Beep(600, 200);
}

//Songs

