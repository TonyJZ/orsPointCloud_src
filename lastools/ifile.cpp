// IFile.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "IFile.h"


struct IFILE 
{
	HANDLE hfile;
	__int64 pos;
};



IFILE *ifopen(const char *fname,const char *perm)
{
	DWORD flag=0;
	DWORD access=GENERIC_READ;

// 	for (long i=0;i<strlen(perm);i++)
// 	{
	switch(perm[0])
	{
		// 		case 'a':
		//			flag|=(GENERIC_READ&);
		// 			break;
	case 'r':
		flag=OPEN_EXISTING;
		access=GENERIC_READ;
		break;
	case 'w':
		flag=CREATE_ALWAYS;
		access=GENERIC_WRITE;
		break;
	default:
		flag=OPEN_ALWAYS;
		access=GENERIC_READ;
		break;
	}


	HANDLE hfile;
	if( strchr( perm, 's' ) )	{
		hfile=CreateFile(fname, access, FILE_SHARE_READ, NULL, flag/*OPEN_ALWAYS*/,0/*FILE_FLAG_OVERLAPPED*/,NULL);
	}
	else
		hfile = CreateFile(fname, access, 0/* FILE_SHARE_READ*/, NULL, flag/*OPEN_ALWAYS*/,0/*FILE_FLAG_OVERLAPPED*/,NULL);

	if( NULL != hfile )	{
		IFILE *fp=new IFILE;
		fp->hfile=hfile;
		fp->pos=0;
		return(fp);
	}

	
	return NULL;
}

size_t ifread(void* buf,size_t size,size_t count,IFILE *fp)
{
	DWORD rtn=0;

	__int64     off=fp->pos;
	OVERLAPPED overlapped;
 	
 	overlapped.Internal    =0;
 	overlapped.InternalHigh=0;
 	overlapped.Offset     = *((DWORD *)((char *)&off));
 	overlapped.OffsetHigh = *((DWORD *)((char *)&off + sizeof(DWORD)));
	overlapped.hEvent     = NULL;
	ReadFile(fp->hfile,buf,size*count,&rtn,&overlapped);
	fp->pos+=rtn;

	return( rtn/size );
}

size_t ifwrite(const void* buf,size_t size,size_t count,IFILE *fp)
{
	DWORD rtn=0;

	__int64     off=fp->pos;
	OVERLAPPED overlapped;
 	
 	overlapped.Internal    =0;
 	overlapped.InternalHigh=0;
 	overlapped.Offset     = *((DWORD *)((char *)&off));
 	overlapped.OffsetHigh = *((DWORD *)((char *)&off + sizeof(DWORD)));
	overlapped.hEvent     = NULL;
	BOOL b = WriteFile(fp->hfile,buf,size*count,&rtn,&overlapped);
	fp->pos+=rtn;

	DWORD err = GetLastError();

	return(rtn/size);

}

__int64 ifseek(IFILE *fp,__int64 offset,int flag)
{
	DWORD low;
	DWORD high;
	switch(flag)
	{
	case SEEK_SET:
		fp->pos=offset;
		break;
	case SEEK_CUR:
		fp->pos+=offset;
		break;
	case SEEK_END:
		low=GetFileSize(fp->hfile,&high);
		*((DWORD*)&(fp->pos))=low;
		*((DWORD*)&(fp->pos)+1)=high;
		fp->pos+=offset;
		break;
	}
	return(fp->pos);
}

long ifclose(IFILE *fp)
{
	CloseHandle(fp->hfile);
	delete fp;
	return(1);
}

/*
#define BUFFERSIZE 1024*1024
int main(int argc, char* argv[])
{
	IFILE *fp=ifopen("e:\\hdf\\TEST_CP.HDF","wr");
	IFILE *fp1=ifopen("h:\\big.hd","ss");
	char *buf = new char[BUFFERSIZE];

	DWORD rtn;
 	while((rtn=ifread(buf,BUFFERSIZE,1,fp)))
 	{
 		//memset(buf,'A'+i%26,10000);
 		ifwrite(buf,rtn,1,fp1);
	}
	ifclose(fp);
	ifclose(fp1);
	return 0;
}
#undef CloseHandle*/


DWORD get_file_length( IFILE *file ) 
{
	DWORD  dwFileSize = GetFileSize( file, NULL);
	
	return dwFileSize;
}