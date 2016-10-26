/*
===============================================================================

  FILE:  LASreader.h
  
  CONTENTS:
  
    Reads LIDAR points from the LAS format (Version 1.x , April 29, 2008).

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    18 February 2007 -- created after repairing 2 vacuum cleaners in the garden
  
===============================================================================
*/
#ifndef LAS_READER_H
#define LAS_READER_H

//#include <stdio.h>

#include "lastools/lasdefinitions.h"
#include "lastools/laspointreader.h"

#include "orsBase/orsTypedef.h"

// #include "orsPointStruDef.h"

#include "orsPointCloud/orsIPointCloudReader.h"



class LASreader : public orsIPointCloudReader, public orsObjectBase
{
public:
	LASheader header;
	
	LASpoint point;
	double gps_time;
	unsigned short rgb[3];
	
	bool points_have_gps_time;
	bool points_have_rgb;
	
	int npoints;
	int p_count;
	
	int additional_bytes_per_point;

//	orsPointAttributionFlag  m_attFlag;

public:
	LASreader(bool bForRegister);
	virtual ~LASreader();
	
	virtual bool open( const char *lasFileName, bool bShared = true);

	// 取支持存储的内容
// 	virtual ors_uint32 getSupportedContents() const
// 	{
// 		return m_ptContent;
// 	}
	
	virtual ors_uint32 get_point_contentMask()  const
	{
		return m_ptContent;
	}

	virtual void close();
	
	virtual void reopen();	//重新从第一个点记录开始读取
	
	virtual bool set_readpos(int ptNum = 0);
	virtual bool set_readpos(ors_int64  offset);
	
	//	virtual bool read_point() = 0;
	//	virtual bool read_point(float* coordinates);
//	virtual void* get_header();
	virtual void  get_offset(double offset[3])
	{
		offset[0] = header.x_offset;
		offset[1] = header.y_offset;
		offset[2] = header.z_offset;
	}

	virtual void  get_scale_factor(double scale[3])
	{
		scale[0] = header.x_scale_factor;
		scale[1] = header.y_scale_factor;
		scale[2] = header.z_scale_factor;
	}

	virtual ors_int32 get_number_of_points()
	{
		return header.number_of_point_records;
	}

	virtual void  get_boundary(orsPOINT3D *bbmax, orsPOINT3D *bbmin)
	{
		bbmax->X=header.max_x;	bbmin->X=header.min_x;
		bbmax->Y=header.max_y;	bbmin->Y=header.min_y;
		bbmax->Z=header.max_z;	bbmin->Z=header.min_z;
	}

// 	virtual __int32 get_point_record_length()
// 	{
// 		return header.point_data_record_length;
// 	}

//	virtual void set_attribution_flag(orsPointAttributionFlag flag);
	virtual bool read_point(double coordinates[3]) ;
	virtual bool read_rawpoint(int coordinates[3] );
//	virtual bool read_point(void* pData);
//	virtual bool read_point(LASpointXYZI* pt);

	virtual unsigned short get_intensity();
 	virtual unsigned char get_classification();
	virtual double get_gpstime();
	virtual void get_rgb(unsigned short Rgb[3]);
	virtual int get_return_number();
	virtual int get_number_of_returns();
	virtual bool get_point_observed_info(orsPointObservedInfo* info) {return false;};

	virtual unsigned char get_scan_direction_flag();
	virtual unsigned char get_edge_of_flight_line();
	virtual char get_scan_angle_rank();
	virtual unsigned char get_user_data();
	virtual unsigned short get_point_source_ID();

private:
	bool read_point();
	bool read_point(float* coordinates);
//	bool read_point(double* coordinates);
//	bool read_point(LASpointXYZI* pt);
//	virtual bool read_point(LDI_POINT* pt);

	void get_coordinates(float* coordinates);
	void get_coordinates(double* coordinates);
// 	void get_intensity(unsigned short* intensity);
// 	void get_classification(unsigned char* cls);
//	void get_return_number(unsigned char* rtnum);
	
	

//	void get_rgb(unsigned short Rgb[3]);
		
private:

	void *m_file;

	LASpointReader* pointReader;

	ors_uint32	m_ptContent;

public:
	ORS_OBJECT_IMP1( LASreader, orsIPointCloudReader, "las", "las" );
};

#endif
