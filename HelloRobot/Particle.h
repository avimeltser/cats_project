/*
 * Particle.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Globals.h"
#include <stdlib.h>
#include <vector>
using namespace std;

class Particle {

private:
	double _posX, _posY, _yaw;
public:
	Particle(double posX, double posY, double yaw);
	Particle();
	double GetBelife(vector <double > readings, vector <double > simulatedReadings, int readingsNumber);
	void Move(double deltaDetination);
	double GetPosX();
	double GetPosY();
	double GetYaw();
	Location GetLocation();
	virtual ~Particle();
};

#endif /* PARTICLE_H_ */
