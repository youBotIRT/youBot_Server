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


#include <string>
#include <iostream>
#include <iomanip>

#include "Client.hpp"
#include "Kinematics.hpp"

using namespace std;

class Monitor: public Client {

private:
	std::string server_adr_;
	youBot_Reply reply;
	youBot_Request request;
	Kinematics kin;
	int written_lines;
	float t;
public:
	void step_up() {
		for (int i = 0; i < written_lines; i++) {
			cout << "\e[A";
		}
		written_lines = 0;
	}

	Monitor(std::string adr) {
		server_adr_ = adr;
		init("YOUBOT::MONITOR", 999, &request, &reply);
		dispatch_Communication(server_adr_, 100);
		written_lines = 0;
		t=0;
	}

	void pre_Comm() {
		request.set_State_Req(0);
	}

	void new_line() {
		cout << endl;
		written_lines++;
	}
	void post_Comm() {
		cout << flush << "TimeStamp: " << std::dec << setw(13) << reply.get_Timestamp() << " us | State:  "
				<< std::showbase << std::hex << reply.get_State() << std::dec << "     ";
		new_line();
		cout << flush << "Arm  Control: " << reply.get_Active_Arm_Id() << " ID: " << reply.get_Active_Arm_Priority()
				<< "               ";
		new_line();
		cout << flush << "Base Control: " << reply.get_Active_Base_Id() << " ID: " << reply.get_Active_Base_Priority()
				<< "               ";
		new_line();
		Joint_Information j_info;

		if(reply.get_Number_Of_Joint_Msg()>0)
		{
			cout << "------Arm----- "; new_line();
		}

		for (int i = 0; i < reply.get_Number_Of_Joint_Msg(); i++) {
			j_info = reply.get_Joint_Info(i);
			cout << flush  << j_info.joint + 1;
			cout << " -> Position: " << setw(13) << j_info.position;
			cout << " Velocity: " << setw(13) << j_info.velocity;
			cout << " Current: " << setw(13) << j_info.current;
			cout << " Status: " << setw(8) << j_info.status << "      ";
			new_line();

		}


		if (reply.get_Number_Of_Joint_Msg() == 5) {
			cout << "------Cartesian Arm----- "; new_line();
			std::vector<float> q, dq, q_calc, dq_calc;
			for (int i = 0; i < 5; i++) {
				q.push_back(reply.get_Joint_Info(i).position);
				dq.push_back(reply.get_Joint_Info(i).velocity);
			}
			cartesian pos, vel;
			kin.fkin_pos(q, pos);
			kin.fkin_vel(q, dq, vel);

			cout << flush << "Position-> X: " << setw(13) << pos.x;
			cout << " Y:" << setw(13) << pos.y;
			cout << " Z:" << setw(13) << pos.z;
			cout << " A:" << setw(13) << pos.A;
			cout << " B:" << setw(13) << pos.B << "   ";
			new_line();

			cout << flush << "Velocity-> X: " << setw(13) << vel.x;
			cout << " Y:" << setw(13) << vel.y;
			cout << " Z:" << setw(13) << vel.z;
			cout << " A:" << setw(13) << vel.A;
			cout << " B:" << setw(13) << vel.B << "   ";
			new_line();
		}
		Base_Information b_info;
		b_info = reply.get_Base_Info(0);
		if (reply.get_Number_Of_Base_Msg() > 0) {
			cout << "------Base----- "; new_line();
			cout << flush << "Velocity: Long: " << setw(13) << b_info.velocity[0];
			cout << " Trans: " << setw(13) << b_info.velocity[1];
			cout << " Rot: " << setw(13) << b_info.velocity[2] << "  ";
			new_line();
			cout << flush << "Position: Long: " << setw(13) << b_info.position[0];
			cout << " Trans: " << setw(13) << b_info.position[1];
			cout << " Rot: " << setw(13) << b_info.position[2] << "  ";
			new_line();

		}

		if (reply.get_Number_Of_Gripper_Msg()) {
			cout << "------Gripper----- "; new_line();
			cout << flush << "Gripper Position : " << setw(13) << reply.get_Gripper_Info(0).position << "   ";
			new_line();
		}


		step_up();
	}

};

void step_up(int lines) {
	for (int i = 0; i < lines; i++) {
		cout << "\e[A";
	}
}

int main(int c, char **v) {

	string adr;
	if (c != 2) {
		adr = "tcp://localhost:5555";
	} else {
		adr = v[1];
		adr += ":5555";
	}

	Monitor mon(adr);
	mon.wait();

	return 0;

}
