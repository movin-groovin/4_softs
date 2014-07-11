
# Some make file

main_target: main.so
    
	
main.so: main.o
	ld -ldl -shared -fini=initFunc -init=initFunc main.o -o main.so
	
main.o: main.cpp main.h
	gcc -static-libgcc -static-libstdc++ -lstdc++ -fPIC -c main.cpp
	
clean:
	rm -f *.o
#reset
