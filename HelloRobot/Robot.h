/*
 * robot.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
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
	PlayerClient* _playerClient;
	Position2dProxy* _playerProxy;
	LaserProxy* _laserProxy;
	double _x;
	double _y;
	double _yaw;
	double _oldX;
	double _oldY;
	double _oldYaw;
	double _xDistanceFromVirtual;
	double _yDistanceFromVirtual;
	double _yawDistanceFromVirtual;

public:
	double robotWidth;
	double robotLengt;
	double mapResolution;
	double gridResolution;
	int gridHeight;
	int gridWidth;
	Robot(char* ip, int port, int gridWidth, int gridHeight);
	void MoveRobot(Location destination);
	Location read();
	void setFirstpPos(double x, double y, double yaw);
	LaserProxy* GetLaser();
	double getXPosition();
	double getYPosition();
	double getYaw();
	void SetVirtualLocation(Location location);
};

#endif /* ROBOT_H_ */
