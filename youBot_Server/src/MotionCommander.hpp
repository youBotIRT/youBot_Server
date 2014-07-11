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

#ifndef MOTIONCOMMANDER_HPP_
#define MOTIONCOMMANDER_HPP_

#include "JointController.hpp"
#include "PID.hpp"
#include "boost/thread.hpp"

#include "AbstractAction.hpp"
#include "PTPMotion.hpp"
#include "LinearVelocity.hpp"
#include "Trajectory.hpp"

class Motion_Commander {

public:
	Motion_Commander();
	void setup(boost::shared_ptr<JointController> p_joints, int rate);
	virtual ~Motion_Commander();bool in_Moiton();

	void update_Action(int type, std::vector<float> value);
	void stop_Action();

private:
	Abstract_Action *action_;bool active_;
	boost::thread motion_Thread_;
	int rate_;
	boost::shared_ptr<JointController> p_joints_;
	boost::mutex startup_mtx_;
	int last_type_;
	void loop();

};

#endif /* MOTIONCOMMANDER_HPP_ */
