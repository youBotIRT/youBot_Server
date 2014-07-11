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

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "youBot_Request.hpp"
#include "youBot_Reply.hpp"
#include "boost/thread.hpp"

class Client{


protected:

    zmq::socket_t *p_socket_;
    zmq::context_t *p_context_;
    std::string adr_;
    int retries_;
    int timeout_;
    int fails_;

    int msg_id_;
    int priority_;
    std::string sender_id_;

    int send_type_;
    int receive_type_;
    std::string send_type_str_;
    std::string receive_type_str_;
    Message *p_receive_Msg_;
    Message *p_send_Msg_;

    bool reconnected_;

    int rate_ms_;

    void loop();

    boost::thread t;

public:

    Client();
    Client(std::string name,  int priority, Message* send_Msg, Message* reveive_Msg);

    virtual ~Client();

	void connect(std::string adr, zmq::context_t *context);

	void dispatch_Communication(std::string adr,int rate_ms);

	int reconnect();

	int send(Message *msg);
	bool receive(Message *msg);
	bool send_receive(Message *request, Message *reply);

	void init(std::string name,  int priority, Message* send_Msg, Message* reveive_Msg);

	virtual void pre_Comm();
	virtual void post_Comm();

	void wait();

};


#endif /* CLIENT_HPP_ */
