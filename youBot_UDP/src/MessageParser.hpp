
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

#ifndef MESSAGEPARSER_HPP_
#define MESSAGEPARSER_HPP_

#define HEAD 0x0a
#define JOINT 0x0b
#define BASE 0x0c
#define GRIPPER 0x0d
#define COMPLEX 0x0e


#include "Client.hpp"

class Message_Parser {
public:
	Message_Parser();
	virtual ~Message_Parser();

	void to_Byte_Array(youBot_Reply &msg, char buffer[],int &length);

	void to_Request(char buffer[],const int &length, youBot_Request &msg);

private:

	char *buffer_;
	int length_;
	int max_length_;

	void init_Buffer(char buffer[]);
	void init_Buffer(char buffer[],int length);
	void write_to_Buffer(char c);
	void write_to_Buffer(int i);
	void write_to_Buffer(unsigned int i);
	void write_to_Buffer(float f);
	void write_to_Buffer(unsigned long long l);

	void read_form_Buffer(char &c);
	void read_form_Buffer(int &i);
//	void read_form_Buffer(unsigned int &i);
	void read_form_Buffer(float &f);
//	void read_form_Buffer(unsigned long long &l);

	float byteToFloat(char b[], int offset);
	int byteToInt(char b[], int offset);

	void increase_Counter(int inc);

	void floatToByte(char b[], float n,int offset);
	void intToByte(char b[], int n,int offset);
	void uintToByte(char b[], unsigned int n,int offset);
	void longlongToByte(char b[], unsigned long long n,int offset);
};

#endif /* MESSAGEPARSER_HPP_ */
