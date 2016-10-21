#ifndef OPENRS_POINTCLOUD_SERVICE_IMP_H
#define OPENRS_POINTCLOUD_SERVICE_IMP_H

#include "orsPointCloud/orsIPointCloudService.h"

class orsXPointCloudService : public orsIPointCloudService, public orsObjectBase
{
public:	
//	virtual int SizeOfType(orsDataTYPE type);

// 	virtual orsFileFormatList getSupportedPointFormatsForRead();
// 	virtual orsFileFormatList getSupportedPointFormatsForWrite();

	//打开一个影像文件，将遍历所有的实现了orsIImageReader的对象来实现
	virtual orsIPointCloudReader *openPointFileForRead( const char *filename, bool bShared = true );
	virtual orsIPointCloudWriter *openPointFileForWrite( const char *fileFormat, const char *lasFileName, 
		lasDataFORMAT point_data_format, double xScale, double yScale, double zScale );
	
	virtual bool startup(orsIPlatform *platform);
	virtual void shutdown();
	virtual bool isok();
	
	//Object方法实现
	ORS_OBJECT_IMP2( orsXPointCloudService, orsIPointCloudService, orsIService, "default", "PointCloudService" )
};

#endif