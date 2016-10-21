#ifndef _MY_FILE_H_


#ifdef _WIN32

#include "ifile.h"

#define myFILE IFILE
#define fopen ifopen
#define fread ifread
#define fwrite ifwrite
#define fseek ifseek

#define fclose ifclose

#else

#define myFILE FILE

#endif

#endif
