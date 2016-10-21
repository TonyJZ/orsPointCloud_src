/********************************************************************
	离散点云服务    create by Tony
	2009.4.7		提供点云数据的读写
*********************************************************************/

#include "stdafx.h"
#include "orsBase/orsIPlatform.h"
#include "orsBase/orsIRegisterService.h"
#include "orsXPointCloudService.h"
//#include "orsPointCloud\orsIPointCloudSourceReader.h"
//#include "orsPointCloud\orsIPointCloudSourceWriter.h"


orsIPointCloudReader *orsXPointCloudService::openPointFileForRead( const char *filename, bool bShared )
{
	orsIRegisterService *registerService = getPlatform()->getRegisterService();

	orsArray<ref_ptr<orsIObjectDesc> > objDescs = 
		registerService->getObjectDescsByInterface( "orsIPointCloudReader" );

	if( objDescs.size() == 0 )	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not find orsIPointCloudReader" );
		return NULL;
	}

	for(int i=0;i<objDescs.size();i++)
	{
		orsIObjectDesc *desc = objDescs[i].get();

		orsIPointCloudReader *reader = ORS_CREATE_OBJECT( orsIPointCloudReader, desc->getID() );

		if( reader != NULL )	{
			if( reader->open(filename, bShared ) ) 
				return reader;
			
			reader->release();
		}
	}

	return NULL;
}

orsIPointCloudWriter *orsXPointCloudService::openPointFileForWrite( const char *fileFormat, const char *lasFileName, 
		lasDataFORMAT point_data_format, double xScale, double yScale, double zScale )
{
	orsIRegisterService *registerService = getPlatform()->getRegisterService();
	
	orsArray<ref_ptr<orsIObjectDesc> > objDescs = 
		registerService->getObjectDescsByInterface( "orsIPointCloudWriter" );
	
	if( objDescs.size() == 0 )	{
		getPlatform()->logPrint( ORS_LOG_INFO, "Can not find orsIPointCloudWriter" );
		return NULL;
	}
	
	for(int i=0;i<objDescs.size();i++)
	{
		orsIObjectDesc *desc = objDescs[i].get();
		
		orsIPointCloudWriter *writer = ORS_CREATE_OBJECT( orsIPointCloudWriter,  desc->getID() );

		if( writer != NULL )	
		{
			if( writer->get_file_format() == fileFormat )
			{
				// 分析内容的支持能力
				// ...

				//
				if( writer->open( lasFileName, point_data_format, xScale, yScale, zScale ) ) 
					return writer;
			}
					
			writer->release();
		}
	}
	
	return NULL;
}

// orsFileFormatList orsXPointCloudService::getSupportedPointFormatsForRead()
// {
// 	orsFileFormatList allFormatlist;
// 
// 	orsIRegisterService *registerService = getPlatform()->getRegisterService();
// 
// 	orsArray<ref_ptr<orsIObjectDesc> > objDescs = 
// 		registerService->getObjectDescsByInterface("orsIPointCloudReader");
// 
// 	for(int i=0;i<objDescs.size();i++)
// 	{
// 		orsFileFormatList formatlist;
// 		ref_ptr<orsIObjectDesc> desc = objDescs[i];
// 		ref_ptr<orsIPointCloudReader> reader = ORS_CREATE_OBJECT(orsIPointCloudReader, desc->getID() );
// 
// 		if(reader != NULL){
// 			formatlist = reader->getSupportedPointFormats();
// 			for(int j=0;j<formatlist.size();j++)
// 				allFormatlist.push_back(formatlist[j]);
// 		}
// 	}
// 
// 	return allFormatlist;
// }
// 
// orsFileFormatList orsXPointCloudService::getSupportedPointFormatsForWrite()
// {
// 	orsFileFormatList allFormatlist;
// 	
// 	orsIRegisterService *registerService = getPlatform()->getRegisterService();
// 	
// 	orsArray<ref_ptr<orsIObjectDesc> > objDescs = 
// 		registerService->getObjectDescsByInterface("orsIPointCloudWriter");
// 	
// 	for(int i=0;i<objDescs.size();i++)
// 	{
// 		orsFileFormatList formatlist;
// 		ref_ptr<orsIObjectDesc> desc = objDescs[i];
// 		ref_ptr<orsIPointCloudWriter> writer = ORS_CREATE_OBJECT(orsIPointCloudWriter, desc->getID());
// 		
// 		if(writer != NULL){
// 			formatlist = writer->getSupportedPointFormats();
// 			for(int j=0;j<formatlist.size();j++)
// 				allFormatlist.push_back(formatlist[j]);
// 		}
// 	}
// 	
// 	return allFormatlist;
// }

bool orsXPointCloudService::startup(orsIPlatform *platform)
{
	return true;
}

void orsXPointCloudService::shutdown()
{
	return;
}

bool orsXPointCloudService::isok()
{
	return true;
}
