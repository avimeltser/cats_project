/*
 * Map.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <unistd.h>
#include "Grid.h"
#include "Globals.h"
using namespace std;

class Map {
private:
		vector<unsigned char> pixels;
		unsigned int width;
		unsigned int height;
		vector< vector<bool> > map;
		double mapResolution;
		double gridResolution;
		unsigned int robotSize;
		Location start;
		Location goal;
		void InflateObstacle();
		void CreateGridFromMap();
		void LoadMap(const char* mapFile);
public:
	Map();
	Map(const char* mapFile, double mapResolution, double gridResolution, int robotSize, Location start, Location goal);
	Grid grid;
	void saveMapImage(const char* saveLocation, vector<unsigned char> picture, unsigned w, unsigned h);
	void LoadMapImage(const char* mapFile,vector<unsigned char>& out, unsigned& w, unsigned& h);
	int GetHeight();
	int GetWidth();
	Grid GetGrid();
	void Print();
	void PrintGrid();

};

#endif /* MAP_H_ */
