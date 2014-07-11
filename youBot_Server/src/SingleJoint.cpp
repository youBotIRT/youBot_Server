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

#include "SingleJoint.hpp"

SingleJoint::SingleJoint() {
	velocity_counter_ = 0;
	status_ = 0;
	p_joint_ = NULL;
	joint_num_ = 0;
	offset_ = 0;
	last_vel.resize(11, 0);
	dummy_position_ = 0.15;
}

SingleJoint::~SingleJoint() {
}

void SingleJoint::update() {
	if (p_joint_) {
		try {
			p_mtx_->lock();
			p_joint_->getData(current_Angle_);
			p_joint_->getData(current_Velocity_);
			p_joint_->getData(current_Current_);
			p_joint_->getStatus(status_);
			p_mtx_->unlock();
		} catch (std::exception& e) {
			cout << e.what() << endl;
		}
	} else {
		current_Angle_.angle = dummy_position_ * radian;
		current_Current_.current = 0 * ampere;
		current_Velocity_.angularVelocity = 0 * radian_per_second;
		status_ = 0;
	}
	last_vel.pop_back();
	last_vel.insert(last_vel.begin(), current_Velocity_.angularVelocity.value());
	std::vector<float> sorted = last_vel;
	std::sort(sorted.begin(), sorted.end());
	current_Velocity_Filterd_.angularVelocity = sorted[floor(sorted.size() / 2) + 1] * radian_per_second;

}

void SingleJoint::setup(boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator, int joint_nummer,
		float offset, int control_rate_ms_, boost::shared_ptr<boost::mutex> p_mtx) {

	if (p_myYouBotManipulator)
		p_joint_ = &p_myYouBotManipulator->getArmJoint(joint_nummer + 1);
	else
		p_joint_ = 0;

	p_mtx_ = p_mtx;
	joint_num_ = joint_nummer;
	offset_ = offset;

	youbot::JointLimitsRadian lim;
	youbot::InverseMovementDirection dir;
	youbot::MotorAcceleration max_acc;
	youbot::MaximumPositioningVelocity max_vel;

	if (p_joint_) {
		p_mtx_->lock();
		p_joint_->getConfigurationParameter(lim);
		p_joint_->getConfigurationParameter(dir);
		p_joint_->getConfigurationParameter(max_acc);
		p_joint_->getConfigurationParameter(max_vel);
		p_mtx_->unlock();
		quantity<plane_angle> tmp_lower_limit;
		quantity<plane_angle> tmp_upper_limit;
		bool active;
		bool direction;
		lim.getParameter(tmp_lower_limit, tmp_upper_limit, active);
		dir.getParameter(direction);

		if (direction) {
			lower_limit_ = -tmp_upper_limit;
			upper_limit_ = -tmp_lower_limit;
		} else {
			lower_limit_ = tmp_lower_limit;
			upper_limit_ = tmp_upper_limit;
		}

		limit_buffer_ = 1.0 / 180.0 * M_PI * radian;

		max_acc.getParameter(max_acceleration_);
		max_vel.getParameter(max_velocity_);
		pid_.setup(1, 0, 0, ((float) control_rate_ms_) / 1000.0, max_velocity_.value(),
				max_acceleration_.value() * ((float) control_rate_ms_) / 1000.0, 1);
	} else {
		if (joint_num_ == 2) {
			dummy_position_ = -0.15;
		}
	}

}

int SingleJoint::set_Position(float position) {

	if ((isinf(position) != 0) || isnan(position))
		position = current_Angle_.angle.value();

	youbot::JointAngleSetpoint joint_angle;
	joint_angle.angle = (position + offset_) * radian;

	if (joint_angle.angle > upper_limit_ - limit_buffer_)
		joint_angle.angle = upper_limit_ - limit_buffer_;
	if (joint_angle.angle < lower_limit_ + limit_buffer_)
		joint_angle.angle = lower_limit_ + limit_buffer_;
	p_mtx_->lock();
	try {
		if (p_joint_) {
			//std::cout << joint_angle.angle << std::endl;
			p_joint_->setData(joint_angle);
		}

	} catch (std::exception& e) {
		cout << e.what() << endl;
	}
	p_mtx_->unlock();
	return 0;
}

