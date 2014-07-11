
//
// Headers
//
#include <string>

#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cassert>
#include <cstdlib>

#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//
// Macroses
//

//#define NDEBUG

#ifndef NDEBUG
	#define MAGIC_STRING "0x1234DEADBEAF4321"
#endif

//
// Prototypes and POD types
//
extern "C" void finitFunc ();
extern "C" void initFunc ();


typedef dirent* (*RDDR) (DIR *dirp);
typedef int (*RDDR_R) (DIR *dirp, struct dirent *entry, struct dirent **result);

//
// C++ classes and structures
//

class CErrnoSaver {
	int m_errno;
public:
	CErrnoSaver (): m_errno (errno) {}
	~CErrnoSaver () {errno = m_errno;}
};






