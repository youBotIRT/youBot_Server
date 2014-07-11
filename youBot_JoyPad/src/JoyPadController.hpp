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

#ifndef JOYPADCONTROLLER_HPP_
#define JOYPADCONTROLLER_HPP_

#include "Client.hpp"
#include "Joystick.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define B_X 0
#define B_A 1
#define B_B 2
#define B_Y 3
#define B_LB 4
#define B_RB 5
#define B_LT 6
#define B_RT 7
#define B_LS  10
#define B_RS 11
#define B_START 9
#define B_BACK 8

#define A_C_X 4
#define A_C_Y 5
#define A_L_X 0
#define A_L_Y 1
#define A_R_X 2
#define A_R_Y 3

#define MODE_NORMAL 0
#define MODE_CARTESIAN 1


class JoyPadController : public Client{
private:

	std::string server_adr_;
	std::string js_path;

	Joystick js;
	youBot_Reply reply;
	youBot_Request request;
	bool js_ok_;
	int js_check_counter_;

	int button[16];
	float axis[6];

	float base_trans_vel_;
	float base_rot_vel_;
	float arm_vel_;
	float lin_vel_rot_;
	float lin_vel_trans_;
	int mode_;
	bool admin_mode_;

	bool gipper_state_;


	float speed_;

public:
	JoyPadController(std::string adr);
	virtual ~JoyPadController();


	bool start_Joystick();
	bool reconntect_Joystick();
	bool check_Joystick();

	void start_Comm();


	void pre_Comm();
	void post_Comm();
	void process_JS_Event(js_event e);

	void control(js_event e);
	void base(js_event e);
	void arm(js_event e);
	void admin(js_event e);

	void set_PTP(float j1,float j2,float j3,float j4,float j5);
	void set_Linear(float x, float y, float z, float A, float B);
};

#endif /* JOYPADCONTROLLER_HPP_ */
