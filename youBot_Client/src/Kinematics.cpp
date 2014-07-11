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

#include "Kinematics.hpp"

Kinematics::Kinematics() {

	joints_[0].a = 33;		//mm
	joints_[0].d = 147;		//mm
	joints_[0].alpha = M_PI / 2.0;	//rad
	joints_[0].dphi = 0;		//rad
	joints_[0].q_max = 4;	//rad
	joints_[0].q_min = -4;	//rad
	joints_[0].direction = -1;

	joints_[1].a = 155;		//mm
	joints_[1].d = 0;		//mm
	joints_[1].alpha = 0;	//rad
	joints_[1].dphi = 0;		//rad
	joints_[1].q_max = 4;	//rad
	joints_[1].q_min = -4;	//rad
	joints_[1].direction = -1;

	joints_[2].a = 135;		//mm
	joints_[2].d = 0;		//mm
	joints_[2].alpha = 0;	//rad
	joints_[2].dphi = 0;		//rad
	joints_[2].q_max = 4;	//rad
	joints_[2].q_min = -4;	//rad
	joints_[2].direction = -1;

	joints_[3].a = 0;		//mm
	joints_[3].d = 0;		//mm
	joints_[3].alpha = M_PI / 2.0;	//rad
	joints_[3].dphi = M_PI / 2.0;	//rad
	joints_[3].q_max = 4;	//rad
	joints_[3].q_min = -4;	//rad
	joints_[3].direction = -1;

	joints_[4].a = 0;		//mm
	joints_[4].d = 217.5;		//mm
	joints_[4].alpha = 0;	//rad
	joints_[4].dphi = 0;		//rad
	joints_[4].q_max = 4;	//rad
	joints_[4].q_min = -4;	//rad
	joints_[4].direction = -1;
}

Kinematics::~Kinematics() {

}

int Kinematics::fkin_pos(const std::vector<float>& q_in, cartesian& position) {

	for (int i = 0; i < 5; i++) {
		joints_[i].form_Real(q_in[i]);
	}

	position.B = joints_[4].q;
	position.A = joints_[1].q + joints_[2].q + joints_[3].q;

	float r = cos(joints_[1].q) * (joints_[1].a) + cos(joints_[1].q + joints_[2].q) * (joints_[2].a)
			+ cos(joints_[1].q + joints_[2].q + joints_[3].q) * (joints_[3].a + joints_[4].d) + joints_[0].a;

	position.x = cos(joints_[0].q) * r;
	position.y = sin(joints_[0].q) * r;
	position.z = sin(joints_[1].q) * (joints_[1].a) + sin(joints_[1].q + joints_[2].q) * (joints_[2].a)
			+ sin(joints_[1].q + joints_[2].q + joints_[3].q) * (joints_[3].a + joints_[4].d) + joints_[0].d;

	return 0;
}

int Kinematics::fkin_vel(const std::vector<float>& q_in, const std::vector<float>& dq_in, cartesian& velocity) {

	for (int i = 0; i < 5; i++) {
		joints_[i].form_Real(q_in[i], dq_in[i]);

	}

	velocity.B = joints_[4].dq;
	velocity.A = joints_[1].dq + joints_[2].dq + joints_[3].dq;

	velocity.z = (joints_[1].dq) * cos(joints_[1].q) * (joints_[1].a)
			+ (joints_[1].dq + joints_[2].dq) * cos(joints_[1].q + joints_[2].q) * (joints_[2].a)
			+ (joints_[1].dq + joints_[2].dq + joints_[3].dq) * cos(joints_[1].q + joints_[2].q + joints_[3].q)
					* (joints_[3].a + joints_[4].d);

	float r = cos(joints_[1].q) * (joints_[1].a) + cos(joints_[1].q + joints_[2].q) * (joints_[2].a)
			+ cos(joints_[1].q + joints_[2].q + joints_[3].q) * (joints_[3].a + joints_[4].d) + joints_[0].a;

	float dr = -sin(joints_[1].q) * (joints_[1].dq) * (joints_[1].a)
			+ -sin(joints_[1].q + joints_[2].q) * (joints_[1].dq + joints_[2].dq) * (joints_[2].a)
			+ -sin(joints_[1].q + joints_[2].q + joints_[3].q) * (joints_[1].dq + joints_[2].dq + joints_[3].dq)
					* (joints_[3].a + joints_[4].d);

	velocity.x = -sin(joints_[0].q) * joints_[0].dq * r + cos(joints_[0].q) * dr;
	velocity.y = cos(joints_[0].q) * joints_[0].dq * r + sin(joints_[0].q) * dr;

	return 0;
}

kin_conf Kinematics::get_Current_Config(const std::vector<float>& q_in) {
	kin_conf conf;

	for (int i = 0; i < 5; i++) {
		joints_[i].form_Real(q_in[i]);
	}

	float r = cos(joints_[1].q) * (joints_[1].a) + cos(joints_[1].q + joints_[2].q) * (joints_[2].a)
			+ cos(joints_[1].q + joints_[2].q + joints_[3].q) * (joints_[3].a + joints_[4].d) + joints_[0].a;
	if (r > 0)
		conf.a = true;
	else
		conf.a = false;

	if (joints_[2].q < 0)
		if (conf.a) {
			conf.b = true;
		} else {
			conf.b = false;
		}

	else {
		if (conf.a) {
			conf.b = false;
		} else {
			conf.b = true;
		}
	}

	return conf;
}

