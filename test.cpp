
#include <iostream>
#include <string>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main () {
	std::string nm = "qwerty\n", file ("/home/rainbow/src/4_softs/qwertyui");
	int fd;
	
	
	if (-1 == (fd = open (file.c_str (), O_CREAT | O_RDONLY, 0))) {
		printf ("Error of open: %s - %d\n", strerror (errno), errno);
		return 1;
	}
	
	while (true) {
		std::cout << nm;
		sleep (3);
	}
	
	return 0;
}







