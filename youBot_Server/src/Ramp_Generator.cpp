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

#include "Ramp_Generator.hpp"



Ramp_Generator::Ramp_Generator(void)
{
	ramp_state_ = 0;
	cycle_time_ =0;
	slow_joint_ =0;
	speed_ =0.2;
	t_ =0;
	t_a_ramp_ =0;
	t_c_ramp_ =0;
	time_ =0;
}

Ramp_Generator::~Ramp_Generator(void)
{
}

void Ramp_Generator::setup(std::vector<float> start_position, std::vector<float> target_position, std::vector<float> velocity, std::vector<float> &acceleration, float speed)
{




	time_ = 0;
	speed_ = speed;

	ramp_state_ = INIT_PHASE;


	ramp_start_ = start_position;
	ramp_target_ = target_position;
	ramp_velocity_max_ = velocity;
	ramp_acceleration_max_ = acceleration;

	t_a_.resize(ramp_target_.size(),0);
	t_c_.resize(ramp_target_.size(),0);
	s_c_.resize(ramp_target_.size(),0);
	s_a_.resize(ramp_target_.size(),0);
	ramp_velocity_.resize(ramp_target_.size(),0);
	ramp_acceleration_.resize(ramp_target_.size(),0);

	cur_target_.resize(ramp_target_.size(),0);
	cur_velocity_.resize(ramp_target_.size(),0);
	cur_acceleration_.resize(ramp_target_.size(),0);


	for(unsigned int i=0;i<ramp_target_.size();i++)
	{
		ramp_distance_.push_back(fabs(ramp_target_[i] - ramp_start_[i]));
		if(ramp_target_[i]>ramp_start_[i])
			ramp_direction_.push_back(1);
		else
			ramp_direction_.push_back(-1);
	}



	std::vector<float> duration;

	float max_duration;
	float s;

	if(speed>1)
		s = 1;
	else if(speed<=0.001)
		s = 0.001;
	else
		s = speed;

	for (unsigned int i=0;i<ramp_target_.size();i++)
	{
		duration.push_back( duration_to_Target(ramp_distance_[i],ramp_velocity_max_[i]*s,ramp_acceleration_max_[i],t_a_[i],t_c_[i]));
	}

	slow_joint_ = 0;
	max_duration = duration[0];
	for(unsigned int i=1;i<ramp_target_.size();i++)
	{
		if(max_duration < duration[i])
		{
			max_duration = duration[i];
			slow_joint_ = i;
		}
	}
	t_a_ramp_ = t_a_[slow_joint_];
	t_c_ramp_ = t_c_[slow_joint_];

	for(unsigned int i=0;i<ramp_target_.size();i++)
	{
		fit_ramp_to_time(ramp_distance_[i],t_a_ramp_,t_c_ramp_,ramp_acceleration_[i],ramp_velocity_[i]);
	}

}

