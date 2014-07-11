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

#ifndef BASECOMMANDER_H_
#define BASECOMMANDER_H_
#include "youBot_Reply.hpp"
#include "youBot_Request.hpp"


#include "youbot/YouBotBase.hpp"
#include "boost/shared_ptr.hpp"

class Base_Commander {
public:
	Base_Commander();
	virtual ~Base_Commander();

	void setup(int num,boost::shared_ptr<youbot::YouBotBase> p_myYouBotBase,boost::shared_ptr<boost::mutex> p_mtx);
	void add_Base_Info(youBot_Reply *msg);
	void execute_Command(youBot_Request *msg);
	void reset();
	void stop();
	bool exists();

private:

	boost::shared_ptr<boost::mutex> p_mtx_;
	boost::shared_ptr<youbot::YouBotBase> p_myYouBotBase_;

	void reset_thread();

	bool resetting_;
	int num_;
	float dummy_position_[3];
	float dummy_velocity_[3];
};

#endif /* BASECOMMANDER_H_ */
