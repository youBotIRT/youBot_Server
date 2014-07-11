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

#include <iostream>
#include "zmq.hpp"
#include <unistd.h>
#include <vector>
#include <sys/time.h>

#include <iomanip>

#include "Message.hpp"

#include "youBot_Reply.hpp"
#include "youBot_Request.hpp"

#include "ControlManagement.hpp"
#include "BaseCommander.hpp"
#include "ArmCommander.hpp"
#include "GripperCommander.hpp"

#include "youbot/YouBotBase.hpp"
#include "youbot/YouBotManipulator.hpp"

#include "boost/shared_ptr.hpp"

using namespace std;
using namespace youbot;

int main() {

	Control_Management control_;
	Base_Commander base_;
	Arm_Commander arm_;
	GripperCommander gripper_;

	struct timeval current_time;
	struct timeval start_time;

	boost::shared_ptr<YouBotBase> myYouBotBase;
	boost::shared_ptr<YouBotManipulator> myYouBotManipulator;
	boost::shared_ptr<boost::mutex> mtx_(new boost::mutex);

	char* configLocation = getenv("YOUBOT_CONFIG");
	if (configLocation == NULL)
		throw std::runtime_error("youbot_Server: Could not find environment variable YOUBOT_CONFIG");

	try {

		myYouBotBase = boost::shared_ptr<YouBotBase>(new YouBotBase("youbot-base", configLocation));
		myYouBotBase->doJointCommutation();
	} catch (std::exception& e) {
		LOG(warning) << e.what();
	}

	try {
		if (myYouBotBase) {

			myYouBotManipulator = boost::shared_ptr<YouBotManipulator>(
					new YouBotManipulator("youbot-manipulator", configLocation));
		} else {
			myYouBotManipulator = boost::shared_ptr<YouBotManipulator>(
					new YouBotManipulator("youbot-manipulator-single", configLocation));
		}
		myYouBotManipulator->doJointCommutation();
		myYouBotManipulator->calibrateManipulator();
	} catch (std::exception& e) {
	}

	arm_.setup(0, myYouBotManipulator, mtx_);
	base_.setup(0, myYouBotBase, mtx_);
	gripper_.setup(0, myYouBotManipulator, mtx_);

	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5555");
	printf("Server Started\n");
	gettimeofday(&start_time, NULL);

	unsigned long long us_time_Start = (unsigned long long) (start_time.tv_usec)
			+ (unsigned long long) (start_time.tv_sec * 1000000);

	while (true) {

		zmq::message_t request;
		socket.recv(&request);
		youBot_Request request_msg;

		zmq::message_t reply;

		if (request_msg.unpack(&request)) {

			if (!base_.exists() && !arm_.exists()) {
				cout << request_msg.get_Sender_ID() << endl;
				cout << request_msg.toString();
				cout << "--------------" << endl;
			}

			gettimeofday(&current_time, NULL);
			unsigned long long us_time = (unsigned long long) (current_time.tv_usec)
					+ (unsigned long long) (current_time.tv_sec * 1000 * 1000) - us_time_Start;

			youBot_Reply reply_msg;
			reply_msg.set_State(0);

			if (base_.exists())
				reply_msg.add_State(HAS_BASE);

			if (arm_.exists())
				reply_msg.add_State(HAS_ARM);

			if (gripper_.exists())
				reply_msg.add_State(HAS_GRIPPER);

			control_.mangae(&request_msg, &reply_msg);

			if (reply_msg.get_State() & ARM_CONTROL) {
				if (request_msg.get_State_Req() & STOP) {
					arm_.stop();
					gripper_.stop();
				} else if (request_msg.get_State_Req() & RESET) {
					arm_.reset();
					gripper_.reset();
				} else {
					arm_.execute_Command(&request_msg);
					gripper_.execute_Command(&request_msg);
				}

			}

			if (reply_msg.get_State() & BASE_CONTROL) {

				if (request_msg.get_State_Req() & STOP) {
					base_.stop();
				} else if (request_msg.get_State_Req() & RESET) {
					base_.reset();
				} else {
					base_.execute_Command(&request_msg);
				}

			}

			arm_.add_Arm_Info(&reply_msg);
			base_.add_Base_Info(&reply_msg);
			gripper_.add_Gripper_Info(&reply_msg);

			reply_msg.set_Timestamp(us_time);

			reply_msg.set_Sender_ID("YOUBOT::SERVER");
			reply_msg.pack(&reply);
		} else {
			Message msg_send;
			msg_send.set_Sender_ID("YOUBOT::SERVER");
			msg_send.set_Data_Type("PONG");
			msg_send.pack(&reply);
		}

		socket.send(reply);

	}
	return 0;

}

