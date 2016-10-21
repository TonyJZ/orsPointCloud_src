#ifndef _ORS_POINT_STRUCTURE_DEFINITION_H__
#define _ORS_POINT_STRUCTURE_DEFINITION_H__

#pragma once

#pragma pack(1) // ����1�ֽڷ�ʽ���ж���

//las���Ƶ��ڴ�ṹ  36bytes  
//2010.6.24  ���ṹѹ��Ϊ16 bytes
#ifndef _LDI_POINT_Def
#define _LDI_POINT_Def  
typedef struct LDI_POINT
{
//	DWORD  ptNo;	//���
	float x;
	float y;
	float z;
	unsigned short intensity;
	unsigned char return_number : 3;	//��������Ļز���	1 ~ 5
	unsigned char number_of_returns_of_given_pulse : 3;	 //�ܻز��� 1 ~ 5
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


#pragma pack() // ȡ��1�ֽڶ��뷽ʽ

#endif