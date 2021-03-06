/*
 * LocalizationManager.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include "LocalizationManager.h"
#include "math.h"
#include "Particle.h"
#include "Globals.h"
#include <stdlib.h>
#include <libplayerc++/playerc++.h>
#include "SimulateScan.h"

#define PARTICLES_NUMBER 1000
LocalizationManager::LocalizationManager() {
	srand(time(NULL));
	Particles.resize(PARTICLES_NUMBER);
}

void LocalizationManager::RandomizeParticles(Location location){
	for(int i = 0; i < PARTICLES_NUMBER; i++)
	{
		// randomize between 0 - 100
		int randomNamber = rand() % 100;
		double randomDeltaX = 0;
		double randomDeltaY = 0;
		double randomPositionYaw = 0;
		
		// randomize between 0 - 40 cms x y probability
		if(randomNamber < 50)
		{
			randomDeltaX = rand() % 40 - 20;
			randomDeltaY = rand() % 40 - 20;
		}
		// randomize between 0 - 60 cms x y probability
		else if(randomNamber < 75)
		{
			randomDeltaX = rand() % 60 - 30;
			randomDeltaY = rand() % 60 - 30;
		}
		// randomize between 0 - 70 cms x y probability
		else if(randomNamber < 90)
		{
			randomDeltaX = rand() % 70 - 35;
			randomDeltaY = rand() % 70 - 35;
		}
		// randomize between 0 - 80 cms x y probability
		else
		{
			randomDeltaX = rand()%80 - 40;
			randomDeltaY = rand()%80 - 40;
		}

		randomPositionYaw = rand() % 10 - 5;
		
		// final rnadom location
		Location randomizedLocation;
		randomizedLocation.x = location.x + randomDeltaX / 100;
		randomizedLocation.y = location.y + randomDeltaY / 100;
		randomizedLocation.yaw = location.yaw + DTOR(randomPositionYaw);
		Particles[i] = Particle(randomizedLocation.x, randomizedLocation.y, randomizedLocation.yaw);
	}
	
	_location = location;
}

vector<unsigned char> LocalizationManager::CopyPicture(vector<unsigned char> picture){
	vector<unsigned char> pictureCopy;
	int pictureSize = picture.size();
	pictureCopy.resize(pictureSize);
	for(int pixelColor =0 ; pixelColor < pictureSize; pixelColor++)
	{
		pictureCopy[pixelColor] = picture[pixelColor];
	}
	
	return pictureCopy;
}

Location LocalizationManager::GetNextPrefferedLocation(SimulateScan scan, Location original){
	if(IS_DEBUG)
	{
		return original;
	}
	
	vector <double > robotScan = scan.Robot();
	Location maxBeliefLocation;
	double maxBelief = 0;
	
	for(int i =0; i < PARTICLES_NUMBER; i++)
	{
		Particle current = Particles[i];
		Location location = current.GetLocation();
		vector <double > particleScan = scan.Particle(location);
		double belief = current.GetBelife(robotScan, particleScan, NUMBER_OF_RAYS);
		if(belief >= maxBelief)
		{
			maxBeliefLocation = current.GetLocation();
			maxBelief = belief;
		}
	}
	
	return maxBeliefLocation;
}

void LocalizationManager::Move(double deltaDetination){
	for(int i =0; i < PARTICLES_NUMBER; i++)
	{
		Particles[i].MoveParticle(deltaDetination);
	}
}


vector<unsigned char> LocalizationManager::GetParticlesPixels(vector<unsigned char> picture,int width,int height,double resolutionInCM, Location current, Location chosen){
	
	// Creating a local copy of the picture
	vector<unsigned char> pictureCopy = CopyPicture(picture);
	
	for(int i = 0; i < PARTICLES_NUMBER; i++)
	{
		Particle current = Particles[i];
		double currentX = ((current.GetPosX() * 100) / resolutionInCM) + (double)width/2;
		double currentY = height -(-(-(current.GetPosY() * 100) / resolutionInCM) + (double)height/2);
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

LocalizationManager::~LocalizationManager() {
}

