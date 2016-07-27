/*
 * ReadRobotParameters.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "RobotParameters.h"
#include "stdlib.h"
#include<stdio.h>
#include <iostream>
#include <string.h>
#include "math.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
RobotParameters::RobotParameters(char* path)
{
	  string line;
	  ifstream myfile(path);

	  if (myfile.is_open())
	  {
	    while ( getline (myfile,line) )
	    {
			std::istringstream iss(line);

	    	if (line.find("map:") != string::npos)
	    	{
				string mapHolder;
				iss >> mapHolder >> this->map_path;
			}
	    	else if (line.find("startLocation:") != string::npos)
			{
				int a, b, c;
				string mapHolder;
				iss >> mapHolder >> a >> b >> c;
				this->start.x = a;
				this->start.y = b;
				this->start.yaw = c;
			}
	    	else if (line.find("goal:") != string::npos)
	    	{
				int a, b;
				string mapHolder;
				iss >> mapHolder >> a >> b;
				this->goal.x = a;
				this->goal.y = b;
				this->goal.yaw = 0;
			}
	    	else if (line.find("robotSize:") != string::npos)
			{
				int a, b;
				string mapHolder;
				iss >> mapHolder >> a >> b;
				this->robot_size = max(a,b);
			}
	    	else if (line.find("GridResolutionCM:") != string::npos)
			{
				string mapHolder;
				iss >> mapHolder >> grid_res;
			}
	    	else if (line.find("MapResolutionCM:") != string::npos)
			{
				string mapHolder;
				iss >> mapHolder >> map_res;
			}
	    }
	    myfile.close();
	  }

	  else cout << "File could not open";

	// TODO Auto-generated constructor stub
}

Location RobotParameters::GetStartLocation(){

	return this->start;
}


Location RobotParameters::GetGoalLocation()
{
	return this->goal;
}

const char* RobotParameters::GetMapPath(){
	return this->map_path.c_str();
}

int RobotParameters::GetRobotSize(){
	return this->robot_size;
}

float RobotParameters::GetMapResolution(){
	return this->map_res;
}

float RobotParameters::GetGridResolution(){
	return this->grid_res;
}

RobotParameters::~RobotParameters() {
	// TODO Auto-generated destructor stub
}



