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

#ifndef GRIPPERCOMMANDER_HPP_
#define GRIPPERCOMMANDER_HPP_


#include "youBot_Reply.hpp"
#include "youBot_Request.hpp"

#include "youbot/YouBotManipulator.hpp"

#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"


class GripperCommander {
public:
	GripperCommander();
	virtual ~GripperCommander();

	void setup(int num,boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator,	boost::shared_ptr<boost::mutex> p_mtx);
	void add_Gripper_Info(youBot_Reply *msg);
	void execute_Command(youBot_Request *msg);
	bool exists();
	void reset();
	void stop();

public:
	int num_;
	boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator_;
	boost::shared_ptr<boost::mutex> p_mtx_;
	float position_;
	boost::thread gripper_Thread;
	bool has_gripper_;
	void loop();

};

#endif /* GRIPPERCOMMANDER_HPP_ */
