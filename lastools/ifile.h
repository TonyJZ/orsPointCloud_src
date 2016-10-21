
#if !defined(AFX_STDAFX_H____INCLUDED_)
#define AFX_STDAFX_H____INCLUDED_

struct IFILE;
IFILE *ifopen(const char *fname,const char *perm);

size_t ifread(void* buf,size_t size,size_t count,IFILE *fp);

size_t ifwrite(const void* buf,size_t size,size_t count,IFILE *fp);

__int64 ifseek(IFILE *fp,__int64 offset,int flag);

long ifclose(IFILE *fp);

DWORD get_file_length( IFILE *file );

#endif