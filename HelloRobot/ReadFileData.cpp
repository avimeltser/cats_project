/*
 * ReadFileData.cpp
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#include "ReadFileData.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ReadFileData::ReadFileData() {
	// TODO Auto-generated constructor stub

}
const char* ReadFileData::FromPath(char* path){
	  string text = "";
	  string line;
	  ifstream myfile (path);
	  if (myfile.is_open())
	  {
	    while ( getline (myfile,line) )
	    {
	      text += line + '\n';
	    }
	    myfile.close();
	  }

	  else cout << "Unable to open file";

	  return text.c_str();
}
ReadFileData::~ReadFileData() {
	// TODO Auto-generated destructor stub
}

