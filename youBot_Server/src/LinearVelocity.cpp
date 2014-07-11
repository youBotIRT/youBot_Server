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

#include "LinearVelocity.hpp"

Linear_Velocity::Linear_Velocity() {

}

Linear_Velocity::~Linear_Velocity() {

}

bool Linear_Velocity::increment_internal(int rate_ms) {


	cartesian set_velocitiy = velocity_Ramp(velocity);

	std::vector<float>target_position,target_velocity;

	kin.fkin_pos(joints_->get_Positions(), current);

	next = current;

	next.step(set_velocitiy, ((float) rate_ms) / 1000.0f);

	kin_conf conf=kin.get_Current_Config(joints_->get_Positions());
	int ret = kin.ikin_vel(current,set_velocitiy,conf,target_position,target_velocity);
	ret |= kin.ikin_pos(next, conf, target_position);

	joints_->update_Trajectory(target_position,target_velocity);

	if (ret)
		return false;

	return true;
}

void Linear_Velocity::refresh( std::vector<float> msg) {
	velocity.x = msg[0];
	velocity.y = msg[1];
	velocity.z = msg[2];
	velocity.A = msg[3];
	velocity.B = msg[4];
}

cartesian Linear_Velocity::velocity_Ramp(cartesian target_velocity) {
	cartesian vel;
	float max = 0.5;

	vel = target_velocity;

	float total =  sqrt(pow(last_vel.x - vel.x,2)+pow(last_vel.y - vel.y,2) + pow(last_vel.z - vel.z,2) + pow(last_vel.A - vel.A,2)+pow(last_vel.B - vel.B,2));

	if( total  > max )
	{
		vel.sub(last_vel);
		vel.Mult(max/total);
		vel.add(last_vel);
	}




	last_vel = vel;
	return vel;
}

void Linear_Velocity::internal_setup(std::vector<float> msg) {

	velocity.x = msg[0];
	velocity.y = msg[1];
	velocity.z = msg[2];
	velocity.A = msg[3];
	velocity.B = msg[4];

	kin.fkin_vel(joints_->get_Positions(),joints_->get_Filterd_Velocitys(),last_vel);
}

