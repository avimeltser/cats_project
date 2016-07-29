/*
 * ReadFileData.h
 *
 *  Created on: July 26, 2016
 *      Authors: Yakir Kadkoda   	  203550546,
 *      		 Daniel Roitenberg    308154558,
 *      		 Avi Meltser   		  307929182
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
