/*
 * Grid.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef GRID_H_
#define GRID_H_

#include <vector>
#include "Globals.h"

using namespace std;

class Grid {
private:
	double gridResolution;
	double mapResolution;
	vector< vector<bool> > grid;
	int gridWidth;
	int gridHeight;
	Location startLocation;
	Location goalLocation;

public:
	Grid();
	Grid(vector< vector<bool> > grid, double resolution, double mapResolution, double height, double width, Location start, Location goal);
	int GetGridHeight();
	int GetGridWidth();
	vector< vector<bool> > GetGrid();
	double GetGridResolution();
	double GetMapResolution();
	Location GetGridStartLocation();
	Location GetGridGoalLocation();
	virtual ~Grid();
};

#endif /* GRID_H_ */
