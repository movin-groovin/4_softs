
# Some make file

main_target: main.so
    
	
#main.so: main.o
#	ld -ldl -shared -fini=initFunc -init=initFunc main.o -o main.so
	
main.so: main.cpp main.h
	gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ \
	-fPIC main.cpp -o main.so -Wl,-fini=initFunc,-init=initFunc
	
clean:
	rm -f *.o
#reset
