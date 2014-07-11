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

#include "MotionCommander.hpp"

Motion_Commander::Motion_Commander() {
	last_type_ = -1;
	active_ = false;
	action_ = NULL;
	rate_ =0;
}

Motion_Commander::~Motion_Commander() {
	active_ = false;
}

void Motion_Commander::setup(boost::shared_ptr<JointController> p_joints, int rate) {

	std::cout << "Starting Interpolator (" << rate << "ms)" << endl;
	p_joints_ = p_joints;
	rate_ = rate;
	last_type_ = -1;
	active_ = false;
	action_ = 0;
}

bool Motion_Commander::in_Moiton() {
	if (action_)
		return action_->is_active();
	else
		return false;
}

void Motion_Commander::update_Action(int type, std::vector<float> value) {
	bool start = false;
	if (last_type_ != type) {

		last_type_ = type;

		if (action_) {
			action_->cancel();
			motion_Thread_.join();
			delete action_;
			action_ = 0;
		}

		if (type == 1) {
			action_ = new PTP_Motion();
			start = true;
		} else if (type == 2) {
			action_ = new Linear_Velocity();
			start = true;
		} else if (type == 3) {
			action_ = new Trajectory();
			start = true;
		} else if (type == 1337) {
			last_type_ =-1;
			if (value.size() == 4) {
				int j = (int) value[0];
				if (j >= 0 && j < 5){
					p_joints_->get_Joint(j)->tune_PID(value[1], value[2], value[3]);
				}
				if(j <0)
				{
					for(int i=0;i<5;i++)
					{
						p_joints_->get_Joint(i)->tune_PID(value[1], value[2], value[3]);
					}
				}
			}
		}

		if (start) {
			action_->setup(p_joints_, value);
			boost::thread t(&Motion_Commander::loop, this);
			motion_Thread_ = t.move();
		}
	} else
		action_->refresh(value);

}

void Motion_Commander::stop_Action() {
	if (action_)
		action_->cancel();
}

void Motion_Commander::loop() {

	while (action_->increment(rate_)) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(rate_));
	}
	last_type_ = -1;
}

