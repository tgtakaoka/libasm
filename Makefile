CXXFLAGS=-Wall -g -O -std=c++17

OBJS_6809 = asm6809.o table_6809.o
OBJS_6309 = asm6309.o table_6309.o

.PHONY: clean-objs clean

all: $(OBJS_6809) $(OBJS_6309)

asm6809.o: asm6809.h
asm6309.o: asm6309.h

table_6809.o: config_6809.h table.h text_6809.h
table_6309.o: config_6309.h table.h text_6309.h

asm6809.h: config_6809.h insn.h
asm6309.h: config_6309.h insn.h
config_6809.h: config_host.h
config_6309.h: config_host.h
insn.h: error_reporter.h config_host.h config_target.h
error_reporter.h : config_host.h
table.h: config_host.h config_target.h
text_6809.h: string_utils.h
text_6309.h: string_utils.h

clean-objs:
	-rm -f $(OBJS_6809) $(OBJS_6309)

clean: clean-objs

