#ifndef OPENRS_POINTCLOUD_SERVICE_IMP_H
#define OPENRS_POINTCLOUD_SERVICE_IMP_H

#include "orsPointCloud/orsIPointCloudService.h"

class orsXPointCloudService : public orsIPointCloudService, public orsObjectBase
{
public:	
//	virtual int SizeOfType(orsDataTYPE type);

// 	virtual orsFileFormatList getSupportedPointFormatsForRead();
// 	virtual orsFileFormatList getSupportedPointFormatsForWrite();

	//��һ��Ӱ���ļ������������е�ʵ����orsIImageReader�Ķ�����ʵ��
	virtual orsIPointCloudReader *openPointFileForRead( const char *filename, bool bShared = true );
	virtual orsIPointCloudWriter *openPointFileForWrite( const char *fileFormat, const char *lasFileName, 
		lasDataFORMAT point_data_format, double xScale, double yScale, double zScale );
	
	virtual bool startup(orsIPlatform *platform);
	virtual void shutdown();
	virtual bool isok();
	
	//Object����ʵ��
	ORS_OBJECT_IMP2( orsXPointCloudService, orsIPointCloudService, orsIService, "default", "PointCloudService" )
};

#endif