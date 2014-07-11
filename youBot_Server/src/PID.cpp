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

#include "PID.hpp"

PID::PID() {
	error_1_ =0;
	error_2_ =0;
	error_3_ =0;
	error_sum_ =0;
	last_ret_ =0;
	P_ = 0;
	D_ = 0;
	I_ = 0;
	T_ = 0;
	limit_ = 0;
	change_limit_ = 0;
	sum_limit_ = 0;


}

PID::~PID() {

}

float PID::run(float current, float setpoint, float pilot) {
	return run(setpoint-current,pilot);
}

float PID::run(float error, float pilot) {

	// Pre
	float ret;

	float error_change = (error + 3.0 * error_1_ - 3.0 * error_2_ - error_3_)/6.0;

	// Main

	ret = P_ * ( error + T_ * I_ * error_sum_ + D_/T_ * error_change ) + pilot;

	//Ramp Limit
	if(ret > limit_)
		ret = limit_;

	if(ret< -limit_)
		ret = -limit_;

	//RampGain Limit

	if((ret - last_ret_) > change_limit_)
		ret = last_ret_ + change_limit_;

	if((ret - last_ret_) < -change_limit_)
		ret = last_ret_ - change_limit_;

	// Post
	last_ret_ = ret;
	error_3_ = error_2_;
	error_2_ = error_1_;
	error_1_ = error;

	error_sum_ += error;

	if(error_sum_> sum_limit_)
		error_sum_ = sum_limit_;

	if(error_sum_< -sum_limit_)
		error_sum_ = -sum_limit_;

	return ret;
}

void PID::reset() {
	error_1_ =0;
	error_2_ =0;
	error_3_ =0;
	error_sum_ =0;
}

void PID::setup(float P, float I, float D, float T, float limit,
		float change_limit, float sum_limit) {

	P_ = P;
	D_ = D;
	I_ = I;
	T_ = T;
	limit_ = limit;
	change_limit_ = change_limit;
	sum_limit_ = sum_limit;

}

float PID::run(float error) {
	return run(error,0);
}

void PID::tune(float P, float I, float D) {
	P_ = P;
	D_ = D;
	I_ = I;
	print();
}

void PID::print() {

	std::cout << "    PID:" << std::setw(3) << "P" << " | ";
	std::cout << std::setw(5) << "I" << " | ";
	std::cout << std::setw(5) << "D" << " | ";
	std::cout << std::setw(5) << "T" << " | "<< std::endl;
	std::cout << "      " << std::setw(5) << P_ << " | ";
	std::cout << std::setw(5) << I_ << " | ";
	std::cout << std::setw(5) << D_ << " | ";
	std::cout << std::setw(5) << T_ << " | "<< std::endl;


	std::cout << " " << std::setw(10) << "Lim" << " | ";
	std::cout << std::setw(10) << "C_Lim" << " | ";
	std::cout << std::setw(8) << "S_Lim" << " | "<< std::endl;
	std::cout << " " << std::setw(10) << limit_ << " | ";
	std::cout << std::setw(10) << change_limit_ << " | ";
	std::cout << std::setw(8) << sum_limit_ << " | "<< std::endl;
}
