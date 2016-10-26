/*
===============================================================================

  FILE:  lasreader.cpp
  
	CONTENTS:
	
	  see corresponding header file
	  
		PROGRAMMERS:
		
		  martin isenburg@cs.unc.edu
		  
			COPYRIGHT:
			
			  copyright (C) 2007-2008  martin isenburg@cs.unc.edu
			  
				This software is distributed in the hope that it will be useful,
				but WITHOUT ANY WARRANTY; without even the implied warranty of
				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
				
				  CHANGE HISTORY:
				  
					see corresponding header file
					
					  ===============================================================================
*/
#include "StdAfx.h"
#include "orslasreader.h"

#define ENABLE_LAS_COMPRESSION_SUPPORT
#undef ENABLE_LAS_COMPRESSION_SUPPORT

#include "lastools/laspointreader0raw.h"
#include "lastools/laspointreader1raw.h"
#include "lastools/laspointreader2raw.h"
#include "lastools/laspointreader3raw.h"

#ifdef ENABLE_LAS_COMPRESSION_SUPPORT
#include "lastools/laspointreader0compressed.h"
#include "lastools/laspointreader1compressed.h"
#include "lastools/laspointreader2compressed.h"
#include "lastools/laspointreader3compressed.h"
#endif // ENABLE_LAS_COMPRESSION_SUPPORT

#include <assert.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif


#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include "lastools/ifile.h"

#define myFILE IFILE
#define fopen ifopen
#define fread ifread
#define fseek ifseek

#define fclose ifclose

#else

#define myFILE FILE

unsigned   long   get_file_length(  FILE*   file_ptr  ) 
{  
	unsigned   long   PosCur      =   0;  
	unsigned   long   PosBegin    =   0;  
	unsigned   long   PosEnd      =   0;  
	
	if(  NULL ==  file_ptr   )  
	{  
		return   0;  
	}  
	
	PosCur   =   ftell(   file_ptr   );  //得到文件指针的当前位置
	fseek(   file_ptr,   0L,   SEEK_SET   );  //将文件指针移到文件开始
	PosBegin   =   ftell(   file_ptr   );  //得到当前文件的位置，也就是文件头的位置
	fseek(   file_ptr,   0L,   SEEK_END   );  //将文件指针移到文件末尾
	PosEnd   =   ftell(   file_ptr   );  //得到文件的末尾位置
	fseek(   file_ptr,   PosCur,   SEEK_SET   );  //将指针移到文件的原来位置
	
	return   PosEnd   -   PosBegin;          //返回文件大小
}

#endif


