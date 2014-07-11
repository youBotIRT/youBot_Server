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

#include <math.h>
#include <vector>
#include <iostream>
#include <unistd.h>
#define _USE_MATH_DEFINES

#define INIT_PHASE 0

#define P_I 1
#define P_II 2
#define P_III 3

#define DONE 10

class Ramp_Generator
{
public:
	Ramp_Generator(void);
	~Ramp_Generator(void);

	void setup(std::vector<float> start_position, std::vector<float> target_position, std::vector<float> velocity, std::vector<float> &acceleration, float speed);
	bool generate_next_point( int dt_ms,std::vector<float> &target_position,std::vector<float> &target_velocity  ); // Generates the next Ramp Point and returns the current Path State


private:
	void fit_ramp_to_time( const float &distance,const float &t_acc,const float &t_c,float &acceleration,float &velocity );
	float duration_to_Target( const float &distance,const float &velocity,const float &acceleration,float &t_acc, float &t_cont );



	int ramp_state_;

	std::vector<float> ramp_start_;
	std::vector<float> ramp_target_;

	std::vector<float> t_a_;
	std::vector<float> t_c_;
	float t_a_ramp_;
	float t_c_ramp_;
	int slow_joint_;
	std::vector<float> ramp_velocity_;
	std::vector<float> ramp_acceleration_;
	std::vector<float> ramp_velocity_max_;
	std::vector<float> ramp_acceleration_max_;

	std::vector<float> ramp_distance_;
	std::vector<float> ramp_direction_;


	std::vector<float> cur_target_;
	std::vector<float> cur_velocity_;
	std::vector<float> cur_acceleration_;
	std::vector<float> s_c_;
	std::vector<float> s_a_;

	float speed_;
	float cycle_time_;

	float time_;
	float t_;

};

