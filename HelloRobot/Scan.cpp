/*
 * SimulateScan.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "Scan.h"
#include "math.h"

#define ANGLE_BETWEEN_RAYS 20
using namespace std;
Scan::Scan(vector <unsigned char> picture, int width, int height,double resolutionInCM, LaserProxy* laserProxy)
{
	_picture = picture;
	_width = width;
	_height = height;
	_laserProxy = laserProxy;
	_resolutionInCM = resolutionInCM;

	uint laserNumberOfRays = _laserProxy->GetCount();
	double minAngle = _laserProxy->GetMinAngle();
	double maxAngle = _laserProxy->GetMaxAngle();
	double deltaAngle = (maxAngle - minAngle) / laserNumberOfRays;

	int scanCounter = 0;
	for(int angleIndex = 0; angleIndex < laserNumberOfRays && scanCounter < 6; angleIndex+= laserNumberOfRays/ NUMBER_OF_RAYS)
	{
		LaserAngles [scanCounter] = minAngle + deltaAngle * angleIndex;
		scanCounter++;
	}
}
vector<double > Scan::Robot()
{
	vector<double > scans;
	scans.resize(NUMBER_OF_RAYS);
	uint laserNumberOfRays = _laserProxy->GetCount();
	double minAngle = _laserProxy->GetMinAngle();
	double maxAngle = _laserProxy->GetMaxAngle();
	double deltaAngle = (maxAngle - minAngle) / laserNumberOfRays;
	double angleDeltaIndex = laserNumberOfRays/ NUMBER_OF_RAYS;
	int scanCounter = 0;
	for(int angleIndex = 0; angleIndex < laserNumberOfRays && scanCounter < 6; angleIndex+= angleDeltaIndex)
	{
		double distanceToBarrier = (*_laserProxy)[angleIndex];
		scans [scanCounter] = distanceToBarrier;
		scanCounter++;
	}
	return scans;
}

vector<double > Scan::Particle(Location location)
{
	vector<double > scans2;
	scans2.resize(NUMBER_OF_RAYS);

	double maxRange = 4;
	for(int angleIndex = 0; angleIndex < NUMBER_OF_RAYS; angleIndex++)
	{
		double calculatedAngle = LaserAngles[angleIndex] + location.yaw;
		double foundScan = maxRange;
		for(double range = 0.1; range < maxRange; range += 0.1)
		{
			Location calculatedLocation;
			double xDelta = range * cos(calculatedAngle);
			double yDelta = -range * sin(calculatedAngle);
			calculatedLocation.x = location.x + xDelta;
			calculatedLocation.y = location.y + yDelta;
			bool foundBarrier;
			foundBarrier = HasBarrierIn(calculatedLocation);
			if(foundBarrier)
			{
				foundScan = range;
				break;
			}
		}
		scans2[angleIndex] = foundScan;
	}
	return scans2;
}

bool Scan::HasBarrierIn(Location location)
{
	double currentX = round(((location.x * 100) / _resolutionInCM) + (double)_width/2);
	double currentY = round( _height -(-(-(location.y * 100) / _resolutionInCM) + (double)_height/2));
	for(int x = MAX(currentX -5, 0); x < MIN(currentX + 5, _width); x++)
	{
		for(int y = MAX(currentY -5, 0); y < MIN(currentY + 5, _height); y++)
		{
			int yoffset = _width * 4 * y;
			int xoffset = x * 4;
			int offset = yoffset + xoffset;
			bool hasBarrier = _picture[offset] != 255;
			if(hasBarrier) return true;
		}
	}

	return false;
}

Scan::~Scan() {
	// TODO Auto-generated destructor stub
}

