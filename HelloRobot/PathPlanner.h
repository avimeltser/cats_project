/*
 * PathPlanner.h
 *
 *   Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

using namespace std;
#include <string>
#include <vector>
#include "Grid.h"
#include "lodepng.h"

// map of directions
const int dirNum=8; // number of possible directions to go at any position
static int dirX[dirNum]={1, 1, 0, -1, -1, -1, 0, 1};
static int dirY[dirNum]={0, 1, 1, 1, 0, -1, -1, -1};

class PathPlanner {
public:
	string AStarPathFind( const int nRowStart, const int nColStart, const int nRowFinish, const int nColFinish, vector< vector<bool> > GridMap, const int Hight, const int Width );
	void PrintPath(Grid grid, string route);
};

#endif
