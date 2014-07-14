
#include <main.h>


static DESCR_AND_FLAGS dsInfo;



int open(const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	std::string file (pathname);
	size_t ind;
	char chArr[3]; // just in case for utf-16 coding with 2-bytes symbols
	
	
	assert (opnPtr != NULL);
	
	ind = file.find_last_of ('/', 0);
	++ind;
	if (ind != std::string::npos) file.assign (file, ind, file.length () - ind);
	if (!strcmp (prelFileName, file.c_str ())) {
		int fd = opnPtr (dynamic_cnf.txt, O_RDWR);
		ind = rdPtr (fd, chArr, 2);
		
		// We don't know have created or not the file. At default we think not.
		// This situation is rather unlikely
		if (-1 == ind) {
			if (flags & (~O_CREAT))
				return opnPtr (pathname, flags & (~O_TRUNC), mode);
			else {
				serr.set (ENOENT);
				return -1;
			}
		}
		
		chArr[ind] = '\0';
		// If we work with utf-16, not utf-8
		if (chArr[0] == 0) {
			chArr [0] = chArr[1];
			chArr[1] = '\0';
		}
		
		// Have created
		if (chArr[0] == '1') {
			
		}
		// Havn't created
		else {
			
		}
		
		
		return -1;
	}
	
	
	return opnPtr (pathname, flags, mode);
}

int creat(const char *pathname, mode_t mode) { // O_CREATE | O_TRUNC | O_WRONLY
	
	
	
	
	return -1;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	
	
	
	
	return -1;
}
