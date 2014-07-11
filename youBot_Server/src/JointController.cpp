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

#include "JointController.hpp"

JointController::JointController() {
	loop_rate_ms_ = 10;

}

void JointController::setup(boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator,
		boost::shared_ptr<boost::mutex> mtx) {
	float offset[5];
	offset[0] = +2.91748;
	offset[1] = +1.13579 + M_PI / 2.0;
	offset[2] = -2.55644;
	offset[3] = +1.7867 + M_PI / 2.0;
	offset[4] = +2.925;

	loop_rate_ms_ = SINGLE_JOINT_LOOP;

	for (int i = 0; i < 5; i++) {
		joint[i].setup(p_myYouBotManipulator, i, offset[i],loop_rate_ms_, mtx);
		joint[i].print();
	}

	std::vector<float> init_vals;
	init_vals.resize(5, 0);
	setpoint_.resize(3, init_vals);

	type_.resize(5,-1);
	std::cout << "Starting Joint Control Loop (" << loop_rate_ms_ << "ms)" << endl;
	boost::thread t(&JointController::loop,this);

}

void JointController::loop() {

	while (true) {

		for (int i = 0; i < 5; i++) {

			joint[i].update();

			switch (type_[i]) {
			case 0: //POSITION
				joint[i].set_Position(setpoint_[0][i]);
				break;
			case 1: //VELOCITY
				joint[i].set_Velocity(setpoint_[0][i]);
				break;
			case 2: //CURRENT
				joint[i].set_Current(setpoint_[0][i]);
				break;
			case 3: //TRAJECTORY ONLY POS
				joint[i].set_Position(setpoint_[0][i]);
				break;
			case 4: //TRAJECTORY POS + VEL
				joint[i].set_PID(setpoint_[0][i],setpoint_[1][i]);
				break;
			case 5: //TRAJECTORY POS + VEL + ACC
				joint[i].set_PID(setpoint_[0][i],setpoint_[1][i]);
				break;
			case 6: // CLEAR Error
				joint[i].clear_Error();
				break;
			case 7: // Stop
				joint[i].stop_Joint();
				break;
			default:
				break;
			}
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(loop_rate_ms_));
	}
}

void JointController::execute_Commands(youBot_Request *req) {

	for (int i = 0; i < req->get_Number_Of_Joint_Msg(); i++) {
		type_[req->get_Joint_Msg(i).joint] = req->get_Joint_Msg(i).type;
		setpoint_[0][req->get_Joint_Msg(i).joint] = req->get_Joint_Msg(i).value;
	}

}

void JointController::update_Trajectory(std::vector<float> position, std::vector<float> velocity,
		std::vector<float> acceleration) {

	type_.clear();
	type_.resize(5, 5);
	setpoint_[0] = position;
	setpoint_[1] = velocity;
	setpoint_[2] = acceleration;

}

void JointController::update_Trajectory(std::vector<float> position, std::vector<float> velocity) {
	type_.clear();
	type_.resize(5, 4);
	setpoint_[0] = position;
	setpoint_[1] = velocity;

}
void JointController::update_Trajectory(std::vector<float> position) {
	type_.clear();
	type_.resize(5, 3);
	setpoint_[0] = position;
}

void JointController::add_Arm_Info(youBot_Reply* msg) {
	for (int i = 0; i < 5; i++) {
		msg->add_Joint_Info(joint[i].get_Joint_Info());
	}
}

void JointController::stop() {
	type_.clear();
	type_.resize(5, 7);
}

void JointController::reset() {
	type_.clear();
	type_.resize(5, 6);
}

std::vector<float> JointController::get_Positions() {
	std::vector<float> q;
	q.resize(5,0);
	for (int i = 0; i < 5; i++) {
		joint[i].get_Position(q[i]);
	}
	return q;
}

std::vector<float> JointController::get_Velocitys() {
	std::vector<float> dq;
	dq.resize(5,0);
	for (int i = 0; i < 5; i++) {
		joint[i].get_Velocity(dq[i]);
	}
	return dq;
}

std::vector<float> JointController::get_Filterd_Velocitys() {
	std::vector<float> dq;
	dq.resize(5,0);
	for (int i = 0; i < 5; i++) {
		joint[i].get_Filterd_Velocity(dq[i]);
	}
	return dq;
}

std::vector<float> JointController::get_Max_Velocitys() {
	std::vector<float> dq;
	dq.resize(5,0);
	for (int i = 0; i < 5; i++) {
		dq[i]=joint[i].get_MaxVelocity();
	}
	return dq;
}

std::vector<float> JointController::get_Max_Accelerations() {
	std::vector<float> dq;
	dq.resize(5,0);
	for (int i = 0; i < 5; i++) {
		dq[i]=joint[i].get_MaxAccelertion();
	}
	return dq;
}

SingleJoint* JointController::get_Joint(int i) {
	return &joint[i];

}
