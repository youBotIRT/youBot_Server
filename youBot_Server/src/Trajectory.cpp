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

#include "Trajectory.hpp"

Trajectory::Trajectory() {
	dt_ = 0;
}

Trajectory::~Trajectory() {

}

bool Trajectory::increment_internal(int rate_ms) {

	int ret =0;

	std::vector<float>target_position,target_velocity;

	kin_.fkin_pos(joints_->get_Positions(), current_);

	step_ = velocity_;
	step_.Mult((((float)rate_ms)/1000.0));

	target_.add(step_);

//
//	cout << dt_ << endl;
//	cout << current.toString() << endl;
//	cout << target.toString() << endl;


	dt_ -= ((float)rate_ms)/1000.0;


	kin_conf conf=kin_.get_Current_Config(joints_->get_Positions());
	ret = kin_.ikin_vel(current_,velocity_,conf,target_position,target_velocity);
	ret |= kin_.ikin_pos(target_, conf, target_position);

	if(dt_>0){
		joints_->update_Trajectory(target_position,target_velocity);
		if (ret)
			return false;
		else
			return true;
	}
	else
	{
		joints_->stop();
		return false;
	}

}

void Trajectory::refresh(std::vector<float> msg) {

	kin_.fkin_pos(joints_->get_Positions(), current_);
	startpoint_ =  targetpoint_;

	targetpoint_.x = msg[0];
	targetpoint_.y = msg[1];
	targetpoint_.z = msg[2];
	targetpoint_.A = msg[3];
	targetpoint_.B = msg[4];

	dt_ = msg[5];
	velocity_.distance(targetpoint_,startpoint_);
	velocity_.Mult(1/dt_);

	dt_ *= 2;
}

void Trajectory::internal_setup(std::vector<float> msg) {

	kin_.fkin_pos(joints_->get_Positions(), current_);

	targetpoint_.x = msg[0];
	targetpoint_.y = msg[1];
	targetpoint_.z = msg[2];
	targetpoint_.A = msg[3];
	targetpoint_.B = msg[4];
	startpoint_ = current_;
	dt_ = msg[5];
	velocity_.distance(targetpoint_,startpoint_);
	velocity_.Mult(1/dt_);
	target_ = current_;
	dt_ *= 2;
}
