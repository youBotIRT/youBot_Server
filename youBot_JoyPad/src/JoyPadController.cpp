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

#include "JoyPadController.hpp"

JoyPadController::JoyPadController(std::string adr) {
	server_adr_ = adr;
	mode_ = MODE_NORMAL;
	lin_vel_rot_ = 0.4;
	lin_vel_trans_ = 80;
	arm_vel_ = 0.5;
	base_rot_vel_ = 0.6;
	base_trans_vel_ = 0.3;
	gipper_state_ = false;
	admin_mode_ = false;
	js_ok_ = false;
	js_check_counter_ = 1;
	speed_ = 1;
	js_path = "/dev/input/js0";
}

JoyPadController::~JoyPadController() {

}

bool JoyPadController::start_Joystick() {

	while (!js.is_ready()) {
		js.load(js_path.c_str());
		if (!js.is_ready()) {
			std::cout << "No Joypad Found.. Retrying.." << std::endl;
			usleep(1000 * 1000);
		}
	}
	js_ok_ = true;
	return true;
}

bool JoyPadController::reconntect_Joystick() {
	std::cout << "Reconnecting to Joypad" << std::endl;
	js.reload(js_path.c_str());
	if (js.is_ready()) {
		js_ok_ = true;
		std::cout << "Reconnecting Succesfull" << std::endl;
		return true;
	}
	return false;
}

void JoyPadController::start_Comm() {
	init("YOUBOT::JOYPAD", 1, &request, &reply);
	dispatch_Communication(server_adr_, -1);
}

bool JoyPadController::check_Joystick() {
	if (js_check_counter_ % 100 == 0) {
		js_check_counter_ = 1;
		struct stat buffer;
		return (stat(js_path.c_str(), &buffer) == 0);
	}
	js_check_counter_++;
	return true;
}

void JoyPadController::pre_Comm() {
	js_event event;

	bool was_event = false;
	while (!was_event) {
		js_ok_ &= check_Joystick();
		if (!js_ok_) {
			request.add_State_Req(STOP);
			request.add_State_Req(BASE_CONTROL);
			request.add_State_Req(ARM_CONTROL);
			reconntect_Joystick();
			was_event = true;
		} else if (js.get_event(&event)) {

			process_JS_Event(event);

			control(event);
			if (!admin_mode_) {
				base(event);
				arm(event);
			} else
				admin(event);

			if (request.get_State_Req() != 0)
				was_event = true;

		} else {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}

	}

}

void JoyPadController::post_Comm() {

}

void JoyPadController::process_JS_Event(js_event e) {
	if (e.type == JS_EVENT_BUTTON) {
		button[e.number] = e.value;
	} else if (e.type == JS_EVENT_AXIS) {
		axis[e.number] = ((float) e.value) / -32768.0f;
	}
}

void JoyPadController::control(js_event e) {
	if (e.type == JS_EVENT_BUTTON && e.number == B_BACK) {
		if (e.value == 1) {
			admin_mode_ = true;
			request.add_State_Req(STOP);
			request.add_State_Req(BASE_CONTROL);
			request.add_State_Req(ARM_CONTROL);
		} else {
			admin_mode_ = false;
		}
	}

	if (e.type == JS_EVENT_BUTTON && e.number == B_LS && e.value == 1) {
		if (speed_ > 0.1)
			speed_ -= 0.1;
		std::cout << "Decreased Speed to: " << speed_ << std::endl;
	}
	if (e.type == JS_EVENT_BUTTON && e.number == B_RS && e.value == 1) {
		if (speed_ < 2)
			speed_ += 0.1;
		std::cout << "Increased Speed to: " << speed_ << std::endl;
	}
}

void JoyPadController::base(js_event e) {
	if (e.type == JS_EVENT_AXIS) {
		if (e.number == A_L_X || e.number == A_L_Y || e.number == A_R_X) {
			request.add_Base_Msg(0, 1, base_trans_vel_ * speed_ * axis[A_L_Y], base_trans_vel_ * speed_ * axis[A_L_X],
					base_rot_vel_ * speed_ * axis[A_R_X]);
		}
	}
}

