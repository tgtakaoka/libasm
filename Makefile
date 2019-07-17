CXXFLAGS=-Wall -g -O -std=c++17

all: asm6809.o

asm6809.o: asm6809.h

asm6809.h: config_6809.h insn.h
config_6809.h: config_host.h
insn.h: error_reporter.h config_host.h config_target.h
error_reporter.h : config_host.h

