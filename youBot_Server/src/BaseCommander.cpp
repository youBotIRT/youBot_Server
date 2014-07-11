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

#include "BaseCommander.hpp"

Base_Commander::Base_Commander() {

	dummy_position_[0] = 0;
	dummy_position_[1] = 0;
	dummy_position_[2] = 0;

	dummy_velocity_[0] = 0;
	dummy_velocity_[1] = 0;
	dummy_velocity_[2] = 0;

	num_ = 0;
	resetting_ = false;

}

Base_Commander::~Base_Commander() {

}

void Base_Commander::add_Base_Info(youBot_Reply *msg) {
	Base_Information bi;
	bi.num = num_;
	if (p_myYouBotBase_) {
		quantity<si::velocity> longitudinalVelocity;
		quantity<si::velocity> transversalVelocity;
		quantity<si::angular_velocity> angularVelocity;
		quantity<si::length> longitudinalPosition;
		quantity<si::length> transversalPosition;
		quantity<plane_angle> orientation;
		unsigned int status[4];

		p_mtx_->lock();
		p_myYouBotBase_->getBasePosition(longitudinalPosition, transversalPosition, orientation);
		p_myYouBotBase_->getBaseVelocity(longitudinalVelocity, transversalVelocity, angularVelocity);
		p_myYouBotBase_->getBaseJoint(1).getStatus(status[0]);
		p_myYouBotBase_->getBaseJoint(2).getStatus(status[1]);
		p_myYouBotBase_->getBaseJoint(3).getStatus(status[2]);
		p_myYouBotBase_->getBaseJoint(4).getStatus(status[3]);
		p_mtx_->unlock();
		bi.position.push_back((float) (longitudinalPosition / meter));
		bi.position.push_back((float) (transversalPosition / meter));
		bi.position.push_back((float) (orientation / radian));

		bi.velocity.push_back((float) (longitudinalVelocity / meter_per_second));
		bi.velocity.push_back((float) (transversalVelocity / meter_per_second));
		bi.velocity.push_back((float) (angularVelocity / radian_per_second));
		bi.status.push_back(status[0]);
		bi.status.push_back(status[1]);
		bi.status.push_back(status[2]);
		bi.status.push_back(status[3]);

		msg->add_Base_Info(bi);
	} else {
		bi.position.push_back(dummy_position_[0]);
		bi.position.push_back(dummy_position_[1]);
		bi.position.push_back(dummy_position_[2]);

		bi.velocity.push_back(dummy_velocity_[0]);
		bi.velocity.push_back(dummy_velocity_[1]);
		bi.velocity.push_back(dummy_velocity_[2]);

		bi.status.push_back(0);
		bi.status.push_back(0);
		bi.status.push_back(0);
		bi.status.push_back(0);

		msg->add_Base_Info(bi);

	}

}

void Base_Commander::execute_Command(youBot_Request *msg) {

	int s = 0;
	bool found = false;
	while (!found && s < msg->get_Number_Of_Base_Msg()) {
		if (msg->get_Base_Msg(s).num == num_)
			found = true;

		s++;
	}
	s--;
	if (found) {
		if (p_myYouBotBase_) {
			if (msg->get_Base_Msg(s).type == 0) {
				quantity<si::length> longitudinalPosition = msg->get_Base_Msg(s).value[0] * meter;
				quantity<si::length> transversalPosition = msg->get_Base_Msg(s).value[1] * meter;
				quantity<plane_angle> orientation = msg->get_Base_Msg(s).value[2] * radian;
				p_mtx_->lock();
				p_myYouBotBase_->setBasePosition(longitudinalPosition, transversalPosition, orientation);
				p_mtx_->unlock();
			} else if (msg->get_Base_Msg(s).type == 1) {
				quantity<si::velocity> longitudinalVelocity = msg->get_Base_Msg(s).value[0] * meter_per_second;
				quantity<si::velocity> transversalVelocity = msg->get_Base_Msg(s).value[1] * meter_per_second;
				quantity<si::angular_velocity> angularVelocity = msg->get_Base_Msg(s).value[2] * radian_per_second;
				p_mtx_->lock();
				p_myYouBotBase_->setBaseVelocity(longitudinalVelocity, transversalVelocity, angularVelocity);
				p_mtx_->unlock();
			}
		} else {
			if (msg->get_Base_Msg(s).type == 0) {
				dummy_position_[0] = msg->get_Base_Msg(s).value[0];
				dummy_position_[1] = msg->get_Base_Msg(s).value[1];
				dummy_position_[2] = msg->get_Base_Msg(s).value[2];
			} else if (msg->get_Base_Msg(s).type == 1) {
				dummy_velocity_[0] = msg->get_Base_Msg(s).value[0];
				dummy_velocity_[1] = msg->get_Base_Msg(s).value[1];
				dummy_velocity_[2] = msg->get_Base_Msg(s).value[2];
			}
		}
	}
}

void Base_Commander::setup(int num, boost::shared_ptr<youbot::YouBotBase> p_myYouBotBase,
		boost::shared_ptr<boost::mutex> p_mtx) {

	std::cout << "Starting Base Controller " << std::endl;
	p_mtx_ = p_mtx;
	p_myYouBotBase_ = p_myYouBotBase;
	num_ = num;

	resetting_ = false;

}

void Base_Commander::reset() {
	if (p_myYouBotBase_) {
		cout << "Base Reset" << endl;

		if (!resetting_) {
			resetting_ = true;
			boost::thread tmp_thread(&Base_Commander::reset_thread, this);
		}
	}
}

void Base_Commander::stop() {
	if (p_myYouBotBase_) {
		for (int j = 1; j <= 4; j++) {
			p_mtx_->lock();
			p_myYouBotBase_->getBaseJoint(j).stopJoint();
			p_mtx_->unlock();
		}
	}
}

bool Base_Commander::exists() {
	if (p_myYouBotBase_)
		return true;
	else
		return false;
}

void Base_Commander::reset_thread() {
	if (p_myYouBotBase_) {
		youbot::ClearI2tExceededFlag i2t_clear;

		for (int i = 0; i < 4; i++) {

			unsigned int status;
			p_mtx_->lock();
			p_myYouBotBase_->getBaseJoint(i + 1).getStatus(status);
			int counter = 0;

			if (status & youbot::I2T_EXCEEDED)
				counter = 100;
			else
				cout << "No Error for Base Joint " << i + 1 << endl;

			cout << "Resetting Base Joint " << i + 1 << " ..." << endl;
			p_mtx_->unlock();

			while (counter > 0) {

				p_mtx_->lock();
				p_myYouBotBase_->getBaseJoint(i + 1).setConfigurationParameter(i2t_clear);
				p_myYouBotBase_->getBaseJoint(i + 1).getStatus(status);
				p_mtx_->unlock();

				if (status & youbot::I2T_EXCEEDED)
					counter = 100;
				else
					counter--;

				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			}
		}
		cout << "All Base Joints OK" << endl;
		resetting_ = false;
	}
}

