CXXFLAGS=-Wall -g -O -std=c++17

OBJS_hd6309 = asm_hd6309.o dis_hd6309.o registers_hd6309.o table_hd6309.o \
	string_utils.o
OBJS_i8080 =  asm_i8080.o  dis_i8080.o  registers_i8080.o  table_i8080.o \
	string_utils.o

.PHONY: clean-objs clean test

all: $(OBJS_hd6309) $(OBJS_i8080)

asm_hd6309.o: asm_hd6309.h
asm_i8080.o:  asm_i8080.h
dis_hd6309.o: dis_hd6309.h string_utils.h table_hd6309.h
dis_i8080.o:  dis_i8080.h  string_utils.h table_i8080.h
registers_hd6309.o: registers_hd6309.h
registers_i8080.o:  registers_i8080.h
string_utils.o: config_host.h string_utils.h
table_hd6309.o: config_hd6309.h entry_hd6309.h table_hd6309.h text_hd6309.h
table_i8080.o:  config_i8080.h  entry_i8080.h  table_i8080.h  text_i8080.h

asm_hd6309.h: config_hd6309.h error_reporter.h symbol_table.h table_hd6309.h
asm_i8080.h:  config_i8080.h  error_reporter.h symbol_table.h table_i8080.h
config_hd6309.h: config_host.h error_hd6309.h registers_hd6309.h
config_i8080.h:  config_host.h error_i8080.h  registers_i8080.h
config_host.h:
dis_hd6309.h: config_hd6309.h error_reporter.h insn_hd6309.h memory.h symbol_table.h
dis_i8080.h:  config_i8080.h  error_reporter.h insn_i8080.h  memory.h symbol_table.h
entry_hd6309.h: config_hd6309.h
entry_i8080.h:  config_i8080.h
error_hd6309.h: config_host.h
error_i8080.h:  config_host.h
error_reporter.h : config_host.h
insn_hd6309.h: entry_hd6309.h
insn_i8080.h:  entry_i8080.h
memory.h:
string_utils.h:
symbol_table.h: config_host.h
table_hd6309.h: config_hd6309.h insn_hd6309.h
table_i8080.h:  config_i8080.h  insn_i8080.h
text_hd6309.h: config_host.h
text_i8080.h:  config_host.h

clean-objs:
	-rm -f $(OBJS_hd6309) $(OBJS_i8080)

clean: clean-objs
