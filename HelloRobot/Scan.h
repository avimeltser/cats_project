/*
 * SimulateScan.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef SIMULATESCAN_H_
#define SIMULATESCAN_H_
#define NUMBER_OF_RAYS 6
#include <vector>
#include "Globals.h"
#include <libplayerc++/playerc++.h>

using namespace PlayerCc;
using namespace std;

class Scan {
private:
	vector <unsigned char> _picture;
	int _width;
	int _height;
	double _resolutionInCM;
	LaserProxy* _laserProxy;
	double LaserAngles[NUMBER_OF_RAYS];

public:
	Scan(vector <unsigned char> picture, int width, int height,double resolutionInCM, LaserProxy* laserProxy);
	vector < double > Robot();
	vector < double > Particle(Location location);
	virtual ~Scan();
	bool HasBarrierIn(Location location);
};

#endif /* SIMULATESCAN_H_ */
