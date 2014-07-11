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

#include "Watchdog.hpp"

Watchdog::Watchdog() {
	fp_callback_func_ = NULL;

	max_time_ms_ = 1000;
	current_time_ms_ = 0;
	ok_ = true;
	reset_ = false;
	tick_time_ms_ = 10;
}

Watchdog::~Watchdog() {

}


bool Watchdog::ok() {
	return ok_;
}

void Watchdog::reset() {
	reset_ = true;
	ok_ = true;
}

void Watchdog::start(int max_time_ms,void (*callback_error)()){

	fp_callback_func_ = callback_error;
	start(max_time_ms);
}


void Watchdog::start(int max_time_ms) {
	if(max_time_ms>500)
	{
		max_time_ms_ = max_time_ms;
		tick_time_ms_ = max_time_ms /10;
	}else
	{
		max_time_ms_ = 500;
		tick_time_ms_ = 50;
	}

}

void Watchdog::loop() {
	while(true)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(tick_time_ms_));

		if(reset_)
		{
			current_time_ms_ =0;
			reset_ = false;
		}

		if(current_time_ms_>max_time_ms_)
		{
			if( fp_callback_func_ )
				fp_callback_func_();

			ok_ = false;
		}else{
			current_time_ms_ += tick_time_ms_;
		}
	}
}
