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
#include "Globals.h"
class GetRobotParameters {
public:
	GetRobotParameters(const char* parameters);
	const char* GetMapPath();
	Location GetStartLocation();
	Location GetGoalLocation();
	int GetRobotSize();
	float GetMapResolution();
	float GetGridResolution();
	virtual ~GetRobotParameters();
private:
	char _parameters[1024];
	float FindFloat(char* pointer);
};

#endif /* READROBOTPARAMETERS_H_ */
