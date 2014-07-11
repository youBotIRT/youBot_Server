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

#ifndef YOUBOTREPLY_H_
#define YOUBOTREPLY_H_

#include <iostream>
#include <unistd.h>
#include "zmq.hpp"
#include "msgpack.hpp"
#include "Message.hpp"
#include <vector>
#include <string>

#include "youBot_IDs.hpp"

struct Joint_Information {
	int joint;
	float position;
	float velocity;
	float current;
	unsigned int status;
};

struct Base_Information {
	int num;
	std::vector<float> position;
	std::vector<float> velocity;
	std::vector<unsigned int> status;
};

struct Gripper_Information {
	int num;
	float position;
};

class youBot_Reply : public Message {

private:
	int active_arm_prio_;
	int active_base_prio_;
	int state_;
	unsigned long long timestamp_;
	std::string active_arm_id_;
	std::string active_base_id_;

	std::vector<int> joint_num_;
	std::vector<float> joint_pos_;
	std::vector<float> joint_vel_;
	std::vector<float> joint_cur_;
	std::vector<unsigned int> joint_status_;

	std::vector<int> base_num_;
	std::vector<std::vector<float> > base_pos_;
	std::vector<std::vector<float> > base_vel_;
	std::vector<std::vector<unsigned int> > base_status_;

	std::vector<int> gripper_num_;
	std::vector<float> gripper_position_;

	void unpack_Data();
	void pack_Data();

public:
	youBot_Reply();
	virtual ~youBot_Reply();

	std::string get_Active_Arm_Id();
	std::string get_Active_Base_Id();

	int get_Active_Arm_Priority();
	int get_Active_Base_Priority();
	void set_Active_Id(const std::string &arm, const std::string &base);
	void set_Active_Priority(const int &arm, const int &base);
	int get_State();
	unsigned long long get_Timestamp();
	void set_Timestamp(unsigned long long timestamp);
	void add_State(int additional_state);
	void set_State(int state);
	int get_Number_Of_Joint_Msg();
	int get_Number_Of_Base_Msg();
	int get_Number_Of_Gripper_Msg();
	Joint_Information get_Joint_Info(int num);
	Base_Information get_Base_Info(int num);
	Gripper_Information get_Gripper_Info(int num);
	void add_Joint_Info(Joint_Information info);
	void add_Base_Info(Base_Information info);
	void add_Gripper_Info(Gripper_Information info);

	void clear();

};

#endif /* YOUBOTREPLY_H_ */
