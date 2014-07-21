
# gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ -std=c++11 -fPIC 
# main.cpp dir.cpp open.cpp lseek.cpp read.cpp write.cpp -o main.so -Wl,-fini=initFunc,-init=initFunc

# Some make file

main_target: main.so
    
	
#main.so: main.o
#	ld -ldl -shared -fini=initFunc -init=initFunc main.o -o main.so
	
main.so: main.cpp main.h init_fini.cpp
	gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ -std=c++11 \
	-fPIC main.cpp init_fini.cpp -o main.so -Wl,-fini=initFunc,-init=initFunc
	
clean:
	rm -f *.o
#reset