bool Ramp_Generator:: generate_next_point( int dt_ms,  std::vector<float> &target_position,std::vector<float> &target_velocity  )
{

	time_ += ((float)dt_ms)/1000.0;

		if(ramp_state_ == INIT_PHASE)
		{
			ramp_state_ = DONE;
			for(unsigned int i=0;i<ramp_target_.size();i++)
			{
				time_ =0 ;
				cur_target_[i] = 0;
				cur_velocity_[i] = 0;
				cur_acceleration_[i] =0;
				if(ramp_acceleration_[i]!=0 && ramp_velocity_[i] !=0 && ramp_distance_[i] > 0.001)
				{
					ramp_state_ = P_I;
				}
			}

		}
		///////////////////////////////////////////////////////////////////
		if(ramp_state_ == P_I)
		{
			for(unsigned int i=0;i<ramp_target_.size();i++)
			{
				if( time_ >= t_a_ramp_ )
				{
					ramp_state_ = P_II;
					s_a_[i] = cur_target_[i];
				}
				else
				{
					t_ = time_;
					cur_target_[i] = ramp_acceleration_[i] * ( t_*t_/4  + t_a_ramp_*t_a_ramp_/(8*M_PI *M_PI ) * (cos(2*M_PI * t_ /t_a_ramp_)-1) );
					cur_velocity_[i] = ramp_acceleration_[i]/2 * (t_ - t_a_ramp_/(2*M_PI) * sin(2*M_PI  * t_/t_a_ramp_) );
					cur_acceleration_[i] = ramp_acceleration_[i]/2 * (1- cos(2* M_PI * t_/t_a_ramp_));
				}
			}
		}
		///////////////////////////////////////////////////////////////////
		else if(ramp_state_ == P_II)
		{
			for(unsigned int i=0;i<ramp_target_.size();i++)
			{
				if( time_ >= (t_a_ramp_+t_c_ramp_) )
				{
					ramp_state_ = P_III;
					s_c_[i] = cur_target_[i];
				}
				else
				{
					t_ = time_- t_a_ramp_;
					cur_target_[i] =s_a_[i] + t_ * ramp_velocity_[i];
					cur_velocity_[i] = ramp_velocity_[i];
					cur_acceleration_[i] =0;
				}
			}
		}
		///////////////////////////////////////////////////////////////////
		else if(ramp_state_ == P_III)
		{
			for(unsigned int i=0;i<ramp_target_.size();i++)
			{
				if( time_ > 2.0*t_a_ramp_+t_c_ramp_)
				{
					ramp_state_ = DONE;
				}
				else
				{
					t_ = time_ - (t_a_ramp_ +t_c_ramp_);
					cur_target_[i] = s_c_[i] +  ramp_velocity_[i] * t_ - ramp_acceleration_[i] * ( t_*t_/4  + t_a_ramp_*t_a_ramp_/(8*M_PI *M_PI ) * (cos(2*M_PI * t_ /t_a_ramp_)-1) );
					cur_velocity_[i] = ramp_velocity_[i]-  ramp_acceleration_[i]/2 * (t_ - t_a_ramp_/(2*M_PI) * sin(2*M_PI  * t_/t_a_ramp_) );
					cur_acceleration_[i] = - ramp_acceleration_[i]/2 * (1- cos(2* M_PI * t_/t_a_ramp_));
				}
			}
		}

		///////////////////////////////////////////////////////////////////
		else if (ramp_state_==DONE)
		{
			for(unsigned int i=0;i<ramp_target_.size();i++)
			{
				cur_velocity_[i] = 0;
				cur_acceleration_[i] = 0;
				cur_target_[i] = ramp_distance_[i];
			}
		}
		///////////////////////////////////////////////////////////////////
		target_position.resize(ramp_target_.size(),0);
		target_velocity.resize(ramp_target_.size(),0);

		for(unsigned int i=0;i<ramp_target_.size();i++)
		{
			target_position[i] = cur_target_[i] * ramp_direction_[i] + ramp_start_[i];
			target_velocity[i] = cur_velocity_[i] * ramp_direction_[i];
		}

		if(ramp_state_ == DONE)
			return false;
		else
			return true;

}


void Ramp_Generator::fit_ramp_to_time(const float &distance,const float &t_acc,const float &t_c,float &acceleration,float &velocity )
{
		if( distance < 0.001 || t_acc < 0.001) // Keine Bewegung
		{
			acceleration = 0.0f;
			velocity = 0.0f;
		}
		else
		{
			acceleration = 2.0 * distance / ( t_acc * t_acc + t_acc * t_c );
			velocity = acceleration * t_acc/2;
		}

}

float Ramp_Generator::duration_to_Target(const float &distance,const float &velocity,const float &acceleration,float &t_acc, float &t_cont )
{

	if(acceleration!=0 && velocity !=0)
	{

		if(2*pow(velocity,2)/acceleration <= distance)
		{
			t_acc = 2 * velocity/acceleration;
			t_cont = distance/velocity - 2 * velocity/acceleration;
		}
		else // K
		{
			t_cont = 0;
			t_acc = sqrt(distance/acceleration);
		}
	}
	else
	{
		t_acc  = 0;
		t_cont = 0;
	}

	return 2*t_acc + t_cont;
}
