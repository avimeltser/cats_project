/*
 * WaypointsManager.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "WaypointsManager.h"

#include <iostream>

using namespace std;

int WayPointsManager::createWaypoints(string route, Location startLocation, Location finalLocation, Location* &locations)
{
	const int dirNum=8;
	static int dirX[dirNum]={0, 1, 1, 1, 0, -1, -1, -1};
	static int dirY[dirNum]={1, 1, 0, -1, -1, -1, 0, 1};

	int nNumOfWayPoints = 0;

	// in case the route is not empty
	if (route.length() > 0)
			{
				int countOfDirections = 0;
				int lastDirection = (int)(route.at(0)-'0');
				int direction;
				locations = new Location[100];
				char tempChar;
				int x = startLocation.x;
				int y = startLocation.y;

				// running over all the route and create waypoints
				for (unsigned int i = 0; i < route.length(); i++)
				{
					tempChar = route.at(i);
					direction = tempChar-'0';

					// if the pint is the continue of the way, don't create a waypoint
					if(countOfDirections == 0 || (lastDirection == direction && countOfDirections < 5))
					{
						countOfDirections ++;
					}
					else
					{
						// create a new waypoint
						locations[nNumOfWayPoints].x = x;
						locations[nNumOfWayPoints].y = -y;
						lastDirection = direction;
						countOfDirections = 0;
						nNumOfWayPoints++;

					}

					x += dirX[direction];
					y += dirY[direction];
				}

				cout << "Finished to create waypoints, Found: " << nNumOfWayPoints << " waypoints";
				cout << endl;
			}
			else
			{
				cout << "No route found";
				cout << endl;
			}
	finalLocation.y = - finalLocation.y;
	locations[nNumOfWayPoints] = finalLocation;
	return nNumOfWayPoints + 1;
}
