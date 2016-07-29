/*
 * SimulateScan.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#ifndef SIMULATESCAN_H_
#define SIMULATESCAN_H_
#define NUMBER_OF_RAYS 6
#include <vector>
#include "Globals.h"
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;
using namespace std;

class SimulateScan {
private:
	vector <unsigned char> _picture;
	int _width;
	int _height;
	double _resolutionInCM;
	LaserProxy* _laserProxy;
	double LaserAngles[NUMBER_OF_RAYS];

public:
	SimulateScan(vector <unsigned char> picture, int width, int height,double resolutionInCM, LaserProxy* laserProxy);
	vector < double > Robot();
	vector < double > Particle(Location location);
	virtual ~SimulateScan();
	bool HasBarrierIn(Location location);
};

#endif /* SIMULATESCAN_H_ */
