/*
===============================================================================

  FILE:  laspointreader2raw.h
  
  CONTENTS:
  
    Reads a point of type 2 (without gps_time but with rgb color) in LAS format 1.2

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_READER_2RAW_H
#define LAS_POINT_READER_2RAW_H

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

#include "laspointreader.h"

class LASpointReader2raw : public LASpointReader
{
public:
	inline bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0)
	{
		fread(point, sizeof(LASpoint), 1, file);
		return (fread(rgb, sizeof(unsigned short), 3, file) == 3);
	};
	LASpointReader2raw( myFILE* file){this->file = file;};
	~LASpointReader2raw(){};
private:
	myFILE* file;
};

#endif
