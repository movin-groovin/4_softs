
#include <main.h>



int open(const char *pathname, int flags, mode_t mode) {
	OPEN opnPtr = (OPEN)dlsym (RTLD_NEXT, "open");
	
	
	assert (opnPtr != NULL);
	
	
	
	
	return -1;
}

int creat(const char *pathname, mode_t mode) { // O_CREATE | O_TRUNC | O_WRONLY
	
	return -1;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	
	return -1;
}
