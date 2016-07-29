/*
 * Grid.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "Grid.h"

using namespace std;
Grid::Grid(){

}

Grid::Grid(vector< vector<bool> > grid, double gridResolution, double mapResolution,double height, double width, Location start, Location goal) {
	this->grid = grid;
	this->gridResolution = gridResolution;
	this->mapResolution = mapResolution;
	this->gridHeight = height;
	this->gridWidth = width;
	this->startLocation = start;
	this->goalLocation = goal;
}

int Grid::GetGridHeight(){
	return gridHeight;
}

int Grid::GetGridWidth(){
	return gridWidth;
}

vector< vector<bool> > Grid::GetGrid(){
	return grid;
}

double Grid::GetGridResolution(){
	return gridResolution;
}

double Grid::GetMapResolution(){
	return mapResolution;
}

Location Grid::GetGridStartLocation(){
	return startLocation;
}

Location Grid::GetGridGoalLocation(){
	return goalLocation;
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}
