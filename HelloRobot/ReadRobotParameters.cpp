/*
 * ReadRobotParameters.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "ReadRobotParameters.h"
#include "stdlib.h"
#include<stdio.h>
#include <iostream>
#include <string.h>
#include "math.h"

using namespace std;
GetRobotParameters::GetRobotParameters(const char parameters[]) {
	// TODO Auto-generated constructor stub
	strcpy(_parameters, parameters);
}

Location GetRobotParameters::GetStartLocation(){
	char* pointer = strstr(_parameters, "startLocation:");
	bool onNumber = false;

	int locationIndex = 0;
	float location[3];

	while(*pointer != '\n' && *pointer != '\0')
	{
		if((*pointer >= '0' && *pointer <= '9') || *pointer == '.'|| *pointer == '+'|| *pointer == '-')
		{
			if(! onNumber)
			{
				onNumber = true;
				float coordinate = FindFloat(pointer);
				location[locationIndex] = coordinate;
				locationIndex++;
			}
		}
		else
		{
			onNumber = false;
		}
		pointer ++;
	}
	Location locationStruct = Location();
	locationStruct.x = location[0];
	locationStruct.y = location[1];
	locationStruct.yaw = location[2];

	return locationStruct;
}


Location GetRobotParameters::GetGoalLocation(){
	char* pointer = strstr(_parameters, "goal:");
		bool onNumber = false;

		int locationIndex = 0;
		float location[2];

		while(*pointer != '\n' && *pointer != '\0')
		{
			if((*pointer >= '0' && *pointer <= '9') || *pointer == '.'|| *pointer == '+'|| *pointer == '-')
			{
				if(! onNumber)
				{
					onNumber = true;
					float coordinate = FindFloat(pointer);
					location[locationIndex] = coordinate;
					locationIndex++;
				}
			}
			else
			{
				onNumber = false;
			}
			pointer ++;
		}

		Location locationStruct = Location();
		locationStruct.x = location[0];
		locationStruct.y = location[1];
		locationStruct.yaw = 0;

		return locationStruct;
}

const char* GetRobotParameters::GetMapPath(){
	char* pointer = strstr(_parameters, "map:");
	pointer += 4;// jump on the variable name
	string mapPath = "";

	while(*pointer == ' ') // trim
	{
		pointer++;
	}

	while(*pointer != '\0' && *pointer != '\n')
	{
		mapPath += *pointer;
		pointer++;
	}

	return mapPath.c_str();
}

int GetRobotParameters::GetRobotSize(){
	char* pointer = strstr(_parameters, "robotSize:");
		bool onNumber = false;

		int robotSizeIndex = 0;
		int robotSize[2];

		while(*pointer != '\n' && *pointer != '\0')
		{
			if((*pointer >= '0' && *pointer <= '9') || *pointer == '.'|| *pointer == '+'|| *pointer == '-')
			{
				if(! onNumber)
				{
					onNumber = true;
					float size = FindFloat(pointer);
					robotSize[robotSizeIndex] = (int)size;
					robotSizeIndex++;
				}
			}
			else
			{
				onNumber = false;
			}
			pointer ++;
		}

		return max(robotSize[0], robotSize[1]);
}

float GetRobotParameters::GetMapResolution(){
	const char* mapResolutionVariableName = "MapResolutionCM:";
	char* pointer = strstr(_parameters, mapResolutionVariableName);
	pointer += strlen(mapResolutionVariableName);// jump on the variable name
	while(*pointer == ' ') // trim
	{
		pointer++;
	}
	float resolutionInCM = FindFloat(pointer);
	return resolutionInCM;
}

float GetRobotParameters::GetGridResolution(){
	const char* mapResolutionVariableName = "GridResolutionCM:";
	char* pointer = strstr(_parameters, mapResolutionVariableName);
	pointer += strlen(mapResolutionVariableName);// jump on the variable name
	while(*pointer == ' ') // trim
	{
		pointer++;
	}
	float resolutionInCM = FindFloat(pointer);
	return resolutionInCM;
}

float GetRobotParameters::FindFloat(char * pointer)
{
	bool foundDecDot = false;
	int number = 0;
	float decimalPart = 0;
	int sign = 1;
	while((*pointer >= '0' && *pointer <= '9') || *pointer == '.'|| *pointer == '+'|| *pointer == '-')
	{
		if(*pointer >= '0' && *pointer <= '9')
		{
			int currentDigit = *pointer - '0';
			if(!foundDecDot)
			{
				number = number *10 + currentDigit;
			}
			else
			{
				decimalPart = decimalPart * 10 + currentDigit;
			}
		}
		else if(*pointer == '.')// found the decimal dot indicator
		{
			foundDecDot = true;
		}
		else if(*pointer == '+')// set signd
		{
			sign = 1;
		}
		else if(*pointer == '-')// set sign
		{
			sign = -1;
		}
		pointer++;
	}
	while(decimalPart > 1)
	{
		decimalPart /= 10;
	}
	return (number + decimalPart) * sign;
}

GetRobotParameters::~GetRobotParameters() {
	// TODO Auto-generated destructor stub
}