void JoyPadController::arm(js_event e) {

	if (e.type == JS_EVENT_BUTTON && e.number == B_START && e.value == 1) {
		if (gipper_state_)
			request.add_Gripper_Msg(0, 0, 0.0022);
		else
			request.add_Gripper_Msg(0, 0, 0.0);

		gipper_state_ = !gipper_state_;
	}

	if (mode_ == MODE_NORMAL) {
		if (e.type == JS_EVENT_AXIS) {
			if (e.number == A_C_X) {
				request.add_Joint_Msg(0, 1, -1.0 * arm_vel_ * speed_ * axis[A_C_X]);
			}
			if (e.number == A_C_Y) {
				request.add_Joint_Msg(1, 1, arm_vel_ * speed_ * axis[A_C_Y]);
			}
		}
		if (e.type == JS_EVENT_BUTTON) {

			if (e.number == B_Y && e.value == 1) { //Canle
				set_PTP(0, -3.14 / 2.0, 0, -3.14 / 2.0, 0);
			}
			if (e.number == B_A && e.value == 1) { //EMBRYO
				set_PTP(-2.9, -2.7, 2.53, -1.75 - 3.14 / 2.0, -2.82);
			}

			if (e.number == B_LT || e.number == B_LB) { //Joint 3
				request.add_Joint_Msg(2, 1, arm_vel_ * speed_ * (button[B_LT] - button[B_LB]));
			}

			if (e.number == B_RT || e.number == B_RB) { //Joint 4
				request.add_Joint_Msg(3, 1, arm_vel_ * speed_ * (button[B_RT] - button[B_RB]));
			}

			if (e.number == B_X || e.number == B_B) { //Joint 5
				request.add_Joint_Msg(4, 1, arm_vel_ * speed_ * (button[B_X] - button[B_B]));
			}

		}
	}

	if (mode_ == MODE_CARTESIAN) {
		if ((e.type == JS_EVENT_AXIS && (e.number == A_C_X || e.number == A_C_Y))
				|| ((e.type == JS_EVENT_BUTTON)
						&& (e.number == B_LB || e.number == B_RB || e.number == B_A || e.number == B_B
								|| e.number == B_X || e.number == B_Y))) {
			set_Linear(lin_vel_trans_ * axis[A_C_Y], lin_vel_trans_ * axis[A_C_X],
					lin_vel_trans_ * (button[B_LB] - button[B_RB]), lin_vel_rot_ * (button[B_A] - button[B_Y]),
					lin_vel_rot_ * (button[B_B] - button[B_X]));
		}
	}

}

void JoyPadController::set_PTP(float j1, float j2, float j3, float j4, float j5) {
	std::vector<float> x;
	x.push_back(1);
	x.push_back(j1);
	x.push_back(j2);
	x.push_back(j3);
	x.push_back(j4);
	x.push_back(j5);
	x.push_back(0.2);

	request.add_Complex_Msg(1, x);
}

void JoyPadController::admin(js_event e) {
	if (e.type == JS_EVENT_BUTTON) {
		if (e.number == B_START && e.value == 1) {
			request.add_State_Req(ARM_CONTROL);
			request.add_State_Req(BASE_CONTROL);
			request.add_State_Req(FREE);
			std::cout << "FREE" << std::endl;
		}
		if (e.number == B_A && e.value == 1) {
			mode_ = MODE_NORMAL;
			std::cout << "NORMAL MODE" << std::endl;
		}
		if (e.number == B_Y && e.value == 1) {
			mode_ = MODE_CARTESIAN;
			std::cout << "CART MODE" << std::endl;
		}

		if (e.number == B_B && e.value == 1) {
			request.add_State_Req(ARM_CONTROL);
			request.add_State_Req(BASE_CONTROL);
			request.add_State_Req(RESET);
			std::cout << "RESET" << std::endl;
		}
	}
}

void JoyPadController::set_Linear(float x, float y, float z, float A, float B) {
	std::vector<float> a;
	a.push_back( speed_ *x);
	a.push_back( speed_ *y);
	a.push_back( speed_ *z);
	a.push_back( speed_ * A *0.3);
	a.push_back( speed_ * B);
	request.add_Complex_Msg(2, a);
}
