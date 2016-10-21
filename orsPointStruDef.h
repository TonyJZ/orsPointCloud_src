#ifndef _ORS_POINT_STRUCTURE_DEFINITION_H__
#define _ORS_POINT_STRUCTURE_DEFINITION_H__

#pragma once

#pragma pack(1) // 按照1字节方式进行对齐

//las点云的内存结构  36bytes  
//2010.6.24  将结构压缩为16 bytes
#ifndef _LDI_POINT_Def
#define _LDI_POINT_Def  
typedef struct LDI_POINT
{
//	DWORD  ptNo;	//点号
	float x;
	float y;
	float z;
	unsigned short intensity;
	unsigned char return_number : 3;	//激光脉冲的回波号	1 ~ 5
	unsigned char number_of_returns_of_given_pulse : 3;	 //总回波数 1 ~ 5
	unsigned char scan_direction_flag : 1;
	unsigned char edge_of_flight_line : 1;
	unsigned char classification;
//	double	gps_time;
	
} LDI_POINT;
#endif

#ifndef LDI_POINT_SIZE
#define LDI_POINT_SIZE sizeof(LDI_POINT)
#endif
//const int LDI_POINT_SIZE=sizeof(LDI_POINT);


#pragma pack() // 取消1字节对齐方式

#endif