/*
 * main.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include <libplayerc++/playerc++.h>
#include "ReadFileData.h"
#include "RobotParameters.h"
#include "Robot.h"
#include "Map.h"
#include "Globals.h"
#include "PathPlanner.h"
#include "WaypointsManager.h"
#include "LocalizationManager.h"
#include "stdlib.h"
using namespace PlayerCc;
using namespace std;

int main() {

	// Initializing robot and it's parameters
	const char* parametersString = ReadFileData().FromPath("Parameters");
	RobotParameters robotPatameters = RobotParameters(parametersString);

	// Initial location
	Location start = robotPatameters.GetStartLocation();

	// Goal location
	Location dest = robotPatameters.GetGoalLocation();

	// Initial map with robot parameters
	string path = robotPatameters.GetMapPath();
	float mapResolution = robotPatameters.GetMapResolution();
	float gridResolution = robotPatameters.GetGridResolution();
	int robotSize  = robotPatameters.GetRobotSize();
	Map map = Map(robotPatameters.GetMapPath(), mapResolution, gridResolution, robotSize ,start, dest);

	// Initial Grid
	PathPlanner pathPlanner;
	Grid grid = map.grid;
	Robot robot("localhost", 6665, grid.GetGridWidth(), grid.GetGridHeight());
	Location robotLocation ;
	robotLocation.x = 2.175;
	robotLocation.y = -2.875;
	robotLocation.yaw = 0.785;
	for (int i=0; i<20;i++)
	{
	robot.setFirstpPos(robotLocation.x, robotLocation.y, robotLocation.yaw);
	robot.read();
	}

	// Load map
	vector<unsigned char> pixels;
	unsigned width;
	unsigned height;
	map.LoadMapImage(robotPatameters.GetMapPath(), pixels,width, height);
	LocalizationManager localization;
	localization.RandomizeParticles(robotLocation);
	vector<unsigned char> pixels2 = localization.GetParticlesPixels(pixels, width, height, robotPatameters.GetMapResolution(),robotLocation, robotLocation);
	map.saveMapImage("particles/particelMapPng_ORIGIN",pixels2, width, height);

	// Finding path using A* 
	string route = pathPlanner.AStarPathFind(grid.GetGridStartLocation().y, grid.GetGridStartLocation().x,
	grid.GetGridGoalLocation().y, grid.GetGridGoalLocation().x,
	grid.GetGrid(),
	grid.GetGridHeight(),
	grid.GetGridWidth());
	pathPlanner.PrintPath(grid, route);
	Location* locations;
	WayPointsManager waypointsManager;
	int amountOfWaypoints = waypointsManager.createWaypoints(route, grid.GetGridStartLocation(), grid.GetGridGoalLocation(), locations);

	// Saving map
	map.saveMapImage("particles/particelMapPng_ORIGIN1",pixels, width, height);
	SimulateScan scan = SimulateScan (pixels, width, height,robotPatameters.GetMapResolution(), robot.GetLaser());
	Location previousLocation;

	// Setting updated location
	previousLocation.x = robot.getXPosition();
	previousLocation.y = robot.getYPosition();


	// Moving the robot using the location array
	for(int i = 0; i < amountOfWaypoints; i++){
	robot.MoveRobot(locations[i]);
	robot.read();
	Location currentLocation;
	currentLocation.x = robot.getXPosition();
	currentLocation.y = robot.getYPosition();
	currentLocation.yaw = robot.getYaw();
	double deltaToDestionation = sqrt(pow(previousLocation.x - currentLocation.x, 2) +
	pow(previousLocation.y - currentLocation.y, 2)) ;
	// moving particles by destinationDelta
	localization.Move(deltaToDestionation);
	// Get The location that has the highest accuracy on scans
	Location newLocation = localization.GetNextPrefferedLocation(scan, currentLocation);

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

	// Saving the map image
	map.saveMapImage(str, particelsPixels, width, height);

	// Set new Location
	robot.SetVirtualLocation(newLocation);
	localization.RandomizeParticles(currentLocation);
	}
	return 0;

}
