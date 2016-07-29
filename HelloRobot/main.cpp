/*
 * main.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include <libplayerc++/playerc++.h>
#include "ReadFileData.h"
#include "ReadRobotParameters.h"
#include "Robot.h"
#include "Map.h"
#include "Globals.h"
#include "PathPlanner.h"
#include "WaypointsManager.h"
#include "Localization.h"
#include "stdlib.h"
using namespace PlayerCc;
using namespace std;

int main() {

	const char* parametersString = ReadFileData().FromPath("Parameters");
	GetRobotParameters robotPatameters = GetRobotParameters(parametersString);
	Location start = robotPatameters.GetStartLocation();
	Location goal = robotPatameters.GetGoalLocation();
	float gridResolution = robotPatameters.GetGridResolution();

	Map map = Map(robotPatameters.GetMapPath(),
					   robotPatameters.GetMapResolution(),
					   robotPatameters.GetGridResolution(),
					   robotPatameters.GetRobotSize()
					   ,start, goal);

	PathPlanner pathPlanner;
	Grid grid = map.grid;
	Robot robot(IP, 6665, grid.GetGridWidth(), grid.GetGridHeight());

	Location robotLocation ;
	robotLocation.x = 2.175;
	robotLocation.y = -2.875;
	robotLocation.yaw = 0.349;

	for (int i=0; i<20;i++)
	{
		robot.setFirstpPos(robotLocation.x, robotLocation.y, robotLocation.yaw);
		robot.read();
	}

	vector<unsigned char> pixels;
	unsigned width;
	unsigned height;
	map.LoadMapImage(robotPatameters.GetMapPath(), pixels,width, height);
	Localization localization;
	localization.RandomizeParticles(robotLocation);
	vector<unsigned char> pixels2 = localization.PrintParticlesOnPixels(pixels, width, height, robotPatameters.GetMapResolution(),robotLocation, robotLocation);
	map.saveMapImage("particles/particelMapPng_ORIGIN",pixels2, width, height);
	//robot.setFirstpPos(start.x, start.y, start.yaw);


	string route = pathPlanner.AStarPathFind(grid.GetGridStartLocation().y, grid.GetGridStartLocation().x,
			grid.GetGridGoalLocation().y, grid.GetGridGoalLocation().x,
			grid.GetGrid(),
			grid.GetGridHeight(),
			grid.GetGridWidth());

	pathPlanner.PrintPath(grid, route);

	Location* locations;
	WayPointsManager waypointsManager;
	int amountOfWaypoints = waypointsManager.createWaypoints(route, grid.GetGridStartLocation(), grid.GetGridGoalLocation(), locations);
	map.saveMapImage("particles/particelMapPng_ORIGIN1",pixels, width, height);
	Scan scan = Scan(pixels, width, height,robotPatameters.GetMapResolution(), robot.GetLaser());
	Location previousLocation;
	previousLocation.x = robot.getXPosition();
	previousLocation.y = robot.getYPosition();

	for(int i = 0; i < amountOfWaypoints; i++){

		robot.MoveTo(locations[i]);

		robot.read();
		Location currentLocation;
		currentLocation.x = robot.getXPosition();
		currentLocation.y = robot.getYPosition();
		currentLocation.yaw = robot.getYaw();

		double deltaToDestionation = sqrt(pow(previousLocation.x - currentLocation.x, 2) +
										  pow(previousLocation.y - currentLocation.y, 2)) ;

		// moving particles by destinationDelta
		localization.MoveParticles(deltaToDestionation);

		// Get The location that has the highest accuracy on scans
		Location newLocation = localization.GetBestLocation(scan, currentLocation);

		previousLocation.x = currentLocation.x;
		previousLocation.y = currentLocation.y;

		// Debug!
		vector<unsigned char> particelsPixels = localization.PrintParticlesOnPixels(pixels, width, height,
				robotPatameters.GetMapResolution(), currentLocation, newLocation);

		char str[100];
		str[0] = '\0';
		strcat(str,"particles/particelMapPng_");
		char chr[2];
		chr[1] = '\0';
		chr[0] = 'a' + i;
		strcat(str, chr);
		map.saveMapImage(str, particelsPixels, width, height);

		// Set new Location
		robot.SetVirtualLocation(newLocation);
		localization.RandomizeParticles(currentLocation);

	}

	return 0;
}
