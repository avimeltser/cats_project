/*
 * WaypointsManager.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
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
