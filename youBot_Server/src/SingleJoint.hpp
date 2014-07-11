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

#ifndef SINGLEJOINT_HPP_
#define SINGLEJOINT_HPP_

#include "youbot/YouBotManipulator.hpp"
#include "youBot_Reply.hpp"
#include "youBot_Request.hpp"
#include <algorithm>    // std::sort
#include "PID.hpp"

class SingleJoint {

private:
	youbot::JointSensedAngle current_Angle_;
	youbot::JointSensedVelocity current_Velocity_;
	youbot::JointSensedVelocity current_Velocity_Filterd_;
	youbot::JointSensedCurrent current_Current_;
	unsigned int status_;
	youbot::ClearI2tExceededFlag i2t_clear;

	quantity<plane_angle> lower_limit_;
	quantity<plane_angle> upper_limit_;
	quantity<plane_angle> limit_buffer_;

	quantity<angular_velocity> max_velocity_;
	quantity<angular_acceleration> max_acceleration_;

	youbot::YouBotJoint *p_joint_;
	boost::shared_ptr<boost::mutex>  p_mtx_;

	std::vector<float> last_vel;
	PID pid_;
	int joint_num_;
	int velocity_counter_;
	float offset_;
	float dummy_position_;
public:
	SingleJoint();
	virtual ~SingleJoint();

	void setup(boost::shared_ptr<youbot::YouBotManipulator> myYouBotManipulator, int joint_nummer,float offset, int control_rate_ms_,boost::shared_ptr<boost::mutex>  p_mtx);

	void update();

	int set_Position(float position);
	int set_Velocity(float velocity);
	int set_Current(float current);

	int set_PID(float pos, float vel);


	int stop_Joint();
	int clear_Error();

	float get_Upper_Limit();
	float get_Lower_Limit();
	float get_MaxVelocity();
	float get_MaxAccelertion();

	void get_Position(float &position);
	void get_Velocity(float &velocity);
	void get_Filterd_Velocity(float &velocity);
	void get_Current(float &current);

	bool has_Status(unsigned int status);
	void get_Status(unsigned int &status) ;
	Joint_Information get_Joint_Info();

	void tune_PID(float P,float I, float D);

	void print();
};

#endif /* SINGLEJOINT_HPP_ */
