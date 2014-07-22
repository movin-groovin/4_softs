
# gcc -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ -std=c++11 -fPIC 
# main.cpp dir.cpp open.cpp lseek.cpp read.cpp write.cpp -o main.so -Wl,-fini=finitFunc,-init=initFunc

# Some make file
objects = dir.o lseek.o main.o open.o read.o write.o
main_target: main.so
    
	
#main.so: main.o
#	ld -ldl -shared -fini=initFunc -init=initFunc main.o -o main.so
	
main.so: $(objects)
	g++ -static-libgcc -static-libstdc++ -shared -ldl -lstdc++ \
	$(objects) -o main.so -Wl,-fini=finitFunc,-init=initFunc
	
dir.o: dir.cpp main.h main.o
	g++ -fPIC -c -std=c++11 dir.cpp
	
lseek.o: lseek.cpp main.h main.o
	g++ -fPIC -c -std=c++11 lseek.cpp
	
open.o: open.cpp main.h main.o
	g++ -fPIC -c -std=c++11 open.cpp
	
read.o: read.cpp main.h main.o
	g++ -fPIC -c -std=c++11 read.cpp
	
write.o: write.cpp main.h main.o
	g++ -fPIC -c -std=c++11 write.cpp
	
main.o: main.cpp main.h
	g++ -fPIC -c -std=c++11 main.cpp
	
clean:
	rm -f *.o
#reset
















