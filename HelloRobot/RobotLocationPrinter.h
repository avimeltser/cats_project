/*
 * RobotLocationPrinter.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef ROBOTLOCATIONPRINTER_H_
#define ROBOTLOCATIONPRINTER_H_

#define ROBOT_PIXEL_SIZE 20
#define R_COLOR 20
#define G_COLOR 20
#define B_COLOR 20
#define A_COLOR 255

#include <vector>
#include <unistd.h>
#include "Globals.h"
using namespace std;

class RobotLocationPrinter {
public:
	RobotLocationPrinter();
	void PrintInConsole(Location location);
	vector<unsigned char>  PrintIntoPixelArray(float location[3], vector<unsigned char> pixels,int width, int height, float MapResolution);
	virtual ~RobotLocationPrinter();
};

#endif /* ROBOTLOCATIONPRINTER_H_ */
