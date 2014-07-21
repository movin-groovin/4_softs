
#include "main.h"
// Wl is gcc option to pass the parameters to the linker
// gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ -fPIC main.cpp -o main.so


/*
* /proc/$PID/comm - process name
* /proc/$PID/cmdline - cmd line without whitespaces
* To specify proper process we will add UUID number as one of cmdline argument
*/


void initFunc () {
	//abort ();
	//printf ("===============\n");
	return;
}

void finitFunc () {
	//
	// When this func is calling, descriptors 0-2 already are closed
	//
	//printf ("+++++++++++++++\n");
	//sleep (1000);
	return;
}