bool LASreader::open( const char *lasFileName, bool bShared)
{
	bool skip_all_headers = false;
	myFILE *file = NULL;

	close();

	if( bShared )	
		file = fopen( lasFileName, "rbs");
	else
		file = fopen( lasFileName, "rb");
	
	if( NULL == file )
	{
		fprintf(stderr,"ERROR: file pointer is zero\n");
		return false;
	}
	
	
	m_file = file;
	
	// clean the header
	header.clean();
	
	// read the header variable after variable (to avoid alignment issues)
	
	if (fread(&(header.file_signature), sizeof(char), 4, file) != 4)
	{
		fprintf(stderr,"ERROR: reading header.file_signature\n");
		return false;
	}
	if (fread(&(header.file_source_id), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.file_source_id\n");
		return false;
	}
	if (fread(&(header.global_encoding), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.global_encoding\n");
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_1), sizeof(unsigned int), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_1\n");
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_2), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_2\n");
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_3), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_3\n");
		return false;
	}
	if (fread(&(header.project_ID_GUID_data_4), sizeof(char), 8, file) != 8)
	{
		fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_4\n");
		return false;
	}
	if (fread(&(header.version_major), sizeof(char), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.version_major\n");
		return false;
	}
	if (fread(&(header.version_minor), sizeof(char), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.version_minor\n");
		return false;
	}
	if (fread(&(header.system_identifier), sizeof(char), 32, file) != 32)
	{
		fprintf(stderr,"ERROR: reading header.system_identifier\n");
		return false;
	}
	if (fread(&(header.generating_software), sizeof(char), 32, file) != 32)
	{
		fprintf(stderr,"ERROR: reading header.generating_software\n");
		return false;
	}
	if (fread(&(header.file_creation_day), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.file_creation_day\n");
		return false;
	}
	if (fread(&(header.file_creation_year), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.file_creation_year\n");
		return false;
	}
	if (fread(&(header.header_size), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.header_size\n");
		return false;
	}
	if (fread(&(header.offset_to_point_data), sizeof(unsigned int), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.offset_to_point_data\n");
		return false;
	}
	if (fread(&(header.number_of_variable_length_records), sizeof(unsigned int), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.number_of_variable_length_records\n");
		return false;
	}
	if (fread(&(header.point_data_format), sizeof(unsigned char), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.point_data_format\n");
		return false;
	}
	if (fread(&(header.point_data_record_length), sizeof(unsigned short), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.point_data_record_length\n");
		return false;
	}
	if (fread(&(header.number_of_point_records), sizeof(unsigned int), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.number_of_point_records\n");
		return false;
	}
	if (fread(&(header.number_of_points_by_return), sizeof(unsigned int), 5, file) != 5)
	{
		fprintf(stderr,"ERROR: reading header.number_of_points_by_return\n");
		return false;
	}
	if (fread(&(header.x_scale_factor), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.x_scale_factor\n");
		return false;
	}
	if (fread(&(header.y_scale_factor), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.y_scale_factor\n");
		return false;
	}
	if (fread(&(header.z_scale_factor), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.z_scale_factor\n");
		return false;
	}
	if (fread(&(header.x_offset), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.x_offset\n");
		return false;
	}
	if (fread(&(header.y_offset), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.y_offset\n");
		return false;
	}
	if (fread(&(header.z_offset), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.z_offset\n");
		return false;
	}
	if (fread(&(header.max_x), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.max_x\n");
		return false;
	}
	if (fread(&(header.min_x), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.min_x\n");
		return false;
	}
	if (fread(&(header.max_y), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.max_y\n");
		return false;
	}
	if (fread(&(header.min_y), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.min_y\n");
		return false;
	}
	if (fread(&(header.max_z), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.max_z\n");
		return false;
	}
	if (fread(&(header.min_z), sizeof(double), 1, file) != 1)
	{
		fprintf(stderr,"ERROR: reading header.min_z\n");
		return false;
	}
	
	// check header contents
	
	if (strncmp(header.file_signature, "LASF", 4) != 0)
	{
		fprintf(stderr,"ERROR: wrong file signature '%s'\n", header.file_signature);
		return false;
	}
	if ((header.version_major != 1) || ((header.version_minor != 0) && (header.version_minor != 1) && (header.version_minor != 2)))
	{
		fprintf(stderr,"WARNING: unknown version %d.%d (should be 1.0 or 1.1 or 1.2)\n", header.version_major, header.version_minor);
	}
	if (header.header_size < 227)
	{
		fprintf(stderr,"WARNING: header size is %d but should be at least 227\n", header.header_size);
	}
	if (header.offset_to_point_data < header.header_size)
	{
		fprintf(stderr,"ERROR: offset to point data %d is smaller than header size %d\n", header.offset_to_point_data, header.header_size);
		return false;
	}
	
	if (header.number_of_point_records <= 0)
	{
		fprintf(stderr,"WARNING: number of point records is %d\n", header.number_of_point_records);
	}
	
	if ((header.point_data_format & 127) == 0)
	{
		if (header.point_data_record_length != 20)
		{
			fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header.point_data_record_length);
			if (header.point_data_record_length > 20)
			{
				additional_bytes_per_point = header.point_data_record_length - 20;
				fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
			}
			header.point_data_record_length = 20;
		}
	}
	else if ((header.point_data_format & 127) == 1)
	{
		if (header.point_data_record_length != 28)
		{
			fprintf(stderr,"WARNING: wrong point data record length of %d instead of 28 for format 1\n", header.point_data_record_length);
			if (header.point_data_record_length > 28)
			{
				additional_bytes_per_point = header.point_data_record_length - 28;
				fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
			}
			header.point_data_record_length = 28;
		}
	}
	else if ((header.point_data_format & 127) == 2)
	{
		if (header.point_data_record_length != 26)
		{
			fprintf(stderr,"WARNING: wrong point data record length of %d instead of 26 for format 2\n", header.point_data_record_length);
			if (header.point_data_record_length > 26)
			{
				additional_bytes_per_point = header.point_data_record_length - 26;
				fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
			}
			header.point_data_record_length = 26;
		}
	}
	else if ((header.point_data_format & 127) == 3)
	{
		if (header.point_data_record_length != 34)
		{
			fprintf(stderr,"WARNING: wrong point data record length of %d instead of 34 for format 3\n", header.point_data_record_length);
			if (header.point_data_record_length > 34)
			{
				additional_bytes_per_point = header.point_data_record_length - 34;
				fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
			}
			header.point_data_record_length = 34;
		}
	}
	else
	{
		fprintf(stderr,"WARNING: unknown point data format %d ... assuming format 0\n", header.point_data_format);
		if (header.point_data_record_length != 20)
		{
			fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header.point_data_record_length);
			if (header.point_data_record_length > 20)
			{
				additional_bytes_per_point = header.point_data_record_length - 20;
				fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
			}
			header.point_data_record_length = 20;
			return false;
		}
		header.point_data_format = 0;
	}
	if (header.x_scale_factor == 0 || header.y_scale_factor == 0 || header.z_scale_factor == 0)
	{
		fprintf(stderr,"WARNING: some scale factors are zero %g %g %g. will set them to 0.01.\n", header.x_scale_factor, header.y_scale_factor, header.z_scale_factor);
		if (header.x_scale_factor == 0) header.x_scale_factor = 0.01;
		if (header.y_scale_factor == 0) header.y_scale_factor = 0.01;
		if (header.z_scale_factor == 0) header.z_scale_factor = 0.01;
	}
	if (header.max_x < header.min_x || header.max_y < header.min_y || header.max_z < header.min_z)
	{
		fprintf(stderr,"WARNING: invalid bounding box [ %g %g %g / %g %g %g ]\n", header.min_x, header.min_y, header.min_z, header.max_x, header.max_y, header.max_z);
	}
	
	/// repair number of data records
	if( 0 == header.number_of_point_records  )	{
		unsigned long fileSize = get_file_length(file);
		
		header.number_of_point_records = ( fileSize - header.offset_to_point_data ) / header.point_data_record_length;
		
		fprintf(stderr, "estimated number_of_point_records from file size: %d\n", header.number_of_point_records );
	}
	
	// load any number of user-defined bytes that might have been added to the header
	header.user_data_in_header_size = header.header_size - 227;
	if (header.user_data_in_header_size)
	{
		header.user_data_in_header = new char[header.user_data_in_header_size];
		
		if (fread(header.user_data_in_header, sizeof(char), header.user_data_in_header_size, file) != (unsigned int)header.user_data_in_header_size)
		{
			fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_in_header\n", header.user_data_in_header_size);
			return false;
		}
	}
	
	// create the right point reader in dependance on compression and point data format
	
	if (header.point_data_format & 128)
	{
#ifdef ENABLE_LAS_COMPRESSION_SUPPORT
		// change the format to uncompressed
		header.point_data_format &= 127;
		switch (header.point_data_format)
		{
		case 0:
			pointReader = new LASpointReader0compressed(file);
			points_have_gps_time = false;
			points_have_rgb = false;
			break;
		case 1:
			pointReader = new LASpointReader1compressed(file);
			points_have_gps_time = true;
			points_have_rgb = false;
			break;
		case 2:
			pointReader = new LASpointReader2compressed(file);
			points_have_gps_time = false;
			points_have_rgb = true;
			break;
		case 3:
			pointReader = new LASpointReader3compressed(file);
			points_have_gps_time = true;
			points_have_rgb = true;
			break;
		}
#else // ENABLE_LAS_COMPRESSION_SUPPORT
		fprintf(stderr,"ERROR: this version of the lasreader does not support compression\n");
		return false;
#endif // ENABLE_LAS_COMPRESSION_SUPPORT
	}
	else
	{
		switch (header.point_data_format)
		{
		case 0:
			pointReader = new LASpointReader0raw(file);
			points_have_gps_time = false;
			points_have_rgb = false;
			m_ptContent = ORS_PCM_XYZ | ORS_PCM_INTENSITY | ORS_PCM_CLASS | ORS_PCM_NUM_RETURNS;
			break;
		case 1:
			pointReader = new LASpointReader1raw(file);
			points_have_gps_time = true;
			points_have_rgb = false;
			m_ptContent = ORS_PCM_XYZ | ORS_PCM_INTENSITY | ORS_PCM_CLASS | ORS_PCM_NUM_RETURNS | ORS_PCM_GPSTIME;
			break;
		case 2:
			pointReader = new LASpointReader2raw(file);
			points_have_gps_time = false;
			points_have_rgb = true;
			m_ptContent = ORS_PCM_XYZ | ORS_PCM_INTENSITY | ORS_PCM_CLASS | ORS_PCM_NUM_RETURNS | ORS_PCM_RGB;
			break;
		case 3:
			pointReader = new LASpointReader3raw(file);
			points_have_gps_time = true;
			points_have_rgb = true;
			m_ptContent = ORS_PCM_XYZ | ORS_PCM_INTENSITY | ORS_PCM_CLASS | ORS_PCM_NUM_RETURNS | ORS_PCM_GPSTIME | ORS_PCM_RGB;
			break;
		}
	}
	
	if( skip_all_headers )
	{
#ifdef WIN32
		fseek( (myFILE*)m_file, header.offset_to_point_data, SEEK_SET );
#else
		int i;
		for(i = header.header_size; i < (int)header.offset_to_point_data; i++) 
			fgetc(file);
#endif
	}
	else
	{
		int vlrs_size = 0;
		
		// read the variable length records into the header
		
		if (header.number_of_variable_length_records)
		{
			header.vlrs = new LASvlr[header.number_of_variable_length_records];
			
			int i;
			for (i = 0; i < (int)header.number_of_variable_length_records; i++)
			{
				// make sure there are enough bytes left to read a variable length record before the point block starts
				if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < 54)
				{
					fprintf(stderr,"WARNING: only %d bytes until point block after reading %d of %d vlrs. skipping remaining vlrs ...\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, i, header.number_of_variable_length_records);
					header.number_of_variable_length_records = i;
					break;
				}
				
				// read variable length records variable after variable (to avoid alignment issues)
				
				if (fread(&(header.vlrs[i].reserved), sizeof(unsigned short), 1, file) != 1)
				{
					fprintf(stderr,"ERROR: reading header.vlrs[%d].reserved\n", i);
					return false;
				}
				if (fread(header.vlrs[i].user_id, sizeof(char), 16, file) != 16)
				{
					fprintf(stderr,"ERROR: reading header.vlrs[%d].user_id\n", i);
					return false;
				}
				if (fread(&(header.vlrs[i].record_id), sizeof(unsigned short), 1, file) != 1)
				{
					fprintf(stderr,"ERROR: reading header.vlrs[%d].record_id\n", i);
					return false;
				}
				if (fread(&(header.vlrs[i].record_length_after_header), sizeof(unsigned short), 1, file) != 1)
				{
					fprintf(stderr,"ERROR: reading header.vlrs[%d].record_length_after_header\n", i);
					return false;
				}
				if (fread(header.vlrs[i].description, sizeof(char), 32, file) != 32)
				{
					fprintf(stderr,"ERROR: reading header.vlrs[%d].description\n", i);
					return false;
				}
				
				// keep track on the number of bytes we have read so far
				
				vlrs_size += 54;
				
				// check variable length record contents
				
				if (header.vlrs[i].reserved != 0xAABB)
				{
					fprintf(stderr,"WARNING: wrong header.vlrs[%d].reserved: %d != 0xAABB\n", i, header.vlrs[i].reserved);
				}
				
				// make sure there are enough bytes left to read the data of the variable length record before the point block starts
				
				if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < header.vlrs[i].record_length_after_header)
				{
					fprintf(stderr,"WARNING: only %d bytes until point block when trying to read %d bytes into header.vlrs[%d].data\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, header.vlrs[i].record_length_after_header, i);
					header.vlrs[i].record_length_after_header = (int)header.offset_to_point_data - vlrs_size - header.header_size;
				}
				
				// load data following the header of the variable length record
				
				if (header.vlrs[i].record_length_after_header)
				{
					header.vlrs[i].data = new char[header.vlrs[i].record_length_after_header];
					
					if (fread(header.vlrs[i].data, sizeof(char), header.vlrs[i].record_length_after_header, file) != header.vlrs[i].record_length_after_header)
					{
						fprintf(stderr,"ERROR: reading %d bytes of data into header.vlrs[%d].data\n", header.vlrs[i].record_length_after_header, i);
						return false;
					}
				}
				else
				{
					header.vlrs[i].data = 0;
				}
				
				// keep track on the number of bytes we have read so far
				
				vlrs_size += header.vlrs[i].record_length_after_header;
				
				// special handling for known variable header tags
				
				if (strcmp(header.vlrs[i].user_id, "LASF_Projection") == 0)
				{
					if (header.vlrs[i].record_id == 34735) // GeoKeyDirectoryTag
					{
						if (header.vlr_geo_keys)
						{
							fprintf(stderr,"WARNING: variable length records contain more than one GeoKeyDirectoryTag\n");
						}
						header.vlr_geo_keys = (LASvlr_geo_keys*)header.vlrs[i].data;
						
						// check variable header geo keys contents
						
						if (header.vlr_geo_keys->key_directory_version != 1)
						{
							fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_directory_version: %d != 1\n",header.vlr_geo_keys->key_directory_version);
						}
						if (header.vlr_geo_keys->key_revision != 1)
						{
							fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_revision: %d != 1\n",header.vlr_geo_keys->key_revision);
						}
						if (header.vlr_geo_keys->minor_revision != 0)
						{
							fprintf(stderr,"WARNING: wrong vlr_geo_keys->minor_revision: %d != 0\n",header.vlr_geo_keys->minor_revision);
						}
						header.vlr_geo_key_entries = (LASvlr_key_entry*)&header.vlr_geo_keys[1];
					}
					else if (header.vlrs[i].record_id == 34736) // GeoDoubleParamsTag
					{
						if (header.vlr_geo_double_params)
						{
							fprintf(stderr,"WARNING: variable length records contain more than one GeoDoubleParamsTag\n");
						}
						header.vlr_geo_double_params = (double*)header.vlrs[i].data;
					}
					else if (header.vlrs[i].record_id == 34737) // GeoAsciiParamsTag
					{
						if (header.vlr_geo_ascii_params)
						{
							fprintf(stderr,"WARNING: variable length records contain more than one GeoAsciiParamsTag\n");
						}
						header.vlr_geo_ascii_params = (char*)header.vlrs[i].data;
					}
				}
      }
    }
	
    // load any number of user-defined bytes that might have been added after the header
	
    header.user_data_after_header_size = (int)header.offset_to_point_data - vlrs_size - header.header_size;
    if (header.user_data_after_header_size)
    {
		header.user_data_after_header = new char[header.user_data_after_header_size];
		
		if (fread(header.user_data_after_header, sizeof(char), header.user_data_after_header_size, file) != (unsigned int)header.user_data_after_header_size)
		{
			fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_after_header\n", header.user_data_after_header_size);
			return false;
		}
    }
  }
  
  gps_time = 0.0;
  rgb[0] = rgb[1] = rgb[2] = 0;
  npoints = header.number_of_point_records;
  p_count = 0;
  
  return true;
}

void LASreader::reopen()
{
	fseek( (myFILE *)m_file, header.offset_to_point_data, SEEK_SET);
	
	p_count=0;

	gps_time = 0.0;
}

bool LASreader::set_readpos(int ptNum)
{
	if( ptNum < 0 )
		return false;

#ifdef WIN32
	// 最大可相对定位范围

	fseek( (myFILE *)m_file, header.offset_to_point_data + (__int64)header.point_data_record_length*ptNum, SEEK_SET);

#else
	fseek( (myFILE *)m_file, header.offset_to_point_data + header.point_data_record_length*ptNum, SEEK_SET);

#endif

	p_count = ptNum;

	//gps_time = 0.0;

	return true;
}

bool LASreader::set_readpos(ors_int64 offset)
{
	p_count = (offset-header.offset_to_point_data)/header.point_data_record_length;

#ifdef WIN32
	// 最大可相对定位范围

	return fseek( (myFILE *)m_file, offset, SEEK_SET);

#else
	return fseek( (myFILE *)m_file, offset, SEEK_SET);

#endif

//	p_count = ptNum;

	//gps_time = 0.0;

//	return true;
}


// bool LASreader::read_point(void* pData)
// {
// 
// 	return false;
// }

bool LASreader::read_point()
{
	if (p_count < npoints)
	{
		if (pointReader->read_point(&point, &gps_time, rgb) == false)
		{
			fprintf(stderr,"WARNING: end-of-file after %d of %d points\n", p_count, npoints);
			return false;
		}

		if (additional_bytes_per_point)
		{
#ifdef WIN32
			fseek( (myFILE *)m_file, additional_bytes_per_point, SEEK_CUR );
#else
			for (int i = 0; i < additional_bytes_per_point; i++) 
				fgetc((FILE*)m_file);
#endif
		}

		p_count++;
		return true;
	}
	return false;
}

bool LASreader::read_rawpoint(int* coordinates)
{
	if (read_point())
	{
		coordinates[0]=point.x;
		coordinates[1]=point.y;
		coordinates[2]=point.z;
		return true;
	}
	return false;
}

bool LASreader::read_point(float* coordinates)
{
	if (read_point())
	{
		get_coordinates(coordinates);
		return true;
	}
	return false;
}

bool LASreader::read_point(double* coordinates)
{
	if (read_point())
	{
		get_coordinates(coordinates);
// 		if(m_attFlag == PA_INTENSITY)
// 			coordinates[2] = point.intensity;

		return true;
	}
	return false;
}

// bool LASreader::read_point(LASpointXYZI* pt)
// {
// 	if (read_point())
// 	{
// 		pt->x = point.x*header.x_scale_factor+header.x_offset;
// 		pt->y = point.y*header.y_scale_factor+header.y_offset;
// 		pt->z = point.z*header.z_scale_factor+header.z_offset;
// 		pt->intensity = point.intensity;
// 		return true;
// 	}
// 	return false;
// }

// bool LASreader::read_point(LDI_POINT *pt)
// {
// 	if (read_point())
// 	{
// 		pt->x = point.x*header.x_scale_factor+header.x_offset;
// 		pt->y = point.y*header.y_scale_factor+header.y_offset;
// 		pt->z = point.z*header.z_scale_factor+header.z_offset;
// 		
// 		pt->intensity = point.intensity;
// 		pt->classification = point.classification;
// 		pt->edge_of_flight_line = point.edge_of_flight_line;
// 		pt->number_of_returns_of_given_pulse = point.number_of_returns_of_given_pulse;
// 		pt->return_number = point.return_number;
// 		pt->scan_direction_flag = point.scan_direction_flag;
// 		
// 		return true;
// 	}
// 	return false;
// }

void LASreader::get_coordinates(float* coordinates)
{
	coordinates[0] = (float)(point.x*header.x_scale_factor+header.x_offset);
	coordinates[1] = (float)(point.y*header.y_scale_factor+header.y_offset);
	coordinates[2] = (float)(point.z*header.z_scale_factor+header.z_offset);
}

void LASreader::get_coordinates(double* coordinates)
{
	coordinates[0] = point.x*header.x_scale_factor+header.x_offset;
	coordinates[1] = point.y*header.y_scale_factor+header.y_offset;
	coordinates[2] = point.z*header.z_scale_factor+header.z_offset;
}

unsigned short LASreader::get_intensity()
{
	return point.intensity;
}

unsigned char LASreader::get_classification()
{
	return point.classification;
}

int LASreader::get_return_number()
{
	return point.return_number;
}

int LASreader::get_number_of_returns()
{
	return point.number_of_returns_of_given_pulse;
}

unsigned char LASreader::get_edge_of_flight_line()
{
	return point.edge_of_flight_line;
}

unsigned char LASreader::get_scan_direction_flag()
{
	return point.scan_direction_flag;
}

char LASreader::get_scan_angle_rank()
{
	return point.scan_angle_rank;
}

unsigned char LASreader::get_user_data()
{
	return point.user_data;
}

unsigned short LASreader::get_point_source_ID()
{
	return point.point_source_ID;
}

double LASreader::get_gpstime()
{
	return gps_time;
}

void LASreader::get_rgb(unsigned short Rgb[3])
{
	Rgb[0] = rgb[0];
	Rgb[1] = rgb[1];
	Rgb[2] = rgb[2];
}

void LASreader::close()
{
	if( NULL == m_file )
		return ;
	
	fclose( (myFILE *)m_file );
	
	m_file = 0;
	
	if (pointReader) 
	{
		delete pointReader;
		pointReader = 0;
	}
}

LASreader::LASreader(bool bForRegister)
{
	gps_time = 0.0;
	rgb[0] = rgb[1] = rgb[2] = 0;
	points_have_gps_time = false;
	points_have_rgb = false;
	npoints = -1;
	p_count = -1;
	additional_bytes_per_point = 0;
	m_file = 0;
	pointReader = 0;

	m_ptContent = ORS_PCM_XYZ;
//	m_attFlag = PA_ELEVATION;
}

LASreader::~LASreader()
{
	close();
}

// void* LASreader::get_header()
// {
// 	return &header;
// }

// void LASreader::set_attribution_flag(orsPointAttributionFlag flag)
// {
// 	m_attFlag = flag;
//}
