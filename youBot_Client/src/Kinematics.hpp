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

#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

#ifndef KINEMATICS_HPP_
#define KINEMATICS_HPP_


#define KIN_OK								0x00000000
#define KIN_OUT_OF_REACH					0x01000000
#define KIN_JOINT_LIMITS					0x02000000
#define KIN_SINGULARITY						0x04000000
#define KIN_VELOCITY_LIMIT					0x08000000

struct kin_conf{
	bool a; // Joint 0
	bool b; // Elbow Up/Down
	std::string toString(){
		std::stringstream ss;
		ss << "Config: ";

		if(a)
			 ss << " Normal | ";
		else
			ss << " Overhead | ";

		if(b)
			ss << "Elbow Up";
		else
			ss << "Elbow Down";

		return ss.str();
	}
};



struct cartesian{
	float x,y,z;
	float A,B;

	void form_Vecotor(std::vector<float> v){
		if(v.size()>5)
		{
			this->x = v[0];
			this->y = v[1];
			this->z = v[2];
			this->A = v[3];
			this->B = v[4];

		}

	}


	std::string toString(){
		std::stringstream ss;
		ss << "X: " << std::setw(10) << x << " Y: " << std::setw(10) << y << " Z: " << std::setw(10) << z << " A: " <<std::setw(10) <<  A << " B: " << std::setw(10) << B;
		return ss.str();
	}

	void add(cartesian step){
		this->x += step.x;
		this->y += step.y;
		this->z += step.z;
		this->A += step.A;
		this->B += step.B;
	};

	void sub(cartesian step){
		this->x -= step.x;
		this->y -= step.y;
		this->z -= step.z;
		this->A -= step.A;
		this->B -= step.B;
	};

	void Mult(float a){
		this->x *= a;
		this->y *= a;
		this->z *= a;
		this->A *= a;
		this->B *= a;
	};

	void step(cartesian vel, float step_s){
		this->x += vel.x * step_s;
		this->y += vel.y * step_s;
		this->z += vel.z * step_s;
		this->A += vel.A * step_s;
		this->B += vel.B * step_s;
	}

	void distance(cartesian A,cartesian B){
		this->x = A.x - B.x;
		this->y = A.y - B.y;
		this->z = A.z - B.z;
		this->A = A.A - B.A;
		this->B = A.B - B.B;
	};

};

struct carteisan_6{
	float x,y,z;
	float Y,P,R;
	float HT[4][4];

	void form_Cart5(cartesian cart){
		this->x =  cart.x;
		this->y =  cart.y;
		this->z =  cart.z;
		this->Y =  atan2(cart.y,cart.x);
		this->P =  cart.A;
		this->R =  cart.B;
		to_HT();
	}
	void to_HT(){
		HT[0][0] = cos(P)*cos(Y);

		HT[0][1] = sin(R)*sin(P)*cos(Y)-cos(R)*sin(Y);
		HT[0][2] = cos(R)*sin(P)*cos(Y)+sin(R)*sin(Y);
		HT[0][3] = 0;

		HT[1][0] = cos(P)*sin(Y);
		HT[1][1] = sin(R)*sin(P)*sin(Y)+cos(R)*cos(Y);
		HT[1][2] = cos(R)*sin(P)*sin(Y)-sin(R)*cos(Y);
		HT[1][3] = 0;

		HT[2][0] = -sin(P);
		HT[2][1] = sin(R)*cos(P);
		HT[2][2] = cos(R)*cos(P);
		HT[2][3] = 0;

		HT[3][0] = x;
		HT[3][1] = y;
		HT[3][2] = z;
		HT[3][3] = 1;

	}

};

struct joint_parameter{
	float q,dq;
	float dphi,alpha,a,d;
	float q_max,q_min,direction;

	void form_Real(float real_pos){
		q = (real_pos*direction)- dphi;
	}

	void form_Real(float real_pos,float real_vel){
		form_Real(real_pos);
		dq = real_vel * direction;

	}

	float get_Real_Pos(){
		return (q + dphi) * direction;
	}

	float get_Real_Vel(){
		return (dq) * direction;
	}

	bool check_Limit(){
		if(get_Real_Pos()>q_max)
			return false;
		if(get_Real_Pos()<q_min)
			return false;
		return true;
	}
};


class Kinematics {
private:
	joint_parameter joints_[5];

public:
	Kinematics();
	virtual ~Kinematics();

	int fkin_pos(const std::vector<float> &q_in, cartesian &position);
	int fkin_vel(const std::vector<float> &q_in, const std::vector<float> &dq, cartesian &velocity);

	kin_conf get_Current_Config(const std::vector<float> &q_in);

	int ikin_pos(const cartesian &position, const kin_conf &config, std::vector<float> &q_out);
	int ikin_vel(const cartesian &position,const cartesian &velocity, const kin_conf &config, std::vector<float> &q_out, std::vector<float> &dq_out);


};

#endif /* KINEMATICS_HPP_ */
