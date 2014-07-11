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

#include "PTPMotion.hpp"

PTP_Motion::PTP_Motion() {

	space =0;

}

PTP_Motion::~PTP_Motion() {

}

bool PTP_Motion::increment_internal(int rate_ms) {
	std::vector<float>target_position,target_velociy;
	bool ret = ramp_.generate_next_point(rate_ms, target_position, target_velociy);
	joints_->update_Trajectory(target_position,target_velociy);
	return ret;

}

void PTP_Motion::internal_setup(std::vector<float> msg) {
	std::vector<float> startpos;
	std::vector<float> endpos;
	std::vector<float> max_vel;
	std::vector<float> max_acc;


	startpos = joints_->get_Positions();
	max_vel = joints_->get_Max_Velocitys();
	max_acc = joints_->get_Max_Accelerations();

	if(msg[0]<0){
		cartesian p;
		p.x = msg[1];
		p.y = msg[2];
		p.z = msg[3];
		p.A = msg[4];
		p.B = msg[5];

		kin.ikin_pos(p,kin.get_Current_Config(startpos),endpos);
	}
	else
	{
		for(int i=1;i<=5;i++)
		{
			endpos.push_back(msg[i]);
		}
	}

	float speed;

	speed = msg[6];

	if (speed > 1)
		speed = 1;


	if (speed < 0.01)
		speed = 00.01;

	cout << "PTP Motion Started " << endl;
	cout << "Speed: " << speed << endl;


	ramp_.setup(startpos, endpos, max_vel, max_acc, speed);
}

void PTP_Motion::refresh( std::vector<float> msg) {
	ramp_ = Ramp_Generator();
	internal_setup( msg);
}
