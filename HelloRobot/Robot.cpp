/*
 * robot.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "Robot.h"
#include "math.h"
#include "Globals.h"
#include "RobotLocationPrinter.h"

#define ROTATE_SPEED 0.15
#define MOVE_SPEED 0.05

Robot::Robot(char* ip, int port, int gridWidth, int gridHeight){
	_pc = new PlayerCc::PlayerClient(ip,port);

	_pp = new PlayerCc::Position2dProxy(_pc);
	_lp = new LaserProxy(_pc);

	_pp->SetMotorEnable(true);
	// Sets default position
	_pp->SetOdometry(2.175,-2.875,45);
	_xDeltaFromVirtual = 0;
	_yDeltaFromVirtual = 0;
	_yawDeltaFromVirtual = 0;
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	for(int i = 0; i< 15; i++) _pc->Read();
}

void Robot::setFirstpPos(double x, double y, double yaw){
	_pp->SetOdometry(x, y, yaw);
}

void Robot::MoveTo(Location destination){
	Location current = read();
	double deltaX = destination.x - current.x;
	double deltaY = destination.y - current.y;
	double newYawInRad = atan2(deltaY, deltaX);
	double newYaw = rtod(newYawInRad);
	if(newYaw < 0)
	{
		newYaw += 360;
	}
	double currentYaw = current.yaw;
	while(abs(newYaw - currentYaw) > 1)
	{
		if(currentYaw > newYaw)
		{
			if(360 - currentYaw + newYaw < currentYaw - newYaw)
			{
				_pp -> SetSpeed(0, +ROTATE_SPEED);
			}
			else
			{
				_pp -> SetSpeed(0, -ROTATE_SPEED);
			}
		}
		else
		{
			if(360 - newYaw + currentYaw < newYaw - currentYaw)
			{
				_pp -> SetSpeed(0, -ROTATE_SPEED);
			}
			else
			{
				_pp -> SetSpeed(0, +ROTATE_SPEED);
			}
		}
		current = read();
		currentYaw = current.yaw;
	}
	_pp -> SetSpeed(0, 0);
	current = read();

	while(sqrt(pow(current.x - destination.x, 2) + pow(current.y - destination.y, 2)) > 2)
	{
		_pp -> SetSpeed(MOVE_SPEED,0);
		current = read();
	}

	_pp -> SetSpeed(0, 0);
}

// Methods
Location Robot::read()
{
	_pc->Read();
	double xx = getXPosition();
	double yy = getYPosition();
	double _yaw = getYaw();
	_x = (double)((xx * 100)/10) + (double)gridWidth/2;
	_y = -(-(double)((yy* 100)/10) + (double)gridHeight/2);
	_yaw = rtod(_yaw);
	if(_yaw < 0)
	{
		_yaw += 360;
	}
	Location location = Location();
	location.x = _x;
	location.y = _y;
	location.yaw = _yaw;
	return location;
}

void Robot::SetVirtualLocation(Location location)
{
	double x =_pp->GetXPos();
	double y =_pp->GetYPos();
	double yaw = _pp->GetYaw();
	_xDeltaFromVirtual = location.x - x;
	_yDeltaFromVirtual= location.y - y;
	_yawDeltaFromVirtual = location.yaw - yaw;


}

LaserProxy* Robot::GetLaser()
{
	return _lp;
}

double Robot::getXPosition()
{
	if(IS_DEBUG)
	{
		return _pp->GetXPos();
	}
	return _pp->GetXPos() + _xDeltaFromVirtual;
}

double Robot::getYPosition()
{
	if(IS_DEBUG)
	{
		return _pp->GetYPos();
	}
	return _pp->GetYPos() + _yDeltaFromVirtual;
}

double Robot::getYaw()
{
	if(IS_DEBUG)
	{
		return _pp->GetYaw();
	}
	return _pp->GetYaw() + _yawDeltaFromVirtual;
}
