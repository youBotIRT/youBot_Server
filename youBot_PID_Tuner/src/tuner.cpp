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
#include <vector>

#include "Client.hpp"
#include "Kinematics.hpp"

using namespace std;

class Tuner: public Client {

private:

	youBot_Reply reply;
	youBot_Request request;

	bool b;

public:

	Tuner() {
		init("YOUBOT::PID_TUNER", 5, &request, &reply);
		dispatch_Communication("tcp://localhost:5555", 250);
		b = true;

	}

	void pre_Comm() {
		if(b){
		request.set_State_Req(0);
		char buffer[50];
		cout << "Select Joint: (0 for all Joints)" << endl;
		cin >> buffer;
		int j = atoi(buffer);
		cout << "Enter new P Value: " << endl;
		cin >> buffer;
		float p = atof(buffer);
		cout << "Enter new I Value: " << endl;
		cin >> buffer;
		float i = atof(buffer);
		cout << "Enter new D Value: " << endl;
		cin >> buffer;
		float d = atof(buffer);
		request.add_State_Req(ARM_CONTROL);
		std::vector<float> msg;
		msg.push_back(j-1);
		msg.push_back(p);
		msg.push_back(i);
		msg.push_back(d);
		request.add_Complex_Msg(1337,msg);
		b = false;
		}
		else{
			request.add_State_Req(ARM_CONTROL);
			request.add_State_Req(FREE);
			b = true;
		}
	}

	void post_Comm() {

	}

};

//Usage
// youBot_Monitor 'IP'
int main(int c, char **v) {


	Tuner tuner;
	tuner.wait();

	return 0;

}
