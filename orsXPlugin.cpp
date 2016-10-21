#include "stdafx.h"

#include "orsBase/orsIPlatform.h"
#include "orsXPointCloudService.h"
//#include "orsBase\orsIUtilityService.h"
#include "orsBase/orsIPlugin.h"
#include "orsBase/orsIRegisterService.h"

/*orsIPlatform *g_platform = NULL;*/

#include "orslasReader.h"
orsIObject* createLASReader( bool bForRegister )
{
	LASreader *reader = new LASreader(bForRegister);
	return reader;
}

#include "orslasWriter.h"
orsIObject* createLASWriter( bool bForRegister )
{
	LASwriter *writer = new LASwriter(bForRegister);
	return writer;
}

// orsIPointCloudService *getPointCloudService()
// {
// 	orsIPointCloudService *pointService = 
// 		ORS_PTR_CAST(orsIPointCloudService, getPlatform()->getService( ORS_SERVICE_POINTCLOUD) );
// 	
// 	assert( pointService );
// 	
// 	return pointService;
// }

class orsXPlugin: public orsIPlugin
{
public:
	virtual const orsChar *getID()	
	{
		return "ors.service.PointCloud"; 
	};

	virtual	const orsChar *getName()	
	{	
		return "Point Cloud Service"; 
	};
	
	virtual const orsChar *getProvider() 
	{	
		return "edu.whu.liesmars"; 
	};
	
	virtual const orsChar *getVersion()
	{
		return "0.1";
	}
	
	virtual  bool initialize(orsIPlatform*  platform)
	{
		platform->registerService( ORS_SERVICE_POINTCLOUD, new orsXPointCloudService );

		orsIRegisterService *pReg = platform->getRegisterService();

		pReg->registerObject( createLASReader );
		pReg->registerObject( createLASWriter );

	
		return true;
	}
	
	virtual void finalize()
	{
		
	}
};

ORS_REGISTER_PLUGIN( orsXPlugin )


