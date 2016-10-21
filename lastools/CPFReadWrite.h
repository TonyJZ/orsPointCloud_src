#ifndef _Corresponding_Point_File_def_zj_2011_11_6
#define _Corresponding_Point_File_def_zj_2011_11_6

#include "orsBase/orsTypedef.h"

// #ifdef _LAS_LIB_
// #define  _LasLib_  ORS_EXPORT
// #else
// #define  _LasLib_  ORS_IMPORT	
// #endif
// 
// #ifndef _LAS_LIB_
// #ifdef _DEBUG
// #pragma comment(lib,"LasLibD.lib")
// #else
// #pragma comment(lib,"LasLib.lib")
// #endif
// #endif


//////匹配点文件格式说明////////
//	CPF_Flag[]="CPF format"			32bytes
//  npoints							4bytes
//	limgName						128bytes
//	rimgName						128bytes
//	match_pt						20*npoints

struct match_pt
{
	ors_int32 ID;
	float xl, yl;
	float xr, yr;
};

const ors_int32 cpf_headSize=36;
const ors_int32 cpf_offsettodata=292;

//读取对应点信息
class /*_LasLib_*/ CPFreader
{
public:
	char header[32];
	
	
	ors_int32 npoints;
	ors_int32 p_count;
	
	
public:
	CPFreader();
	~CPFreader();
	
	
	bool open(FILE* file);
	void reopen();	//重新从第一个点记录开始读取
	
	void close();
	
	bool read_point(match_pt* pt);
	bool read_imgName(char *plImgName, char *prImgName);
	
private:
	FILE* file;
//	LASpointReader* pointReader;
};


class /*_LasLib_*/ CPFwriter
{
public:
	ors_int32 npoints;
	ors_int32 p_count;

	char header[32];
	
public:
	CPFwriter();
	~CPFwriter();
	
	bool open(FILE* file);
	
	void close();
	
	bool write_imgName(const char* plImgName, const char* prImgName);
	bool write_point(match_pt* pt);
	
private:
	FILE* file;
	
};

#endif