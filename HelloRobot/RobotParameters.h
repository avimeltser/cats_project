/*
 * ReadRobotParameters.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef READROBOTPARAMETERS_H_
#define READROBOTPARAMETERS_H_

#include <string>
#include "Globals.h"

class RobotParameters {
public:
	RobotParameters(char* config_file_path);
	const char* GetMapPath();
	Location GetStartLocation();
	Location GetGoalLocation();
	int GetRobotSize();
	float GetMapResolution();
	float GetGridResolution();
	virtual ~RobotParameters();
private:
	Location start;
	Location goal;
	int robot_size;
	float grid_res;
	float map_res;
	std::string map_path;
};

#endif /* READROBOTPARAMETERS_H_ */
