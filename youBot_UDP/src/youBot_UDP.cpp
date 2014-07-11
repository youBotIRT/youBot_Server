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

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Client.hpp"
#include "MessageParser.hpp"
#include "Watchdog.hpp"
#include "boost/thread.hpp"

using namespace std;

class youBot_UDP: public Client {

private:

	youBot_Request request_msg_;
	youBot_Reply reply_msg_;
	Message_Parser msg_parser;

	int socket_id;
	char *buffer;


	struct sockaddr_in client;

	bool received_msg_ok;

	int open(int port) {
		struct sockaddr_in servAddr;
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons(port);
		int iSockFd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		int bind_ret = bind(iSockFd, (struct sockaddr*) &servAddr, sizeof(servAddr));
		cout << "Socket returned: " << iSockFd << " Bind returned: " << bind_ret << endl;
		return iSockFd;
	}

	void udp_send_msg( char buff[], int &length) {
		sendto(socket_id, buff, length, 0, (struct sockaddr*) &client, sizeof(client));
	}

	void udp_recv_msg(char buff[], int &length) {

		int cliAddrLen = sizeof(struct sockaddr_in);
		length = recvfrom(socket_id, buff, 1024, 0, (struct sockaddr*) &client, (socklen_t*) &cliAddrLen);
		client.sin_family = AF_INET;

	}

public:
	youBot_UDP(int c, char **v) {
		string name;
		int port;
		if (c != 2) {
			port = 6500;
			name = "YOUBOT::UDP:6500";
		} else {
			port = atoi(v[1]);
			name = "YOUBOT::UDP:";
			name += v[1];
		}

		socket_id = open(port);

		init(name, port % 100 + 100, &request_msg_, &reply_msg_);
		dispatch_Communication("tcp://localhost:5555", -1);
	}

	void pre_Comm() {
		int l;

		bool waiting = true;

		while (waiting) {
			try {
				udp_recv_msg( buffer, l);

				msg_parser.to_Request(buffer, l, request_msg_);
				waiting = false;
			} catch (std::exception &e) {
				cout << e.what() << endl;
				waiting = true;
			}
		}

	}

	void post_Comm() {
		int l;
		msg_parser.to_Byte_Array(reply_msg_, buffer, l);
//		for(int i=0;i<l;i++){
//			std::cout<< i<< " | "<< std::dec << ((int)buffer[i]&0xFF) << endl;
//		}
		cout << "Bytes Send: " << l << endl;
		udp_send_msg( buffer, l);
	}

};

int main(int c, char **v) {
	youBot_UDP yb(c,v);
	yb.wait();
}

