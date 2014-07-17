
#include <cstdio>
#include <iostream>
#include <string>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main () {
	std::string nm = "qwerty\n", file ("/home/yarr/src/4_softs/test.cpp");
	int fd;
	
	
	if (-1 == (fd = open (file.c_str (), O_CREAT | O_RDONLY, 0))) {
		printf ("Error 111 of open: %s - %d\n", strerror (errno), errno);
		return 1;
	}
	if (-1 == open (file.c_str (), O_CREAT | O_RDONLY)) {
		printf ("Error 222 of open: %s - %d\n", strerror (errno), errno);
		return 1;
	}
	
	while (true) {
		std::cout << nm;
		sleep (3);
	}
	
	return 0;
}







