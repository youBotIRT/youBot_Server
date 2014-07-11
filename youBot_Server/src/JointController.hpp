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

#ifndef JOINTCONTROLLER_H_
#define JOINTCONTROLLER_H_

#define POSITION_PID 2
#define POSITION 3
#define VELOCITY 4
#define CURRENT 5
#define STOP_JOINT 6
#define CLEAR 7
#define IDLE 8

#include "youbot/YouBotManipulator.hpp"
#include "youBot_Reply.hpp"
#include "youBot_Request.hpp"
#include "boost/thread.hpp"
#include "SingleJoint.hpp"
#include "Loop_Rates.hpp"



using namespace youbot;

class JointController {

private:

	SingleJoint joint[5];

	void loop();

	int loop_rate_ms_;

	std::vector<int> type_;
	std::vector<std::vector<float> >setpoint_;

public:
	JointController();

	void setup(boost::shared_ptr<youbot::YouBotManipulator> p_myYouBotManipulator,
			boost::shared_ptr<boost::mutex> mtx);

	void execute_Commands(youBot_Request *req);
	void stop();
	void reset();

	void update_Trajectory(std::vector<float> position, std::vector<float> velocity , std::vector<float> acceleration  );
	void update_Trajectory(std::vector<float> position, std::vector<float> velocity  );
	void update_Trajectory(std::vector<float> position );

	void add_Arm_Info(youBot_Reply* msg);

	std::vector<float> get_Positions();
	std::vector<float> get_Velocitys();
	std::vector<float> get_Filterd_Velocitys();
	std::vector<float> get_Max_Velocitys();
	std::vector<float> get_Max_Accelerations();

	SingleJoint* get_Joint(int i);
};

#endif /* JOINTCONTROLLER_H_ */
