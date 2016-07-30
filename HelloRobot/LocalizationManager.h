/*
 * LocalizationManager.h
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

class LocalizationManager {
private:
	Location _location;
	vector < Particle > Particles;
public:
	LocalizationManager();
	void RandomizeParticles(Location location);
	vector<unsigned char> CopyPicture(vector<unsigned char> picture);
	Location GetNextPrefferedLocation(SimulateScan scan, Location original);
	void Move(double deltaDetination);
	vector<unsigned char> GetParticlesPixels(vector<unsigned char > picture,int width,int height,double resolutionInCM, Location current, Location chosen);
	virtual ~LocalizationManager();
};

#endif /* LOCALIZATION_H_ */
