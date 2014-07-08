
// g++ -fPIC -shared main.cpp -o main.so

//#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cassert>

#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>



typedef dirent* (*RDDR) (DIR *dirp);
typedef int (*RDDR_R) (DIR *dirp, struct dirent *entry, struct dirent **result);


bool checkDirectory (dirent * dirPtr) {
	printf ("Name: %s\n", dirPtr->d_name);
	return true;
}

struct dirent *readdir(DIR *dirp)
{
	dirent *tmp;
	RDDR f_ptr = (RDDR)dlsym (RTLD_NEXT, "readdir");
	
	assert (f_ptr != NULL);
	
	while (tmp = f_ptr (dirp)) {
		if (checkDirectory (tmp)) return tmp;
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
		if (checkDirectory (entry)) return 0;
	}
}















