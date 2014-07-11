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

#ifndef YOUBOTREQUEST_H_
#define YOUBOTREQUEST_H_

#include <iostream>
#include <unistd.h>
#include "zmq.hpp"
#include "msgpack.hpp"
#include "Message.hpp"
#include <vector>
#include <string>
#include <sstream>

#include "youBot_IDs.hpp"

struct Joint_Message {
	int joint;
	int type;
	float value;

	std::string toString(){
		std::stringstream ss;
		ss << "Joint Messge J: " << joint;
		ss << " T: " << type;
		ss << " value " << value;
		return ss.str();
	}

};

struct Base_Message {
	int num;
	int type;
	std::vector<float> value;

	std::string toString(){
		std::stringstream ss;
		ss << "Base Messge N: " << num;
		ss << " T: " << type;
		ss << " V " << value[0]<<" | " << value[1] <<" | "<< value[2];
		return ss.str();
	}
};

struct Gripper_Message {
	int num;
	int type;
	float value;

	std::string toString(){
		std::stringstream ss;
		ss << "Gripper Messge J: " << num;
		ss << " T: " << type;
		ss << " value " << value;
		return ss.str();
	}

};

class youBot_Request : public Message {
private:
	int state_req_;

	std::vector<int> joint_num_;
	std::vector<int> joint_type_;
	std::vector<float> joint_value_;

	std::vector<int> base_num_;
	std::vector<int> base_type_;
	std::vector<std::vector<float> > base_value_;

	std::vector<int> gripper_num_;
	std::vector<float> gripper_type_;
	std::vector<float> gripper_value_;

	int complex_msg_type_;
	std::vector<float> complex_msg_value_;

	void unpack_Data();
	void pack_Data();
public:
	youBot_Request();
	virtual ~youBot_Request();
	int get_State_Req();
	void set_State_Req(int state_request);
	void add_State_Req(int additional_state_request);
	int get_Number_Of_Joint_Msg();
	int get_Number_Of_Base_Msg();
	int get_Number_Of_Gripper_Msg();
	Joint_Message get_Joint_Msg(int num);
	Base_Message get_Base_Msg(int num);
	Gripper_Message get_Gripper_Msg(int num);
	void add_Joint_Msg(Joint_Message joint_Message);
	void add_Joint_Msg(int joint, int type, float value);
	void add_Base_Msg(Base_Message base_Message);
	void add_Base_Msg(int num, int type, float value_0, float value_1,
			float value_2);
	void add_Gripper_Msg(Gripper_Message gripper_Message);
	void add_Gripper_Msg(int num, int type, float value);

	void add_Complex_Msg(int type,std::vector<float>  value);
	int get_Complex_Type();
	std::vector<float> get_Complex_Values();

	std::string toString();

	void clear();
};

#endif /* YOUBOTREQUEST_H_ */
