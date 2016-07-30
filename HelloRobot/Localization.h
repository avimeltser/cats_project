/*
 * Localization.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include "Particle.h"
#include "vector"
#include "Globals.h"
#include "SimulateScan.h"

using namespace std;

class Localization {
private:
	Location _currentLocation;
	vector < Particle > Particles;
public:
	Localization();
	void RandomizeParticles(Location location);
	Location Randomize(Location location);
	Location GetBestLocation(SimulateScan scan, Location original);
	void Move(double deltaDetination);
	vector<unsigned char> PrintParticlesOnPixels(vector<unsigned char > picture,int width,int height,double resolutionInCM, Location current, Location chosen);
	virtual ~Localization();
};

#endif /* LOCALIZATION_H_ */
