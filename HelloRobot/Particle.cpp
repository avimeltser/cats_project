/*
 * Particle.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "Particle.h"
#include "math.h"

Particle::Particle(double posX, double posY, double yaw)
{
	_posX = posX;
	_posY = posY;
	_yaw = yaw;

}

Particle::Particle(){}

double Particle::GetBelife(vector < double > readings, vector < double > simulatedReadings, int readingsNumber)
{
	double accuracySum = 0;
	for(int readingIndex = 0; readingIndex < readingsNumber; readingIndex++)
	{
		double actualReading = readings[readingIndex];
		double simulatedReading = simulatedReadings[readingIndex];
		double deltaReading = sqrt(pow(actualReading - simulatedReading,2));
		double readingAccuracy = (actualReading - deltaReading) / actualReading;
		accuracySum += readingAccuracy;
	}
	double accuracyAvg = accuracySum / readingsNumber;
	return accuracyAvg;
}



void Particle::Move(double deltaDetination)
{
	double xDelta = deltaDetination * cos(_yaw);
	double yDelta = deltaDetination * sin(_yaw);
	_posX += xDelta;
	_posY += yDelta;
}

double Particle::GetPosX()
{
	return _posX;
}

double Particle::GetPosY()
{
	return _posY;
}

double Particle::GetYaw()
{
	return _yaw;
}

Location Particle::GetLocation()
{
	Location location;
	location.x = _posX;
	location.y = _posY;
	location.yaw  = _yaw;
	return location;
}
Particle::~Particle() {
	// TODO Auto-generated destructor stub
}

