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

#include "GripperCommander.hpp"

GripperCommander::GripperCommander() {

	has_gripper_ = false;
	num_=0;
	position_ =0;


}

GripperCommander::~GripperCommander() {

}

void GripperCommander::setup(int num, boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator,
		boost::shared_ptr<boost::mutex> p_mtx) {
	std::cout << "Starting Gripper Controller (250ms)" << std::endl;
	p_mtx_ = p_mtx;
	p_myYouBotManipulator_ = p_myYouBotManipulator;
	has_gripper_ = false;
	num_ = num;
	if (p_myYouBotManipulator_) {
		try {
			p_myYouBotManipulator_->getArmGripper().close();
			has_gripper_ = true;
			boost::thread tmp_thread(&GripperCommander::loop, this);
		} catch (std::exception& e) {
			has_gripper_ = false;
		}
	}

}

void GripperCommander::add_Gripper_Info(youBot_Reply* msg) {
	Gripper_Information gi;
	gi.num = num_;
	gi.position = position_;
	msg->add_Gripper_Info(gi);
}

void GripperCommander::execute_Command(youBot_Request* msg) {

	if (msg->get_Number_Of_Gripper_Msg() > 0) {
		if (p_myYouBotManipulator_) {
			if (has_gripper_) {
				youbot::GripperBarSpacingSetPoint barSpacing;
				barSpacing.barSpacing = msg->get_Gripper_Msg(num_).value * meter;
				p_mtx_->lock();
				try {
					p_myYouBotManipulator_->getArmGripper().setData(barSpacing);
				} catch (std::exception& e) {
					cout << e.what() << endl;
				}
				p_mtx_->unlock();
			}
		}
	}
}

bool GripperCommander::exists() {
	return has_gripper_;
}

void GripperCommander::reset() {
}

void GripperCommander::stop() {
}

void GripperCommander::loop() {
	while (true) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(250));
		youbot::GripperSensedBarSpacing space;
		p_mtx_->lock();
		try {
			p_myYouBotManipulator_->getArmGripper().getData(space);
			position_ = space.barSpacing.value();
		} catch (std::exception& e) {
			position_ =0;
			cout << e.what() << endl;
		}
		p_mtx_->unlock();

	}
}
