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

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include "msgpack.hpp"
#include "zmq.hpp"
#include <string>
#include <iostream>
#include <unistd.h>


class Message{
protected:
	int type_;
	int msgid_;
	int error_;
	int priority_;
	std::string sender_id_;
	std::string data_type_;
	std::string data_;

	virtual void pack_Data();
	virtual void unpack_Data();

public:
	Message();
	virtual ~Message();
	Message(zmq::message_t *msg_t);
	virtual void clear();
	int get_Type();
	void set_Type(int type);
	int get_MsgID();
	void set_MsgID(int msgid);
	int get_Error();
	void set_Error(int error);
	int get_Priority();
	void set_Priority(int priority);
	std::string get_Sender_ID();
	void set_Sender_ID(std::string sender_id);
	std::string get_Data_Type();
	void set_Data_Type(std::string data_type);
	void set_Data(std::string data);
	std::string get_Data();
	void pack(zmq::message_t *msg_t);
	bool unpack(zmq::message_t *msg_t);


	int get_Message_Type(zmq::message_t *msg_t);
};

#endif /* MESSAGE_HPP_ */
