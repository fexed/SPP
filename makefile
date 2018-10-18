all : 
	echo "Compiling"
	g++ linuxmain.cpp keyboard.cc -o spplinux -g -lX11 -Wno-multichar -pthread
