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

#include "ArmCommander.hpp"

Arm_Commander::Arm_Commander() {
	num_ = 0;
}

Arm_Commander::~Arm_Commander() {
}

void Arm_Commander::setup(int num, boost::shared_ptr<youbot::YouBotManipulator> myYouBotManipulator,
		boost::shared_ptr<boost::mutex> mtx) {

	mtx_ = mtx;
	myYouBotManipulator_ = myYouBotManipulator;
	num_ = num;

	offset_[0] = +2.91748;
	offset_[1] = +1.13579 + M_PI / 2.0;
	offset_[2] = -2.55644;
	offset_[3] = +1.7867 + M_PI / 2.0;
	offset_[4] = +2.925;

	joints_ = boost::shared_ptr<JointController>(new JointController());
	joints_->setup(myYouBotManipulator_, mtx_);
	complex_Motion_.setup(joints_, COMPLEX_MOTION_LOOP);

}

void Arm_Commander::add_Arm_Info(youBot_Reply* msg) {

	if (complex_Motion_.in_Moiton())
		msg->add_State(IN_MOTION);

	joints_->add_Arm_Info(msg);

}

void Arm_Commander::execute_Command(youBot_Request* msg) {



	if (msg->get_Number_Of_Joint_Msg() > 0) {


		if (complex_Motion_.in_Moiton())
			complex_Motion_.stop_Action();

		joints_->execute_Commands(msg);
	}

	else if (msg->get_Complex_Type() > 0) {
		complex_Motion_.update_Action(msg->get_Complex_Type(), msg->get_Complex_Values());
	}

}

void Arm_Commander::stop() {

	complex_Motion_.stop_Action();
	joints_->stop();
	joints_->reset();
}

void Arm_Commander::reset() {
	complex_Motion_.stop_Action();
	joints_->stop();
}

bool Arm_Commander::exists() {
	if (myYouBotManipulator_)
		return true;
	else
		return false;
}

