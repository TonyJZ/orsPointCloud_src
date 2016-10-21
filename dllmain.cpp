// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#ifdef  WIN32 
#ifdef _DEBUG  
// 内存泄露检查, 请下载、安装visual leak detector 库
//#include "vld.h"
#endif
#endif

#include "orsBase/orsIPlatform.h"
//#include "orsImageUtil.h"

//#include "orsXImageChain.h"
//#include "orsXImageData.h"

#include "orsXPointCloudService.h"



BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}



// #include "lasReader.h"
// orsIObject* createLASReader( bool bForRegister )
// {
// 	CLASReader *reader = new CLASReader;
// 	
// 
// 	return reader;
// }
// 
// #include "lasWriter.h"
// orsIObject* createLASWriter( bool bForRegister )
// {
// 	CLASWriter *writer = new CLASWriter;
// 	
// 	
// 	return writer;
// }
// 
// /////////////////////////////////////////////////////////////////////////
// 
// orsIPointCloudService *getPointCloudService()
// {
// 	orsIPointCloudService *pointService = 
// 		ORS_PTR_CAST(orsIPointCloudService, getPlatform()->getService( ORS_SERVICE_POINTCLOUD) );
// 	
// 	assert( pointService );
// 	
// 	return pointService;
// }
// 
// 
// static orsIPlatform *g_pPlatform = NULL;
// 
// orsIPlatform *getPlatform()	{ return g_pPlatform; };
// 
// extern "C" PLUGIN_API void orsModuleInitialize(orsIPlatform* platform) 
// {
// 	g_pPlatform = platform;
// 	
// 	platform->registerService( ORS_SERVICE_POINTCLOUD, new orsXPointCloudService );
// 	
// 	platform->getRegisterService()->registerObject(createLASReader);
// 	platform->getRegisterService()->registerObject(createLASWriter);
// }
// 
// extern "C" PLUGIN_API void orsModuleUninitialize()
// {
// 
// }

