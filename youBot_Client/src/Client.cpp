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

#include "Client.hpp"

Client::Client() {
	init("NONE", 9999, NULL, NULL);
}

Client::Client(std::string name, int priority, Message *send, Message *receive) {

	init(name, priority, send, receive);

}

Client::~Client() {
	if (p_socket_) {
		delete p_socket_;
	}
}

void Client::connect(std::string adr, zmq::context_t *context) {
	adr_ = adr;
	p_context_ = context;
	p_socket_ = new zmq::socket_t(*p_context_, ZMQ_REQ);
	int linger = 0;
	p_socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
	p_socket_->connect(adr_.c_str());

	std::cout << "----------------------------------" << std::endl;
	std::cout << "Creating Client " << sender_id_ << std::endl;
	std::cout << "Trying to connect to Server at " << adr_ << std::endl;
	std::cout << "Timeout: " << timeout_ << " Retries: " << retries_ << std::endl;
	std::cout << "Request Type: " << send_type_str_ << " | ID: " << send_type_ << std::endl;
	std::cout << "Reply Type: " << receive_type_str_ << " | ID: " << receive_type_ << std::endl;
	std::cout << "----------------------------------" << std::endl;
}

int Client::reconnect() {
	//TODO MAKE IT BETTER
	delete p_socket_;
	p_socket_ = new zmq::socket_t(*p_context_, ZMQ_REQ);
//	p_socket_->close();
	p_socket_->connect(adr_.c_str());
	int linger = 0;
	p_socket_->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
	reconnected_ = true;
	return 0;
}

int Client::send(Message *msg) {

	if (msg->get_Type() == send_type_) {
		msg_id_++;
		msg->set_MsgID(msg_id_);
		msg->set_Sender_ID(sender_id_);
		msg->set_Priority(priority_);
		zmq::message_t msg_t;
		msg->pack(&msg_t);
		p_socket_->send(msg_t);
		return 0;
	} else
		return -1;

}

bool Client::receive(Message *msg) {

	zmq::message_t msg_t;
	int counter = 0;
	while (!p_socket_->recv(&msg_t, ZMQ_NOBLOCK) && counter < timeout_) {
		counter++;
		usleep(1000);
	}

	if (counter < timeout_) {

		if (reconnected_)
		{
			reconnected_ = false;
			std::cout << "Server connection reestablished!                  " << std::endl;
		}
		return msg->unpack(&msg_t);

	} else {

		if (fails_ < retries_ || retries_ == -1) {
			fails_++;
			reconnect();
			std::cout << "Server timeout, trying to reconnect (" << fails_ << ")                           " << std::endl;
			std::cout << "\e[A";
		} else {
			reconnect();
			p_socket_->close();
		}
	}
	return false;

}

bool Client::send_receive(Message *request, Message *reply) {
	send(request);
	return receive(reply);
}

void Client::loop() {
	zmq::context_t context(1);
	this->connect(adr_, &context);

	while (1) {
		if (rate_ms_ > 0)
			boost::this_thread::sleep(boost::posix_time::milliseconds(rate_ms_));

		p_send_Msg_->clear();
		pre_Comm();
		if (send_receive(p_send_Msg_, p_receive_Msg_))
			post_Comm();
	}
}

void Client::dispatch_Communication(std::string adr, int rate_ms) {
	rate_ms_ = rate_ms;
	adr_ = adr;
	if (rate_ms_ > 0)
		std::cout << "Starting Communication Thread with a rate of " << rate_ms_ << "ms" << std::endl;
	else
		std::cout << "Starting triggered Communication Thread" << std::endl;

	boost::thread client_thread(&Client::loop, this);
	t = client_thread.move();
}

void Client::init(std::string name, int priority, Message* send_Msg, Message* reveive_Msg) {
	p_socket_ = 0;
	p_context_ = 0;
	msg_id_ = 0;
	priority_ = priority;

	sender_id_ = name;

	if (send_Msg) {
		send_type_ = send_Msg->get_Type();
		send_type_str_ = send_Msg->get_Data_Type();
		p_send_Msg_ = send_Msg;
	}

	if (reveive_Msg) {
		receive_type_ = reveive_Msg->get_Type();
		receive_type_str_ = reveive_Msg->get_Data_Type();
		p_receive_Msg_ = reveive_Msg;
	}

	retries_ = -1;
	timeout_ = 200;
	fails_ = 0;
	rate_ms_ = -1;
}

void Client::pre_Comm() {
}

void Client::post_Comm() {
}

void Client::wait() {
	t.join();
}
