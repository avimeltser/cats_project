/*
 * Localization.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include "Particle.h"
#include "vector"
#include "Globals.h"
#include "Scan.h"

using namespace std;

class Localization {
private:
	Location _currentLocation;
	vector < Particle > Particles;
public:
	Localization();
	void RandomizeParticles(Location originalLocation);
	Location RandomizeLocation(Location originalLocation);
	Location GetBestLocation(Scan scan, Location originLocation);
	void MoveParticles(double deltaDetination);
	vector<unsigned char> PrintParticlesOnPixels(vector<unsigned char > picture,int width,int height,double resolutionInCM, Location current, Location chosen);
	virtual ~Localization();
};

#endif /* LOCALIZATION_H_ */
