/*
 * RobotLocationPrinter.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
 */

#include "RobotLocationPrinter.h"
#include <iostream>
#include "math.h"
using namespace std;

RobotLocationPrinter::RobotLocationPrinter() {
	// TODO Auto-generated constructor stub

}

void RobotLocationPrinter::PrintInConsole(Location location)
{
	cout << "Location: \n{\n   X: " << location.x << "\n   Y: " << location.y << "\n   rotation: " << location.yaw << "\n}\n" << endl;
}

vector<unsigned char>  RobotLocationPrinter::PrintIntoPixelArray(float location[3], vector<unsigned char> pixels, int width, int height, float MapResolution)
{
	vector<unsigned char> pixelsCopy = vector<unsigned char>(pixels);
	float locationX = location[0];
	float locationY = location[1];
	int indexX = (int)(locationX/MapResolution) + width/2;
	int indexY = (int)(locationY/MapResolution) + height/2;
	int robotRadius = ROBOT_PIXEL_SIZE / 2;
	for(int x = indexX - robotRadius -1; x < indexX + robotRadius + 1; x++)
	{
		for(int y = indexY - robotRadius -1; y < indexY + robotRadius + 1; y++)
		{
			float distance = sqrt(pow(x - indexX, 2) + pow(y - indexY, 2));
			float deltaInAbs = sqrt(pow(distance - robotRadius, 2));
			if(deltaInAbs < 2) // if the pixels are near by 2 pixels
			{
				int indexInPixelsArray = (y * width + x) * 4; // convert from matrix to array and jump colors (*4)
				pixelsCopy[indexInPixelsArray] = R_COLOR;
				pixelsCopy[indexInPixelsArray + 1] = G_COLOR;
				pixelsCopy[indexInPixelsArray + 2] = B_COLOR;
				pixelsCopy[indexInPixelsArray + 3] = A_COLOR;
			}
		}
	}
	return pixelsCopy;
}

RobotLocationPrinter::~RobotLocationPrinter() {
	// TODO Auto-generated destructor stub
}

