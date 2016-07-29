/*
 * Localization.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "Localization.h"
#include "math.h"
#include "Particle.h"
#include "Globals.h"
#include <stdlib.h>
#include <libplayerc++/playerc++.h>
#include "Scan.h"

#define NUMBER_OF_PARTICALES 1000
Localization::Localization() {
	// TODO Auto-generated constructor stub
	srand(time(NULL));
	Particles.resize(NUMBER_OF_PARTICALES);
}

void Localization::RandomizeParticles(Location originalLocation)
{
	for(int particleNumber = 0; particleNumber < NUMBER_OF_PARTICALES; particleNumber++)
	{
		Location randomizedLocation = RandomizeLocation(originalLocation);
		Particles[particleNumber] = Particle(randomizedLocation.x, randomizedLocation.y, randomizedLocation.yaw);
	}
	_currentLocation = originalLocation;
}

Location Localization::GetBestLocation(Scan scan, Location originLocation)
{
	if(IS_DEBUG)
	{
		return originLocation;
	}
	vector <double > robotScan = scan.Robot();
	Location maxBeliefLocation;
	double maxBelief = 0;
	for(int particleIndex =0; particleIndex < NUMBER_OF_PARTICALES; particleIndex++)
	{
		Particle currentParticle = Particles[particleIndex];
		Location particleLocation = currentParticle.GetLocation();
		vector <double > particleScan = scan.Particle(particleLocation);
		double belief = currentParticle.GetBelife(robotScan, particleScan, NUMBER_OF_RAYS);
		if(belief >= maxBelief)
		{
			maxBeliefLocation = currentParticle.GetLocation();
			maxBelief = belief;
		}
	}
	return maxBeliefLocation;
}

void Localization::MoveParticles(double deltaDetination)
{
	for(int particleIndex =0; particleIndex < NUMBER_OF_PARTICALES; particleIndex++)
	{
		Particles[particleIndex].Move(deltaDetination);
	}
}

Location  Localization::RandomizeLocation(Location originalLocation)
{
	// between 0 - 100
	int randomNamber = rand() % 100;
	double randomDeltaX = 0;
	double randomDeltaY = 0;
	double randomPositionYaw = 0;
	// 0 - 40 cms x y probability
	if(randomNamber < 50)
	{
		randomDeltaX = rand() % 40 - 20;
		randomDeltaY = rand() % 40 - 20;
	}
	// 0 - 60 cms x y probability
	else if(randomNamber < 75)
	{
		randomDeltaX = rand() % 60 - 30;
		randomDeltaY = rand() % 60 - 30;
	}
	// 0 - 70 cms x y probability
	else if(randomNamber < 90)
	{
		randomDeltaX = rand() % 70 - 35;
		randomDeltaY = rand() % 70 - 35;
	}
	// 0 - 80 cms x y probability
	else
	{
		randomDeltaX = rand()%80 - 40;
		randomDeltaY = rand()%80 - 40;
	}

	randomPositionYaw = rand() % 10 - 5;
	Location particleLocation;
	particleLocation.x = originalLocation.x + randomDeltaX / 100;
	particleLocation.y = originalLocation.y + randomDeltaY / 100;
	particleLocation.yaw = originalLocation.yaw + DTOR(randomPositionYaw);
	return particleLocation;

}

vector<unsigned char> Localization::PrintParticlesOnPixels(vector<unsigned char> picture,int width,int height,double resolutionInCM, Location current, Location chosen)
{
	vector<unsigned char> pictureCopy;
	int pictureSize = picture.size();
	pictureCopy.resize(pictureSize);
	for(int pixelColor =0 ; pixelColor < pictureSize; pixelColor++)
	{
		pictureCopy[pixelColor] = picture[pixelColor];
	}

	for(int particleNumber = 0; particleNumber < NUMBER_OF_PARTICALES; particleNumber++)
	{
		Particle currentParticle = Particles[particleNumber];
		double currentX = ((currentParticle.GetPosX() * 100) / resolutionInCM) + (double)width/2;
		double currentY = height -(-(-(currentParticle.GetPosY() * 100) / resolutionInCM) + (double)height/2);
		int yoffset = width * 4 * round(currentY);
		int xoffset = round(currentX) * 4;
		int offset = yoffset + xoffset;
		pictureCopy[offset] = 255;
		pictureCopy[offset + 1] = 0;
		pictureCopy[offset + 2] = 0;
		pictureCopy[offset + 3] = 255;
	}

	double currentX = ((current.x * 100) / resolutionInCM) + (double)width/2;
	double currentY = height -(-(-(current.y * 100) / resolutionInCM) + (double)height/2);
	int yoffset = width * 4 * round(currentY);
	int xoffset = round(currentX) * 4;
	int offset = yoffset + xoffset;
	pictureCopy[offset] = 0;
	pictureCopy[offset + 1] = 0;
	pictureCopy[offset + 2] = 255;
	pictureCopy[offset + 3] = 255;


	currentX = ((chosen.x * 100) / resolutionInCM) + (double)width/2;
	currentY = height -(-(-(chosen.y * 100) / resolutionInCM) + (double)height/2);
	yoffset = width * 4 * round(currentY);
	xoffset = round(currentX) * 4;
	offset = yoffset + xoffset;
	pictureCopy[offset] = 0;
	pictureCopy[offset + 1] = 255;
	pictureCopy[offset + 2] = 255;
	pictureCopy[offset + 3] = 255;
	return pictureCopy;
}

Localization::~Localization() {
	// TODO Auto-generated destructor stub
}
