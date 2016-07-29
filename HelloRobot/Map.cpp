/*
 * Map.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include "Map.h"
#include "lodepng.h"
#include <unistd.h>
#include <iostream>
#include "math.h"
using namespace std;

Map::Map(){

}

Map::Map(const char* mapFile, double mapResolution, double gridResolution,int robotSize ,Location start, Location goal){
	this->mapResolution = mapResolution;
	this->gridResolution = gridResolution;
	this->robotSize = robotSize;
	this->start = start;
	this->goal = goal;

	LoadMap(mapFile);
	InflateObstacle();
	
	// For debug
	saveMapImage("roboticLabMapInflated.png", pixels, width, height);
	CreateGridFromMap();
}
void Map::InflateObstacle(){
	double robotSizeInPixels = robotSize / mapResolution;
	int inflationRadius = robotSizeInPixels * 0.65;
	vector< vector<bool> > inflatedMap;

	inflatedMap.resize(height);
	for(int i=0; i < height; i++)
	{
		inflatedMap[i].resize(width);
	}

	for(int i=0 ; i< height ; i++)
	{
		for(int j=0 ; j< width ; j ++)
		{
			if(map[i][j] == 1)
			{
					for(int ii=-inflationRadius; ii < inflationRadius ; ii++)
					{
						for(int jj=-inflationRadius; jj < inflationRadius ; jj++)
						{
							if(ii + i >= 0 && ii +i < height && jj + j >= 0 && jj + j < width)
							inflatedMap[i + ii][j + jj] = 1;
						}
					}
			}
		}
	}

	map = inflatedMap;

	// For debug use only
	int cells = 0;
	for(int i=0 ; i < height ; i++)
	{
		for(int j=0 ; j < width ; j ++)
		{
			if(map[i][j] == 1)
			{
				pixels[cells] = 0;
				pixels[cells+1] = 0;
				pixels[cells+2] = 0;
				pixels[cells+3] = 255;
			}
			else
			{
				pixels[cells] = 255;
				pixels[cells+1] = 255;
				pixels[cells+2] = 255;
				pixels[cells+3] = 255;
			}
			cells += 4;
		}
	}
}

void Map::CreateGridFromMap(){
	// Calculate grid size
	int gridHeight = (height * mapResolution) / gridResolution;
	int gridWidth = (width * mapResolution) / gridResolution;

	vector< vector<bool> > mapGrid;

	// Initiate grid
	mapGrid.resize(gridHeight);
	for (int i = 0; i < gridHeight; i++) {
		mapGrid[i].resize(gridWidth);
	}

	int gridToMapRatio = gridResolution / mapResolution;

	for(int i = 0; i < gridHeight; i++){
		for(int j = 0; j < gridWidth; j++){
			bool foundObstacle = false;

			for(int y = i * gridToMapRatio ; y < i * gridToMapRatio + gridToMapRatio; y++)
			{
				if(y >= 0 && y < height && !foundObstacle)
				{
					for(int x = j * gridToMapRatio; x < j * gridToMapRatio + gridToMapRatio; x++)
					{
						if(x >= 0 && x < width)
						{
							if(this->map[y][x] == 1)
							{
								foundObstacle = true;
								break;
							}
						}
					}
				}
			}

			mapGrid[i][j] = foundObstacle;
		}
	}
	int startX = (start.x * mapResolution) / gridResolution;
	int startY = (start.y * mapResolution) / gridResolution;
	Location gridStartLocation;
	gridStartLocation.x = startX;
	gridStartLocation.y = startY;
	gridStartLocation.yaw = start.yaw;

	int goalX = (goal.x * mapResolution) / gridResolution;
	int goalY = (goal.y * mapResolution) / gridResolution;

	Location gridGoalLocation;
	gridGoalLocation.x = goalX;
	gridGoalLocation.y = goalY;

	this->grid = Grid(mapGrid, gridResolution, mapResolution, gridHeight, gridWidth, gridStartLocation, gridGoalLocation);
}

void Map::saveMapImage(const char* saveLocation,vector<unsigned char> picture, unsigned w, unsigned h){
	lodepng::encode(saveLocation,picture, w, h);
}

void Map::LoadMapImage(const char* mapFile,vector<unsigned char>& out, unsigned& w, unsigned& h)
{
	lodepng::decode(out, w, h, mapFile);
}

void Map::LoadMap(const char* mapFile){
	lodepng::decode(pixels, width, height, mapFile);
	map.resize(height);
	for(int i=0; i < height; i++)
	{
		map[i].resize(width);
	}

	for(int i=0; i < height; i++){
		for(int j=0; j < width; j++){
			if(pixels[((i * width +j)* 4)] == 0){
				map[i][j] = 1;
			}else{
				map[i][j] = 0;
			}
		}
	}
}

int Map::GetHeight(){
	return height;
}

int Map::GetWidth(){
	return width;
}

void Map::Print()
{
	for(int i=0; i < height; i++){
		for(int j=0; j < width; j++){
			if(map[i][j] == 0){
				cout << " ";
			}else{
				cout << map[i][j];
			}
		}
		cout << endl;
	}
}

void Map::PrintGrid()
{
int gridHeight = grid.GetGridHeight();
int gridWidth = grid.GetGridWidth();
	for(int i=0; i < gridHeight; i++){
		for(int j=0; j < gridWidth; j++){
			if(grid.GetGrid()[i][j] == 0){
				cout << " ";
			}else{
				cout << grid.GetGrid()[i][j];
			}
		}
		cout << endl;
	}
}
