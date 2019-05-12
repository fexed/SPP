CC 			= g++
CFLAGS		= -g -Wall -Wno-multichar
OPTFLAGS	=
INCLUDES	= -I.
LDFLAGS		= -L.
LIBS		= -lX11 -lncurses

.PHONY: all

all : 
	@echo "Compiling"
	$(CC) $(CFLAGS) $(INCLUDES) $(OPTFLAGS) linuxmain.cpp -o spplinux $(LIBS)
	@echo "Done"