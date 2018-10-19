all : 
	echo "Compiling"
	g++ linuxmain.cpp -o spplinux -g -lX11 -Wno-multichar