int SingleJoint::set_Velocity(float velocity) {

	if ((isinf(velocity) != 0) || isnan(velocity))
		velocity = 0;

	youbot::JointVelocitySetpoint joint_velocity;
	youbot::JointAngleSetpoint joint_angle;
	joint_angle.angle = current_Angle_.angle;
	joint_velocity.angularVelocity = velocity * radian_per_second;

	if ((current_Angle_.angle) > (upper_limit_ - limit_buffer_)
			&& joint_velocity.angularVelocity > 0.0 * radian_per_second)
		joint_velocity.angularVelocity = 0.0 * radian_per_second;
	else if ((current_Angle_.angle) > (upper_limit_ - 2.0 * limit_buffer_)
			&& joint_velocity.angularVelocity > 0.0 * radian_per_second)
		joint_velocity.angularVelocity /= 2.0;

	if ((current_Angle_.angle) < (lower_limit_ + limit_buffer_)
			&& joint_velocity.angularVelocity < 0.0 * radian_per_second)
		joint_velocity.angularVelocity = 0.0 * radian_per_second;
	else if ((current_Angle_.angle) < (lower_limit_ + 2.0 * limit_buffer_)
			&& joint_velocity.angularVelocity < 0.0 * radian_per_second)
		joint_velocity.angularVelocity /= 2.0;

	if (velocity == 0 && velocity_counter_ < 104)
		velocity_counter_++;
	else {
		if (velocity != 0)
			velocity_counter_ = 0;
	}
	p_mtx_->lock();

	if (p_joint_) {
		try {

			if (velocity_counter_ >= 100 && velocity == 0)
				p_joint_->setData(joint_angle);
			else
				p_joint_->setData(joint_velocity);

		} catch (std::exception& e) {
			//cout << e.what() << endl;
		}
	}
	p_mtx_->unlock();

	return 0;
}

int SingleJoint::set_Current(float current) {

	if ((isinf(current) != 0) || isnan(current))
		current = 0;

	youbot::JointCurrentSetpoint joint_current;
	joint_current.current = current * ampere;
	p_mtx_->lock();
	if (p_joint_) {
		try {
			p_joint_->setData(joint_current);
		} catch (std::exception& e) {
			//cout << e.what() << endl;
		}
	}
	p_mtx_->unlock();
	return 0;
}

int SingleJoint::stop_Joint() {
	p_mtx_->lock();
	try {
		if (p_joint_)
			p_joint_->stopJoint();
	} catch (std::exception& e) {
		//cout << e.what() << endl;
	}
	p_mtx_->unlock();
	return 0;
}

int SingleJoint::clear_Error() {
	if (has_Status(youbot::I2T_EXCEEDED)) {
		try {
			p_mtx_->lock();
			if (p_joint_)
				p_joint_->setConfigurationParameter(i2t_clear);
			p_mtx_->unlock();
		} catch (std::exception& e) {
			//cout << e.what() << endl;
		}
	} else
		stop_Joint();
	return 0;
}

float SingleJoint::get_Upper_Limit() {
	return upper_limit_.value();
}

float SingleJoint::get_Lower_Limit() {
	return lower_limit_.value();
}

float SingleJoint::get_MaxVelocity() {
	return max_velocity_.value();
}

float SingleJoint::get_MaxAccelertion() {
	return max_acceleration_.value();
}

void SingleJoint::get_Position(float& position) {
	position = current_Angle_.angle.value() - offset_;
}

void SingleJoint::get_Velocity(float& velocity) {
	velocity = current_Velocity_.angularVelocity.value();
}

void SingleJoint::get_Current(float& current) {
	current = current_Current_.current.value();
}

Joint_Information SingleJoint::get_Joint_Info() {
	Joint_Information ji;
	ji.joint = joint_num_;
	get_Position(ji.position);
	get_Velocity(ji.velocity);
	get_Current(ji.current);
	get_Status(ji.status);
	return ji;
}

void SingleJoint::get_Status(unsigned int &status) {
	status = status_;
}

int SingleJoint::set_PID(float pos, float vel) {
	return set_Velocity(pid_.run(current_Angle_.angle.value(), pos + offset_, vel));
}

void SingleJoint::tune_PID(float P, float I, float D) {
	std::cout << "Joint " << joint_num_ + 1 << " tuned: " << std::endl;
	pid_.tune(P, I, D);
}

void SingleJoint::get_Filterd_Velocity(float& velocity) {
	velocity = current_Velocity_Filterd_.angularVelocity.value();
}

void SingleJoint::print() {
	cout << "   >-----------" << endl;
	cout << "    Joint Controller for Joint " << joint_num_ + 1 << " started." << endl;
	cout << "    Upper Limit: " << upper_limit_.value() << endl;
	cout << "    Lower Limit: " << lower_limit_.value() << endl;
	cout << "    Max Velocity: " << max_velocity_.value() << endl;
	cout << "    Max Acceleration: " << max_acceleration_.value() << endl;
	cout << "    Offset: " << offset_ << endl;
	pid_.print();
}

bool SingleJoint::has_Status(unsigned int status) {
	if (status & status_) {
		return true;
	} else {
		return false;
	}
}
