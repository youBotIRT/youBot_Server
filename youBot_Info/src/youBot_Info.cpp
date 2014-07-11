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
#include "youBotDisplay.hpp"


using namespace std;


class Info : public Client{
private:
	std::string server_adr_;
	youBot_Reply reply;
	youBot_Request request;
	youBot_Display display;
public:
	Info(std::string adr){
		server_adr_ = adr;
		init("YOUBOT::INFO", 998, &request, &reply);
		dispatch_Communication(server_adr_, 500);
		display.start();
	}
	void pre_Comm(){
		request.set_State_Req(0);
	}

	void post_Comm(){
		string line_1,line_2;
		if(reply.get_Active_Arm_Priority()==-1)
			line_1 = "Arm:  Free";
		else
			line_1 = "Arm:  " + reply.get_Active_Arm_Id().substr(reply.get_Active_Arm_Id().find_first_of("::")+2) ;

		if(reply.get_Active_Base_Priority()==-1)
			line_2 = "Base: Free";
		else
			line_2 = "Base: " + reply.get_Active_Base_Id().substr(reply.get_Active_Base_Id().find_first_of("::")+2) ;

		display.set_Display_Text(line_1,1);
		display.set_Display_Text(line_2,2);
	}

};

int main (int c, char **v)
{
	string adr;
	if (c != 2) {
		adr = "tcp://localhost:5555";
	} else {
		adr = v[1];
		adr += ":5555";
	}

	Info i(adr);
	i.wait();

	return 0;
}
