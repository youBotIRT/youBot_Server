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

#include "MessageParser.hpp"

Message_Parser::Message_Parser() {
	length_ = 0;
	max_length_ = 0;
	buffer_ = "";

}

Message_Parser::~Message_Parser() {

}

void Message_Parser::to_Byte_Array(youBot_Reply& msg, char buffer[], int& length) {

	init_Buffer(buffer);
	write_to_Buffer((char) HEAD);
	write_to_Buffer(msg.get_State());
	write_to_Buffer(msg.get_Timestamp());
	int i = 0;

	for (i = 0; i < msg.get_Number_Of_Joint_Msg(); i++) {
		Joint_Information ji = msg.get_Joint_Info(i);
		write_to_Buffer((char) JOINT);
		write_to_Buffer(ji.joint);
		write_to_Buffer(ji.position);
		write_to_Buffer(ji.velocity);
		write_to_Buffer(ji.current);
		write_to_Buffer(ji.status);
	}
	for (i = 0; i < msg.get_Number_Of_Base_Msg(); i++) {
		Base_Information bi = msg.get_Base_Info(i);
		write_to_Buffer((char) BASE);
		write_to_Buffer(bi.num);
		write_to_Buffer(bi.position[0]);
		write_to_Buffer(bi.position[1]);
		write_to_Buffer(bi.position[2]);
		write_to_Buffer(bi.velocity[0]);
		write_to_Buffer(bi.velocity[1]);
		write_to_Buffer(bi.velocity[2]);
		write_to_Buffer(bi.status[0]);
		write_to_Buffer(bi.status[1]);
		write_to_Buffer(bi.status[2]);
		write_to_Buffer(bi.status[3]);
	}
	for (i = 0; i < msg.get_Number_Of_Gripper_Msg(); i++) {
		Gripper_Information gi = msg.get_Gripper_Info(i);
		write_to_Buffer((char) GRIPPER);
		write_to_Buffer(gi.position);
	}
	length = length_;
}

void Message_Parser::to_Request(char buffer[], const int& length, youBot_Request& msg) {

	msg.set_State_Req(0);

	char c;

	init_Buffer(buffer, length);

	while (length_ < max_length_) {
		read_form_Buffer(c);
		switch (c) {
		case HEAD: {
			std::cout << "Head" << std::endl;
			int state_Req;
			read_form_Buffer(state_Req);
			msg.add_State_Req(state_Req);
			break;
		}
		case JOINT: {
			std::cout << "Joint" << std::endl;
			Joint_Message jm;
			read_form_Buffer(jm.joint);
			read_form_Buffer(jm.type);
			read_form_Buffer(jm.value);
			msg.add_Joint_Msg(jm);
			msg.add_State_Req(ARM_CONTROL);
			break;
		}
		case BASE: {
			std::cout << "base" << std::endl;
			Base_Message bm;
			read_form_Buffer(bm.num);
			read_form_Buffer(bm.type);
			bm.value.resize(3, 0);
			read_form_Buffer(bm.value[0]);
			read_form_Buffer(bm.value[1]);
			read_form_Buffer(bm.value[2]);
			msg.add_Base_Msg(bm);
			msg.add_State_Req(BASE_CONTROL);
			break;
		}
		case GRIPPER: {
			std::cout << "Gripper" << std::endl;
			Gripper_Message gm;
			read_form_Buffer(gm.num);
			read_form_Buffer(gm.value);
			gm.type = 0;
			msg.add_Gripper_Msg(gm);
			msg.add_State_Req(ARM_CONTROL);
			length_++;
			break;
		}
		case COMPLEX: {
			int comp_length, comp_type;
			std::vector<float> data;
			read_form_Buffer(comp_type);
			read_form_Buffer(comp_length);
			data.resize(comp_length, 0);
			for (int i = 0; i < comp_length; i++) {
				read_form_Buffer(data[i]);
			}
			msg.add_Complex_Msg(comp_type, data);
			msg.add_State_Req(ARM_CONTROL);
			break;
		}
		}
	}

}

