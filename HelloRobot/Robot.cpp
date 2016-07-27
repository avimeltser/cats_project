/*
 * robot.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include "Robot.h"
#include "math.h"
#include "Globals.h"

#define ROTATE_SPEED 0.15
#define MOVE_SPEED 0.05

Robot::Robot(char* ip, int port, int gridWidth, int gridHeight){
	_playerClient = new PlayerCc::PlayerClient(ip,port);

	_playerProxy = new PlayerCc::Position2dProxy(_playerClient);
	_laserProxy = new LaserProxy(_playerClient);

	_playerProxy->SetMotorEnable(true);
	// Sets default position
	_playerProxy->SetOdometry(2.175,-2.875,45);
	_xDistanceFromVirtual = 0;
	_yDistanceFromVirtual = 0;
	_yawDistanceFromVirtual = 0;
	this->gridWidth = gridWidth;
	this->gridHeight = gridHeight;
	for(int i = 0; i< 15; i++) _playerClient->Read();
}

void Robot::MoveRobot(Location destination){
	Location current = read();
	double distanceX = destination.x - current.x;
	double distanceY = destination.y - current.y;
	double newYawInRad = atan2(distanceY, distanceX);
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
				_playerProxy -> SetSpeed(0, +ROTATE_SPEED);
			}
			else
			{
				_playerProxy -> SetSpeed(0, -ROTATE_SPEED);
			}
		}
		else
		{
			if(360 - newYaw + currentYaw < newYaw - currentYaw)
			{
				_playerProxy -> SetSpeed(0, -ROTATE_SPEED);
			}
			else
			{
				_playerProxy -> SetSpeed(0, +ROTATE_SPEED);
			}
		}
		current = read();
		currentYaw = current.yaw;
	}
	_playerProxy -> SetSpeed(0, 0);
	current = read();

	while(sqrt(pow(current.x - destination.x, 2) + pow(current.y - destination.y, 2)) > 2)
	{
		_playerProxy -> SetSpeed(MOVE_SPEED,0);
		current = read();
	}

	_playerProxy -> SetSpeed(0, 0);
}

Location Robot::read()
{
	_playerClient->Read();
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
	double x =_playerProxy->GetXPos();
	double y =_playerProxy->GetYPos();
	double yaw = _playerProxy->GetYaw();
	_xDistanceFromVirtual = location.x - x;
	_yDistanceFromVirtual= location.y - y;
	_yawDistanceFromVirtual = location.yaw - yaw;


}

LaserProxy* Robot::GetLaser()
{
	return _laserProxy;
}

void Robot::setFirstpPos(double x, double y, double yaw){
	_playerProxy->SetOdometry(x, y, yaw);
}


double Robot::getXPosition()
{
	if(IS_DEBUG)
	{
		return _playerProxy->GetXPos();
	}
	return _playerProxy->GetXPos() + _xDistanceFromVirtual;
}

double Robot::getYPosition()
{
	if(IS_DEBUG)
	{
		return _playerProxy->GetYPos();
	}
	return _playerProxy->GetYPos() + _yDistanceFromVirtual;
}

double Robot::getYaw()
{
	if(IS_DEBUG)
	{
		return _playerProxy->GetYaw();
	}
	return _playerProxy->GetYaw() + _yawDistanceFromVirtual;
}
