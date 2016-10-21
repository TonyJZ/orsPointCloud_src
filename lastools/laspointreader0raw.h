/*
===============================================================================

  FILE:  laspointreader0raw.h
  
  CONTENTS:
  
    Reads a point of type 0 (without gps_time) in standard LAS format 1.1

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created about an hour before henna's 32nd birthday
  
===============================================================================
*/
#ifndef LAS_POINT_READER_0RAW_H
#define LAS_POINT_READER_0RAW_H

#include "laspointreader.h"

#include <stdio.h>

#ifdef _WIN32

#include "ifile.h"

#define myFILE IFILE
#define fopen ifopen
#define fread ifread
#define fseek ifseek

#else

#define myFILE FILE

#endif


class LASpointReader0raw : public LASpointReader
{
public:
	inline bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0)
	{
		return (fread(point, sizeof(LASpoint), 1, file) == 1);
	};

	LASpointReader0raw( myFILE* file ){this->file = file;};
	~LASpointReader0raw(){};

private:

	myFILE* file;
};

#endif
