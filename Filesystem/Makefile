#
# Makefile for the disk emulator
# Author: D.M. Lyons, Tom Kueny
# Date: Spring 2017
#
# A List of all the source files that should be compiled together
#
SOURCES = DISK_API.cpp Directory.cpp File.cpp UI.cc main.cc
#
# A list of the objects that result from the compilation process
#
OBJECTS = DISK_API.o Directory.o File.o UI.o main.o
#
# A list of include files on which theh compilation depends
#
DEFS = UI.h Directory.h File.h
#
# What flags to send to the compiler when you compile source files
#
CPPFLAGS = -g
#
# Any libraries that should be included in the linkage step of compilation
LIBFLAGS = -lm

#

atos: $(SOURCES) $(DEFS)
	g++ $(CPPFLAGS) $(SOURCES) -o atos $(LIBFLAGS)
