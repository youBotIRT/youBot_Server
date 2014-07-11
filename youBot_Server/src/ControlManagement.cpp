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

#include "ControlManagement.hpp"

Control_Management::Control_Management() {

	active_arm_prio_ = -1;
	active_arm_id_ = "none";
	active_base_prio_ = -1;
	active_base_id_ = "none";

}

Control_Management::~Control_Management() {

}

int Control_Management::mangae(youBot_Request *req, youBot_Reply *rep) {

	if (req->get_State_Req() & FREE) {
		if (req->get_Priority() <= active_arm_prio_) {
			active_arm_prio_ = -1;
			active_arm_id_ = "none";
		}

		if (req->get_Priority() <= active_base_prio_) {
			active_base_prio_ = -1;
			active_base_id_ = "none";
		}
	} else {
		if (req->get_Priority() == active_arm_prio_) {
			rep->add_State(ARM_CONTROL);
		} else if (req->get_State_Req() & ARM_CONTROL) {
			if (req->get_Priority() <= active_arm_prio_ || active_arm_prio_ == -1) {
				active_arm_prio_ = req->get_Priority();
				active_arm_id_ = req->get_Sender_ID();
				rep->add_State(ARM_CONTROL);
			}
		}

		if (req->get_Priority() == active_base_prio_) {
			rep->add_State(BASE_CONTROL);
		} else if (req->get_State_Req() & BASE_CONTROL) {
			if (req->get_Priority() <= active_base_prio_ || active_base_prio_ == -1) {
				active_base_prio_ = req->get_Priority();
				active_base_id_ = req->get_Sender_ID();
				rep->add_State(BASE_CONTROL);
			}
		}
	}

	rep->set_Active_Id(active_arm_id_, active_base_id_);
	rep->set_Active_Priority(active_arm_prio_, active_base_prio_);
	return 0;

}

void Control_Management::print() {
	std::cout << "Arm: " << active_arm_id_ << " " << active_arm_prio_ << " | Base: " << active_base_id_ << " "
			<< active_base_prio_ << std::endl;
}

std::string Control_Management::get_Arm_ID() {
	return active_arm_id_;
}

std::string Control_Management::get_Base_ID() {
	return active_base_id_;
}

