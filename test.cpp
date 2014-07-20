
#include <cstdio>
#include <iostream>
#include <string>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>



int main () {
	std::string nm = "qwerty\n", file ("/home/rainbow/src/4_softs/test.cpp.txt");
	int fd;
	char buf [128];
	
	
	std::cout << tmpnam (buf) << std::endl;
	std::cout << getcwd (buf, 128) << std::endl;
	if (-1 == (fd = open (file.c_str (), O_CREAT | O_RDONLY, S_IRWXU))) {
		printf ("Error of open (first): %s - %d\n", strerror (errno), errno);
		return 1;
	}
	ssize_t ret = read (fd, &fd, 4);
	int err = errno;
	printf ("Ret: 0x%016zX, errno: %d\n", ret, err);
	
	if (-1 == open (file.c_str (), O_CREAT | O_RDONLY)) {
		printf ("Error of open (second): %s - %d\n", strerror (errno), errno);
		return 1;
	}
	
	DIR* dirPtr = opendir ("/home/rainbow/src/4_softs/");
	if (dirPtr == NULL) {
		printf ("Error of opendir: %s\n", strerror (errno));
		return 2;
	}
	int fdDir = dirfd (dirPtr);
	int fdFile = openat (fdDir, "test.cpp", O_RDONLY, 0);
	if (fdFile == -1) {
		printf ("Error of openat: %s - %d\n", strerror (errno), errno);
		return 1;
	}
	
	while (true) {
		std::cout << nm;
		sleep (3);
	}
	
	return 0;
}







