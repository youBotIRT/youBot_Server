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

#include "Joystick.hpp"

Joystick::Joystick()
{
    this->ready = false;
    fd =0;
}

Joystick::~Joystick()
{
    //dtor
}

bool Joystick::reload(const char* device)
{
	close(this->fd);
    this->fd = open(device, O_RDONLY | O_NONBLOCK);
    if(this->fd > 0)
    {
        this->ready = true;
    }
    else
    {
        this->ready = false;
    }

    return this->ready;
}

bool Joystick::load(const char* device)
{
    this->fd = open(device, O_RDONLY | O_NONBLOCK);
    if(this->fd > 0)
    {
        this->ready = true;
    }
    else
    {
        this->ready = false;
    }

    return this->ready;
}

bool Joystick::is_ready()
{
    return this->ready;
}

int Joystick::get_num_buttons()
{
    char number_of_buttons;
    ioctl(this->fd, JSIOCGBUTTONS, &number_of_buttons);
    return (int)number_of_buttons;
}

int Joystick::get_num_axes()
{
    char number_of_axes;
    ioctl(this->fd, JSIOCGAXES, &number_of_axes);
    return (int)number_of_axes;
}

char* Joystick::get_name()
{
    char* name = new char[128];
    ioctl(this->fd, JSIOCGNAME(128), name);
    return name;
}

bool Joystick::get_event(js_event* event)
{
    int bytes = read(this->fd, event, sizeof(*event));

    if(bytes != -1 && bytes == sizeof(*event))
    {
        return true;
    }

    return false;
}
