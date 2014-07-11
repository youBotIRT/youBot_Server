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

#include "youBotDisplay.hpp"

youBot_Display::youBot_Display() {

}

youBot_Display::~youBot_Display() {

	clear(1);
	clear(2);
}

void youBot_Display::start() {
	boost::thread tmp_thread(&youBot_Display::update_Display, this);
}

bool youBot_Display::write(char Input[17], int line) {

	if ((line > 0) & (line < 3))
		line++;
	else
		return false;
	Input[16]=0;

	FILE* display = fopen("/dev/ttyACM0", "a+");
	if(display){

		fputc(line, display);
		if(fprintf(display, "%s", Input)>=0)
			fputc(13, display);
		fclose(display);

	} else {
		std::cout << "Line " << line << " -> " << Input << std::endl;
	}
	return true;
}

void youBot_Display::set_Display_Text(std::string text, int line) {
	mtx_.lock();
	if (text.compare(text_[line - 1]) != 0) {
		text_[line - 1] = text;
		write_[line - 1] = true;
		counter_[line - 1] = 0;
	}
	mtx_.unlock();

}

void youBot_Display::clear_Display_Text(int line) {
	clear(line);
}

void youBot_Display::clear(int line) {
	char string[17];
	for (int i = 0; i < 17; i++)
		string[i] = ' ';

	write(string, line);
}

void youBot_Display::update_Display() {

	counter_[0] = 0;
	counter_[1] = 0;

	while (true) {

		for (int i = 0; i < 2; i++) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(2000));

			mtx_.lock();

			if (write_[i]) {
				char input[17];
				memset(input, ' ', 17);
				if (text_[i].length() > 16) {
					memcpy(input, text_[i].c_str(), 16);
				} else {
					memcpy(input, text_[i].c_str(), text_[i].length());
				}
				write(input, i + 1);

			}

			mtx_.unlock();

		}


	}
}

