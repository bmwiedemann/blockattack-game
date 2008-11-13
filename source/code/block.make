#ifndef GAMEDIR
	GAMEDIR=../../Game/
#endif

#ifndef BINARY
BINARY=$(GAMEDIR)blockattack
#endif

#ifndef CC
	CC=gcc
#endif

#ifndef CPP
	CPP=g++
#endif

#ifndef BASE_CFLAGS
	BASE_CFLAGS=-c $(shell sdl-config --cflags)
#endif

#ifndef BASE_LIBS
	BASE_LIBS=$(shell sdl-config --libs) -lSDL_image -lSDL_mixer -lSDL_ttf
#endif

#For developement only 
ifndef DEBUG
DEBUG=1
endif

ifndef NETWORK
NETWORK=1
endif

#Never use the apstract fs, it is not implemented and just linking it breaks on some systems
ifndef USE_ABSTRACT_FS
USE_ABSTRACT_FS=0
endif

#Compile with debug information or optimized.
ifeq ($(DEBUG),1)
BASE_CFLAGS += -g -DDEBUG=1
else
BASE_CFLAGS += -O4
endif

ifeq ($(NETWORK),1)
BASE_CFLAGS += -DNETWORK=1
BASE_LIBS += -lenet
else
BASE_CFLAGS += -DNETWORK=0
endif

ifeq ($(USE_ABSTRACT_FS),1)
BASE_CFLAGS += -DUSE_ABSTRACT_FS=1
BASE_LIBS += -lphysfs
else
BASE_CFLAGS += -DUSE_ABSTRACT_FS=0
endif

$(BINARY): 	main.o highscore.o SFont.o ReadKeyboard.o joypad.o listFiles.o replay.o common.o stats.o uploadReplay.o
	$(CPP) -O -o $(BINARY) main.o highscore.o SFont.o ReadKeyboard.o joypad.o listFiles.o replay.o common.o stats.o $(BASE_LIBS)
#-lphysfs

main.o:	main.cpp BlockGame.hpp mainVars.hpp common.h
	$(CPP) $(BASE_CFLAGS) main.cpp

highscore.o: highscore.h highscore.cpp
	$(CPP) $(BASE_CFLAGS) highscore.cpp

SFont.o: SFont.h SFont.c
	$(CC) $(BASE_CFLAGS) SFont.c

ReadKeyboard.o: ReadKeyboard.h ReadKeyboard.cpp
	$(CPP) $(BASE_CFLAGS) ReadKeyboard.cpp

joypad.o: joypad.h joypad.cpp
	$(CPP) $(BASE_CFLAGS) joypad.cpp

listFiles.o: listFiles.h listFiles.cpp
	$(CPP) $(BASE_CFLAGS) listFiles.cpp

replay.o: replay.h replay.cpp
	$(CPP) $(BASE_CFLAGS) replay.cpp

stats.o: stats.h stats.cc
	$(CPP) $(BASE_CFLAGS) stats.cc

common.o: common.h common.cc
	$(CPP) $(BASE_CFLAGS) common.cc

uploadReplay.o: uploadReplay.cc uploadReplay.h
	$(CPP) $(BASE_CFLAGS) uploadReplay.cc

#MenuSystem.o: MenuSystem.cc MenuSystem.h
#	$(CPP) $(BASE_CFLAGS) MenuSystem.cc

#ttfont.o: ttfont.h ttfont.cc
#	$(CPP) $(BASE_CFLAGS) ttfont.cc


run: $(BINARY)

clean:
	rm *o
