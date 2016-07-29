/*
 * WaypointsManager.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

using namespace std;

#include<string>
#include "Globals.h"

class WayPointsManager{

public:
static int createWaypoints(string route, Location startLocation, Location finalLocation, Location* &arr);

};

#endif
