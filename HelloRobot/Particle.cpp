/*
 * Particle.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include "Particle.h"
#include "math.h"

Particle::Particle(double posX, double posY, double yaw)
{
	_xPos = posX;
	_yPos = posY;
	_yaw = yaw;

}

Particle::Particle(){}

double Particle::GetBelife(vector < double > readings, vector < double > simulatedReadings, int readingsNumber)
{
	double accuracySum = 0;
	
	for(int i = 0; i < readingsNumber; i++)
	{
		double actualReading = readings[i];
		double simulatedReading = simulatedReadings[i];
		double deltaReading = sqrt(pow(actualReading - simulatedReading,2));
		double readingAccuracy = (actualReading - deltaReading) / actualReading;
		accuracySum += readingAccuracy;
	}
	
	double accuracyAvg = accuracySum / readingsNumber;
	return accuracyAvg;
}



void Particle::MoveParticle(double detination)
{
	double xDestination = detination * cos(_yaw);
	double yDestination = detination * sin(_yaw);
	_xPos += xDestination;
	_yPos += yDestination;
}

double Particle::GetPosX()
{
	return _xPos;
}

double Particle::GetPosY()
{
	return _yPos;
}

double Particle::GetYaw()
{
	return _yaw;
}

Location Particle::GetLocation()
{
	Location location;
	location.x = _xPos;
	location.y = _yPos;
	location.yaw  = _yaw;
	return location;
}
Particle::~Particle() {
	// TODO Auto-generated destructor stub
}

