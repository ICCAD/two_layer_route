final:main.o layer.o
	g++ -g -o final main.o layer.o
main.o:main.cpp layer.h
	g++ -g -c main.cpp layer.h
layer.o:layer.cpp layer.h
	g++ -g -c layer.cpp layer.h
clean:
	rm final main.o layer.o