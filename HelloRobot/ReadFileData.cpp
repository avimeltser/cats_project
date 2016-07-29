/*
 * ReadFileData.cpp
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
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

