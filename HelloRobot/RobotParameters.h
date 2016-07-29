/*
 * RobotParameters.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#ifndef READROBOTPARAMETERS_H_
#define READROBOTPARAMETERS_H_
#include "Globals.h"
class RobotParameters {
public:
	RobotParameters(const char* parameters);
	const char* GetMapPath();
	Location GetStartLocation();
	Location GetGoalLocation();
	int GetRobotSize();
	float GetMapResolution();
	float GetGridResolution();
	virtual ~RobotParameters();
private:
	char _parameters[1024];
	float FindFloat(char* pointer);
};

#endif /* READROBOTPARAMETERS_H_ */
