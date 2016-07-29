/*
 * Particle.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Globals.h"
#include <stdlib.h>
#include <vector>
using namespace std;

class Particle {

private:
	double _xPos, _yPos, _yaw;
public:
	Particle(double posX, double posY, double yaw);
	Particle();
	double GetBelife(vector <double > readings, vector <double > simulatedReadings, int readingsNumber);
	void MoveParticle(double deltaDetination);
	double GetPosX();
	double GetPosY();
	double GetYaw();
	Location GetLocation();
	virtual ~Particle();
};

#endif /* PARTICLE_H_ */
