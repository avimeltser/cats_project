/*
 * robot.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef ROBOT_H_
#define ROBOT_H_
#include <string.h>
#include <libplayerc++/playerc++.h>
#include <iostream>
#include <vector>
#include "Globals.h"
using namespace PlayerCc;
using namespace std;

class Robot {
private:
	PlayerClient* _pc;
	Position2dProxy* _pp;
	LaserProxy* _lp;
	double _x;
	double _y;
	double _yaw;
	double _oldX;
	double _oldY;
	double _oldYaw;
	double _xDeltaFromVirtual;
	double _yDeltaFromVirtual;
	double _yawDeltaFromVirtual;

public:
	double robotWidth;
	double robotLengt;
	double mapResolution;
	double gridResolution;
	int gridHeight;
	int gridWidth;
	Robot(char* ip, int port, int gridWidth, int gridHeight);
	void MoveTo(Location destination);
	Location read();
	void setFirstpPos(double x, double y, double yaw);
	LaserProxy* GetLaser();
	double getXPosition();
	double getYPosition();
	double getYaw();
	void SetVirtualLocation(Location location);
};

#endif /* ROBOT_H_ */
