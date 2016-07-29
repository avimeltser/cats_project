/*
 * ReadFileData.h
 *
 *  Created on: June 15, 2016
 *      Authors: Bar   Miliavsky 205432099,
 *      		 Mor   Tal       312496060,
 *      		 Nadav Kaner     205785645
 */

#ifndef READFILEDATA_H_
#define READFILEDATA_H_

class ReadFileData {
public:
	ReadFileData();
	const char* FromPath(char* path);
	virtual ~ReadFileData();
};

#endif /* READFILEDATA_H_ */
