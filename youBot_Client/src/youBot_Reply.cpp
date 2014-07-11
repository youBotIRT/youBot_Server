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

#include "youBot_Reply.hpp"



youBot_Reply::youBot_Reply() {
	error_=0;
	msgid_=0;
	priority_ =0;
	type_  = 1;
	data_type_ = "YOUBOT::REPLY";

	active_arm_prio_ = 0;
	active_base_prio_ = 0;
	state_ = 0;
	timestamp_ = 0;
	active_arm_id_ = "INIT";
	active_base_id_ = "INIT";

	joint_num_.clear();
	joint_pos_.clear();
	joint_vel_.clear();
	joint_cur_.clear();
	joint_status_.clear();

	base_num_.clear();
	base_pos_.clear();
	base_vel_.clear();
	base_status_.clear();

	gripper_num_.clear();
	gripper_position_.clear();

}

youBot_Reply::~youBot_Reply() {
}

std::string youBot_Reply::get_Active_Arm_Id() {
	return active_arm_id_;
}

std::string youBot_Reply::get_Active_Base_Id() {
	return active_base_id_;
}

int youBot_Reply::get_Active_Arm_Priority() {
	return active_arm_prio_;
}

int youBot_Reply::get_Active_Base_Priority() {
	return active_base_prio_;
}

void youBot_Reply::set_Active_Id(const std::string& arm,
		const std::string& base) {
	active_arm_id_ = arm;
	active_base_id_ = base;
}

void youBot_Reply::set_Active_Priority(const int& arm, const int& base) {
	active_arm_prio_ = arm;
	active_base_prio_ = base;
}

int youBot_Reply::get_State() {
	return state_;
}

unsigned long long youBot_Reply::get_Timestamp() {
	return timestamp_;
}

void youBot_Reply::set_Timestamp(unsigned long long timestamp) {
	timestamp_ = timestamp;
}

void youBot_Reply::add_State(int additional_state) {
	state_ |= additional_state;
}

void youBot_Reply::set_State(int state) {
	state_ = state;
}

int youBot_Reply::get_Number_Of_Joint_Msg() {
	return joint_num_.size();
}

int youBot_Reply::get_Number_Of_Base_Msg() {
	return base_num_.size();
}

int youBot_Reply::get_Number_Of_Gripper_Msg() {
	return gripper_num_.size();
}

Joint_Information youBot_Reply::get_Joint_Info(int num) {
	Joint_Information ji;
	ji.joint = joint_num_[num];
	ji.position = joint_pos_[num];
	ji.current = joint_cur_[num];
	ji.velocity = joint_vel_[num];
	ji.status = joint_status_[num];
	return ji;
}

Base_Information youBot_Reply::get_Base_Info(int num) {
	Base_Information bi;
	bi.num = base_num_[num];
	bi.position = base_pos_[num];
	bi.velocity = base_vel_[num];
	bi.status = base_status_[num];

	return bi;
}

Gripper_Information youBot_Reply::get_Gripper_Info(int num) {
	Gripper_Information gi;
	gi.num = gripper_num_[num];
	gi.position = gripper_position_[num];
	return gi;
}

void youBot_Reply::add_Joint_Info(Joint_Information info) {
	joint_num_.push_back(info.joint);
	joint_pos_.push_back(info.position);
	joint_vel_.push_back(info.velocity);
	joint_cur_.push_back(info.current);
	joint_status_.push_back(info.status);
}

void youBot_Reply::add_Base_Info(Base_Information info) {
	base_num_.push_back(info.num);
	base_pos_.push_back(info.position);
	base_vel_.push_back(info.velocity);
	base_status_.push_back(info.status);
}

void youBot_Reply::add_Gripper_Info(Gripper_Information info) {
	gripper_num_.push_back(info.num);
	gripper_position_.push_back(info.position);
}

void youBot_Reply::unpack_Data() {
	msgpack::unpacker pac;
	pac.reserve_buffer(data_.size());
	memcpy(pac.buffer(), data_.data(), data_.size());
	pac.buffer_consumed(data_.size());
	msgpack::unpacked result;
	pac.next(&result);
	result.get().convert(&active_arm_id_);
	pac.next(&result);
	result.get().convert(&active_base_id_);
	pac.next(&result);
	result.get().convert(&state_);
	pac.next(&result);
	result.get().convert(&timestamp_);
	pac.next(&result);
	result.get().convert(&active_arm_prio_);
	pac.next(&result);
	result.get().convert(&active_base_prio_);
	pac.next(&result);
	result.get().convert(&joint_num_);
	pac.next(&result);
	result.get().convert(&joint_pos_);
	pac.next(&result);
	result.get().convert(&joint_vel_);
	pac.next(&result);
	result.get().convert(&joint_cur_);
	pac.next(&result);
	result.get().convert(&joint_status_);
	pac.next(&result);
	result.get().convert(&base_num_);
	pac.next(&result);
	result.get().convert(&base_pos_);
	pac.next(&result);
	result.get().convert(&base_vel_);
	pac.next(&result);
	result.get().convert(&base_status_);
	pac.next(&result);
	result.get().convert(&gripper_num_);
	pac.next(&result);
	result.get().convert(&gripper_position_);
	data_.clear();
}

void youBot_Reply::pack_Data(){
	msgpack::sbuffer sbuf;
	msgpack::packer<msgpack::sbuffer> pk(&sbuf);
	pk.pack(active_arm_id_);
	pk.pack(active_base_id_);
	pk.pack(state_);
	pk.pack(timestamp_);
	pk.pack(active_arm_prio_);
	pk.pack(active_base_prio_);
	pk.pack(joint_num_);
	pk.pack(joint_pos_);
	pk.pack(joint_vel_);
	pk.pack(joint_cur_);
	pk.pack(joint_status_);
	pk.pack(base_num_);
	pk.pack(base_pos_);
	pk.pack(base_vel_);
	pk.pack(base_status_);
	pk.pack(gripper_num_);
	pk.pack(gripper_position_);
	data_.resize(sbuf.size());
	memcpy((void*) data_.data(), sbuf.data(), sbuf.size());
}

void youBot_Reply::clear() {
	error_=0;
	msgid_=0;
	priority_ =0;
	type_  = 1;
	data_type_ = "YOUBOT::REPLY";

	active_arm_prio_ = 0;
	active_base_prio_ = 0;
	state_ = 0;
	timestamp_ = 0;
	active_arm_id_ = "INIT";
	active_base_id_ = "INIT";

	joint_num_.clear();
	joint_pos_.clear();
	joint_vel_.clear();
	joint_cur_.clear();
	joint_status_.clear();

	base_num_.clear();
	base_pos_.clear();
	base_vel_.clear();
	base_status_.clear();

	gripper_num_.clear();
	gripper_position_.clear();
}
