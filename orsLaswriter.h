/*
===============================================================================

  FILE:  LASwriter.h
  
  CONTENTS:
  
    Writes LIDAR points to the LAS format (Version 1.x , April 29, 2008).

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    21 February 2007 -- created after eating Sarah's veggies with peanutsauce
  
===============================================================================
*/
#ifndef LAS_WRITER_H
#define LAS_WRITER_H

#include "lastools/lasdefinitions.h"
#include "lastools/laspointwriter.h"

#include <stdio.h>

#include "orsBase/orsTypedef.h"
#include "orsPointCloud/orsIPointCloudWriter.h"

// enum lasDataFORMAT	{
// 	lasPOINT = 0,
// 	lasPOINT_GPS = 1,
// 	lasPOINT_RGB = 2,
// 	lasPOINT_GPS_RGB = 3
// };


class LASwriter : public orsIPointCloudWriter, public orsObjectBase
{
public:
	int npoints;
	int p_count;
	
public:
	LASwriter(bool bForRegister);
	virtual ~LASwriter();
	
	// 取文件格式名
	virtual orsString get_file_format()	const
	{
		return _T("las");
	}
	
	// 取支持存储的内容
	virtual ors_uint32 getSupportedContents() const
	{
		return ORS_PCM_XYZ | ORS_PCM_INTENSITY | ORS_PCM_CLASS | ORS_PCM_GPSTIME | ORS_PCM_RGB	| ORS_PCM_NUM_RETURNS;
		// ORS_PCM_OBSINFO
	}

	virtual bool open(const char *lasFileName, lasDataFORMAT point_data_format=lasPOINT,
		double xScale=0, double yScale=0, double zScale=0);

// 	virtual bool open(const char *lasFileName, lasDataFORMAT point_data_format,  
// 		double scale[3], double offset[3]) { return false; }; //此函数在ATN中有特殊意义，这里暂不实现 2014.6.13
	
	virtual void close();

	virtual bool set_writepos(ors_int64  offset);

	virtual bool write_point(double X, double Y, double Z);
	virtual bool write_point(double X, double Y, double Z, unsigned short r, unsigned short g, unsigned short b )
	{
		return write_point( X,Y,Z, 0, r, g, b);
	}

	virtual bool write_point(double* coordinates) 
	{
		return write_point( coordinates[0], coordinates[1], coordinates[2] );
	}

	virtual bool write_point(double* coordinates, unsigned short intensity, unsigned short* rgb = NULL ) ;
	
	
	virtual bool write_point(double* coordinates, unsigned short intensity,
		unsigned char return_number, unsigned char number_of_returns_of_given_pulse,
		unsigned char scan_direction_flag, unsigned char edge_of_flight_line,
		unsigned char classification, 
		char scan_angle_rank, unsigned char user_data,
		unsigned short point_source_ID, double gps_time = 0, unsigned short* rgb = 0);
	
	virtual bool write_point(double* coordinates, unsigned short intensity,
		unsigned char return_number, unsigned char number_of_returns_of_given_pulse,
		unsigned char scan_direction_flag, unsigned char edge_of_flight_line,
		unsigned char classification, 
		char scan_angle_rank, unsigned char user_data,
		unsigned short point_source_ID, double gps_time, orsPointObservedInfo *info,
		unsigned short* rgb = 0)
	{
		return false;
	}
	
	//  bool write_point(double* coordinates);
	
private:
	bool write_point(double* coordinates, double gps_time, unsigned short* rgb = 0);
	
	//  bool write_point(double x, double y, double z, unsigned short intensity = 0, unsigned short* rgb = NULL );
	
	bool write_point(double x, double y, double z, double gps_time, unsigned short r, unsigned short g, unsigned short b);
	
private:  
	void ModifyHeader( double x, double y, double z );
	void SetXyz( LASpoint *point, double x, double y, double z );

	bool write_point(LASpoint* point, double gps_time, unsigned short* rgb)
	{
		p_count++;
		return pointWriter->write_point(point, gps_time, rgb);
	}
	
private:
	void *m_file;
	
	LASpointWriter* pointWriter;
	
	LASheader* header;
	
	bool created_header;
	
	
public:
	ORS_OBJECT_IMP1( LASwriter, orsIPointCloudWriter, "las", "las" );
};

#endif
