
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

#include "Message.hpp"

Message::Message() {
	error_ = 0;
	msgid_ = 0;
	priority_ = 0;
	type_ = 0;

	type_ = 0;
	data_type_ = "NONE";

}

Message::Message(zmq::message_t *msg_t) {
	unpack(msg_t);
}

int Message::get_Type() {
	return type_;
}

void Message::set_Type(int type) {
	type_ = type;
}

int Message::get_MsgID() {
	return msgid_;
}

void Message::set_MsgID(int msgid) {
	msgid_ = msgid;
}

int Message::get_Error() {
	return error_;
}

void Message::set_Error(int error) {
	error_ = error;
}

int Message::get_Priority() {
	return priority_;
}

void Message::set_Priority(int priority) {
	priority_ = priority;
}

std::string Message::get_Sender_ID() {
	return sender_id_;
}

void Message::set_Sender_ID(std::string sender_id) {
	sender_id_ = sender_id;
}

std::string Message::get_Data_Type() {
	return data_type_;
}

void Message::set_Data_Type(std::string data_type) {
	data_type_ = data_type;
}

void Message::set_Data(std::string data) {
	data_ = data;
}

std::string Message::get_Data() {
	return data_;
}

void Message::pack(zmq::message_t *msg_t) {
	pack_Data();
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> pk(&sbuf);

	pk.pack(type_);
	pk.pack(msgid_);
	pk.pack(error_);
	pk.pack(priority_);
	pk.pack(sender_id_);
	pk.pack(data_type_);
	pk.pack(data_);

	msg_t->rebuild(sbuf.size());
	memcpy(msg_t->data(), sbuf.data(), sbuf.size());

}

bool Message::unpack(zmq::message_t *msg_t) {

	msgpack::unpacker pac;
	pac.reserve_buffer(msg_t->size());
	memcpy(pac.buffer(), reinterpret_cast<char*>(msg_t->data()), msg_t->size());
	pac.buffer_consumed(msg_t->size());

	msgpack::unpacked result;
	pac.next(&result);
	int type_msg=0;
	result.get().convert(&type_msg);
	pac.next(&result);
	result.get().convert(&msgid_);
	pac.next(&result);
	result.get().convert(&error_);
	pac.next(&result);
	result.get().convert(&priority_);
	pac.next(&result);
	result.get().convert(&sender_id_);
	pac.next(&result);
	result.get().convert(&data_type_);
	pac.next(&result);
	result.get().convert(&data_);

	if(type_ == type_msg)
	{
		unpack_Data();
		return true;
	}
	else
		return false;

}

//void Message::clear() {
//	error_ = 0;
//	msgid_ = 0;
//	priority_ = 0;
//	type_ = 0;
//}

int Message::get_Message_Type(zmq::message_t *msg_t) {
	msgpack::unpacker pac;
	pac.reserve_buffer(msg_t->size());
	memcpy(pac.buffer(), reinterpret_cast<char*>(msg_t->data()), msg_t->size());
	pac.buffer_consumed(msg_t->size());

	msgpack::unpacked result;
	pac.next(&result);

	result.get().convert(&type_);
	pac.next(&result);
	return type_;
}

Message::~Message() {
}

void Message::pack_Data(){}
void Message::unpack_Data(){}

void Message::clear(){}
