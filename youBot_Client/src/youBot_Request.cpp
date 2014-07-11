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

#include "youBot_Request.hpp"

youBot_Request::youBot_Request() {

	error_=0;
	msgid_=0;
	priority_ =0;


	type_  = 2;
	data_type_ = "YOUBOT::REQUEST";

	complex_msg_type_ = -1;
	state_req_ =0;
	joint_num_.clear();
	joint_type_.clear();
	joint_value_.clear();
	base_num_.clear();
	base_type_.clear();
	base_value_.clear();
	gripper_num_.clear();
	gripper_type_.clear();
	gripper_value_.clear();
	complex_msg_value_.clear();

}

youBot_Request::~youBot_Request() {

}

int youBot_Request::get_State_Req() {
	return state_req_;
}

void youBot_Request::set_State_Req(int state_request) {
	state_req_ = state_request;
}

void youBot_Request::add_State_Req(int additional_state_request) {
	state_req_ |= additional_state_request;
}

int youBot_Request::get_Number_Of_Joint_Msg() {
	return joint_num_.size();
}

int youBot_Request::get_Number_Of_Base_Msg() {
	return base_num_.size();
}

int youBot_Request::get_Number_Of_Gripper_Msg() {
	return gripper_num_.size();
}

Joint_Message youBot_Request::get_Joint_Msg(int num) {
	Joint_Message jm;
	jm.joint = joint_num_[num];
	jm.type = joint_type_[num];
	jm.value = joint_value_[num];
	return jm;
}

Base_Message youBot_Request::get_Base_Msg(int num) {
	Base_Message bm;
	bm.num = base_num_[num];
	bm.type = base_type_[num];
	bm.value = base_value_[num];
	return bm;
}

Gripper_Message youBot_Request::get_Gripper_Msg(int num) {
	Gripper_Message gm;
	gm.num = gripper_num_[num];
	gm.type = gripper_type_[num];
	gm.value = gripper_value_[num];
	return gm;
}

void youBot_Request::add_Joint_Msg(Joint_Message joint_Message) {
	joint_num_.push_back(joint_Message.joint);
	joint_type_.push_back(joint_Message.type);
	joint_value_.push_back(joint_Message.value);
	add_State_Req(ARM_CONTROL);
}

void youBot_Request::add_Joint_Msg(int joint, int type, float value) {
	joint_num_.push_back(joint);
	joint_type_.push_back(type);
	joint_value_.push_back(value);
	add_State_Req(ARM_CONTROL);
}

void youBot_Request::add_Base_Msg(Base_Message base_Message) {
	base_num_.push_back(base_Message.num);
	base_type_.push_back(base_Message.type);
	base_value_.push_back(base_Message.value);
	add_State_Req(BASE_CONTROL);
}

void youBot_Request::add_Base_Msg(int num, int type, float value_0,
		float value_1, float value_2) {
	base_num_.push_back(num);
	base_type_.push_back(type);
	std::vector<float> vel;
	vel.push_back(value_0);
	vel.push_back(value_1);
	vel.push_back(value_2);
	base_value_.push_back(vel);
	add_State_Req(BASE_CONTROL);
}

void youBot_Request::add_Gripper_Msg(Gripper_Message gripper_Message) {
	gripper_num_.push_back(gripper_Message.num);
	gripper_type_.push_back(gripper_Message.type);
	gripper_value_.push_back(gripper_Message.value);
	add_State_Req(ARM_CONTROL);
}

void youBot_Request::add_Gripper_Msg(int num, int type, float value) {
	gripper_num_.push_back(num);
	gripper_type_.push_back(type);
	gripper_value_.push_back(value);
	add_State_Req(ARM_CONTROL);
}

void youBot_Request::pack_Data() {

	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> pk(&sbuf);

	pk.pack(state_req_);
	pk.pack(joint_num_);
	pk.pack(joint_type_);
	pk.pack(joint_value_);
	pk.pack(base_num_);
	pk.pack(base_type_);
	pk.pack(base_value_);
	pk.pack(gripper_num_);
	pk.pack(gripper_type_);
	pk.pack(gripper_value_);
	pk.pack(complex_msg_type_);
	pk.pack(complex_msg_value_);

	data_.resize(sbuf.size());
	memcpy((void*)data_.data(),sbuf.data(), sbuf.size());

}

void youBot_Request::unpack_Data() {
	msgpack::unpacker pac;
    pac.reserve_buffer(data_.size());
    memcpy(pac.buffer(), data_.data(), data_.size());
    pac.buffer_consumed(data_.size());

    msgpack::unpacked result;

    pac.next(&result);result.get().convert(&state_req_);
    pac.next(&result);result.get().convert(&joint_num_);
    pac.next(&result);result.get().convert(&joint_type_);
    pac.next(&result);result.get().convert(&joint_value_);
    pac.next(&result);result.get().convert(&base_num_);
    pac.next(&result);result.get().convert(&base_type_);
    pac.next(&result);result.get().convert(&base_value_);
    pac.next(&result);result.get().convert(&gripper_num_);
    pac.next(&result);result.get().convert(&gripper_type_);
    pac.next(&result);result.get().convert(&gripper_value_);
    pac.next(&result);result.get().convert(&complex_msg_type_);
    pac.next(&result);result.get().convert(&complex_msg_value_);

    data_.clear();
}

void youBot_Request::add_Complex_Msg(int type,
		std::vector<float> value) {
	complex_msg_type_ = type;
	complex_msg_value_ = value;
	add_State_Req(ARM_CONTROL);
}

int youBot_Request::get_Complex_Type() {
	return complex_msg_type_;
}

std::vector<float> youBot_Request::get_Complex_Values() {
	return complex_msg_value_;
}

void youBot_Request::clear() {
	error_=0;
	msgid_=0;
	priority_ =0;


	type_  = 2;
	data_type_ = "YOUBOT::REQUEST";

	complex_msg_type_ = -1;
	state_req_ =0;
	joint_num_.clear();
	joint_type_.clear();
	joint_value_.clear();
	base_num_.clear();
	base_type_.clear();
	base_value_.clear();
	gripper_num_.clear();
	gripper_type_.clear();
	gripper_value_.clear();
	complex_msg_value_.clear();
}

std::string youBot_Request::toString() {
	std::stringstream ss;
	ss << "Head: " << get_State_Req() << "\n";
	for(int i=0;i<get_Number_Of_Joint_Msg();i++){
		ss << get_Joint_Msg(i).toString() << "\n";
	}
	for(int i=0;i<get_Number_Of_Base_Msg();i++){
		ss << get_Base_Msg(i).toString() << "\n";
	}
	for(int i=0;i<get_Number_Of_Gripper_Msg();i++){
		ss << get_Gripper_Msg(i).toString() << "\n";
	}
	if(complex_msg_type_ >=0){
		ss << "Complex Type: " << complex_msg_type_ << " Values:";
		for(unsigned int i=0;i<complex_msg_value_.size()-1;i++)
		{
			ss << complex_msg_value_[i] << " | ";
		}
		ss << complex_msg_value_[complex_msg_value_.size()-1] << "\n";
	}
	return ss.str();
}