float Message_Parser::byteToFloat(char b[], int offset) {
	float output;

	*((char*) (&output) + 0) = b[0 + offset];
	*((char*) (&output) + 1) = b[1 + offset];
	*((char*) (&output) + 2) = b[2 + offset];
	*((char*) (&output) + 3) = b[3 + offset];

	return output;
}

int Message_Parser::byteToInt(char b[], int offset) {
	int output;

	*((char*) (&output) + 0) = b[0 + offset];
	*((char*) (&output) + 1) = b[1 + offset];
	*((char*) (&output) + 2) = b[2 + offset];
	*((char*) (&output) + 3) = b[3 + offset];

	return output;
}

void Message_Parser::floatToByte(char b[], float n, int offset) {
	unsigned int val = *((unsigned int*) &n);

	b[0 + offset] = (char) (val & 0xFF);
	b[1 + offset] = (char) ((val >> 8) & 0xFF);
	b[2 + offset] = (char) ((val >> 16) & 0xFF);
	b[3 + offset] = (char) ((val >> 24) & 0xFF);
}

void Message_Parser::intToByte(char b[], int n, int offset) {
	unsigned int val = *((unsigned int*) &n);

	b[0 + offset] = (char) (val & 0xFF);
	b[1 + offset] = (char) ((val >> 8) & 0xFF);
	b[2 + offset] = (char) ((val >> 16) & 0xFF);
	b[3 + offset] = (char) ((val >> 24) & 0xFF);
}

void Message_Parser::uintToByte(char b[], unsigned int n, int offset) {

	b[0 + offset] = (char) (n & 0xFF);
	b[1 + offset] = (char) ((n >> 8) & 0xFF);
	b[2 + offset] = (char) ((n >> 16) & 0xFF);
	b[3 + offset] = (char) ((n >> 24) & 0xFF);
}

void Message_Parser::read_form_Buffer(char& c) {
	c = buffer_[length_];
	increase_Counter(1);
}

void Message_Parser::read_form_Buffer(int& i) {
	i = byteToInt(buffer_, length_);
	increase_Counter(4);
}

void Message_Parser::read_form_Buffer(float& f) {
	f = byteToFloat(buffer_, length_);
	increase_Counter(4);
}

void Message_Parser::init_Buffer(char buffer[], int length) {
	init_Buffer(buffer);
	max_length_ = length;
}

void Message_Parser::longlongToByte(char b[], unsigned long long n, int offset) {

	b[0 + offset] = (char) (n & 0xFF);
	b[1 + offset] = (char) ((n >> 8) & 0xFF);
	b[2 + offset] = (char) ((n >> 16) & 0xFF);
	b[3 + offset] = (char) ((n >> 24) & 0xFF);

	b[4 + offset] = (char) ((n >> 32) & 0xFF);
	b[5 + offset] = (char) ((n >> 40) & 0xFF);
	b[6 + offset] = (char) ((n >> 48) & 0xFF);
	b[7 + offset] = (char) ((n >> 56) & 0xFF);
}

void Message_Parser::increase_Counter(int inc) {
	length_ += inc;
	if (length_ > max_length_) {
		std::exception e;
		throw std::length_error("Invalid Message format");
	}
}

void Message_Parser::init_Buffer(char buffer[]) {
	buffer_ = buffer;
	length_ = 0;
}
void Message_Parser::write_to_Buffer(char c) {
	buffer_[length_] = c;
	length_++;
}
void Message_Parser::write_to_Buffer(int i) {
	intToByte(buffer_, i, length_);
	length_ += 4;
}

void Message_Parser::write_to_Buffer(unsigned int i) {
	uintToByte(buffer_, i, length_);
	length_ += 4;
}

void Message_Parser::write_to_Buffer(float f) {
	floatToByte(buffer_, f, length_);
	length_ += 4;
}

void Message_Parser::write_to_Buffer(unsigned long long l) {
	longlongToByte(buffer_, l, length_);
	length_ += 8;
}
