
#include <iostream>
#include <string>

#include <unistd.h>



int main () {
	std::string nm = "qwerty\n";
	
	while (true) {
		std::cout << nm;
		sleep (3);
	}
	
	return 0;
}