int Kinematics::ikin_pos(const cartesian& position, const kin_conf& config, std::vector<float>& q_out) {

	float r, C;
	float r_, z_;
	q_out.resize(5, 0);

	r = sqrt(position.x * position.x + position.y * position.y);
	joints_[0].q = atan2(position.y, position.x);

	if (config.a) {
		r_ = r - cos(position.A) * joints_[4].d - joints_[0].a;
	} else {

		if (joints_[0].q >= 0) {
			joints_[0].q -= M_PI;
		} else {
			joints_[0].q += M_PI;
		}

		r_ = -r - cos(position.A) * joints_[4].d - joints_[0].a;
	}

	q_out[0] = joints_[0].get_Real_Pos();

	if (!joints_[0].check_Limit()) {
		return KIN_JOINT_LIMITS;
	}

	z_ = position.z - sin(position.A) * joints_[4].d - joints_[0].d;

	//std::cout << std::flush <<" z_:  " << z_ << " | r_: " << r_ << "      " << std::endl;

	float U = pow(joints_[1].a + joints_[2].a, 2) - (pow(r_, 2) + pow(z_, 2));
	float L = (pow(r_, 2) + pow(z_, 2)) - pow(joints_[1].a - joints_[2].a, 2);

	if (L == 0) {
		return KIN_SINGULARITY;
	}

	if (U / L < 0) {
		return KIN_OUT_OF_REACH;
	}

	C = sqrt(U / L);

	if (config.b == config.a)
		joints_[2].q = -2.0 * atan(C);
	else
		joints_[2].q = +2.0 * atan(C);

	float K = joints_[2].a * sin(joints_[2].q);
	float M = joints_[1].a + joints_[2].a * cos(joints_[2].q);

	joints_[1].q = atan2(z_, r_) - atan2(K, M);

	joints_[3].q = position.A - joints_[2].q - joints_[1].q;

	joints_[4].q = position.B;

	q_out.resize(5, 0);

	for (int i = 0; i < 5; i++) {
		if (joints_[i].check_Limit())
			q_out[i] = joints_[i].get_Real_Pos();
		else
			return KIN_JOINT_LIMITS;
	}

	return KIN_OK;

}

int Kinematics::ikin_vel(const cartesian &position, const cartesian &velocity, const kin_conf &config,
		std::vector<float> &q_out, std::vector<float> &dq_out) {

	float r, dr, C, dC;
	float r_, dr_, z_, dz_;
	q_out.resize(5, 0);
	dq_out.resize(5, 0);

	r = sqrt(position.x * position.x + position.y * position.y);

	if (r != 0)
		dr = (position.x * velocity.x + position.y * velocity.y) / r;
	else
		dr = 0;

	joints_[0].q = atan2(position.y, position.x);
	joints_[0].dq = (position.x * velocity.y - position.y * velocity.x)
			/ (position.x * position.x + position.y * position.y);

	if (config.a) {

		r_ = r - cos(position.A) * joints_[4].d - joints_[0].a;
		dr_ = dr + sin(position.A) * velocity.A * joints_[4].d;
	} else {

		if (joints_[0].q >= 0) {
			joints_[0].q -= M_PI;
		} else {
			joints_[0].q += M_PI;
		}

		r_ = -r - cos(position.A) * joints_[4].d - joints_[0].a;
		dr_ = -dr + sin(position.A) * velocity.A * joints_[4].d;
	}

	z_ = position.z - sin(position.A) * joints_[4].d - joints_[0].d;
	dz_ = velocity.z - cos(position.A) * velocity.A * joints_[4].d;

	float U = pow(joints_[1].a + joints_[2].a, 2) - (pow(r_, 2) + pow(z_, 2));
	float L = (pow(r_, 2) + pow(z_, 2)) - pow(joints_[1].a - joints_[2].a, 2);

	float dL = 2 * (r_ * dr_ + z_ * dz_);


	if (L == 0) {
		return KIN_SINGULARITY;
	}

	if (U / L < 0) {
		return KIN_OUT_OF_REACH;
	}

	C = sqrt(U / L);
	dC = -  dL *(L+U) / ( 2.0 * C * L * L);

	if (config.b == config.a) {
		joints_[2].q = -2.0 * atan(C);
		joints_[2].dq = -2.0 * (dC / (1 + C * C));
	} else {
		joints_[2].q = +2.0 * atan(C);
		joints_[2].dq = +2.0 * (dC / (1 + C * C));
	}

	float K = joints_[2].a * sin(joints_[2].q);
	float M = joints_[1].a + joints_[2].a * cos(joints_[2].q);

	float dK = joints_[2].a * cos(joints_[2].q) * joints_[2].dq;
	float dM = -joints_[2].a * sin(joints_[2].q) * joints_[2].dq;

	joints_[1].q = atan2(z_, r_) - atan2(K, M);

	joints_[1].dq = (r_ * dz_ - dr_ * z_) / (r_ * r_ + z_ * z_) - (M * dK - dM * K) / (M * M + K * K);

	joints_[3].q = position.A - joints_[2].q - joints_[1].q;
	joints_[3].dq = velocity.A - joints_[2].dq - joints_[1].dq;

	joints_[4].q = position.B;

	joints_[4].dq = velocity.B;

	for (int i = 0; i < 5; i++) {
		if (joints_[i].check_Limit()) {
			q_out[i] = joints_[i].get_Real_Pos();
			dq_out[i] = joints_[i].get_Real_Vel();
		}

		else
			return KIN_JOINT_LIMITS;
	}

	return KIN_OK;

}

