CXXFLAGS=-Wall -g -O -std=c++17

OBJS_hd6309 = registers_hd6309.o table_mc6809.o table_hd6309.o \
	string_utils.o
OBJS_r65c02 = table_r6502.o table_r65c02.o string_utils.o
OBJS_i8080 =  asm_i8080.o  dis_i8080.o  registers_i8080.o  table_i8080.o \
	string_utils.o
OBJS_z80 =    asm_z80.o    dis_z80.o    registers_z80.o    table_z80.o \
	string_utils.o

.PHONY: all clean-objs clean test

all: $(OBJS_hd6309) $(OBJS_r65c02) $(OBJS_i8080) $(OBJS_z80)

asm_i8080.o:  asm_i8080.h
asm_z80.o:    asm_z80.h
dis_i8080.o:  dis_i8080.h  string_utils.h table_i8080.h
dis_z80.o:    dis_z80.h    string_utils.h table_z80.h
registers_hd6309.o: registers_hd6309.h
registers_i8080.o:  registers_i8080.h
registers_z80.o:  registers_z80.h
string_utils.o: config_host.h string_utils.h
table_mc6809.o: config_hd6309.h entry_hd6309.h table_hd6309.h text_mc6809.h
table_hd6309.o: config_hd6309.h entry_hd6309.h table_hd6309.h text_hd6309.h
table_r6502.o:  config_r65c02.h entry_r65c02.h table_r65c02.h text_r6502.h
table_r65c02.o: config_r65c02.h entry_r65c02.h table_r65c02.h text_r65c02.h
table_i8080.o:  config_i8080.h  entry_i8080.h  table_i8080.h  text_i8080.h
table_z80.o:    config_z80.h    entry_z80.h    table_z80.h    text_z80.h

asm_hd6309.h: asm_hd6309_impl.h config_hd6309.h error_reporter.h symbol_table.h table_hd6309.h
asm_hd6309_impl.h:
asm_r65c02.h: asm_r65c02_impl.h config_r65c02.h error_reporter.h symbol_table.h table_r65c02.h
asm_r65c02_impl.h:
asm_i8080.h:  config_i8080.h  error_reporter.h symbol_table.h table_i8080.h
asm_z80.h:    config_z80.h    error_reporter.h symbol_table.h table_z80.h
config_hd6309.h: config_host.h error_hd6309.h registers_hd6309.h
config_r65c02.h: config_host.h error_r65c02.h
config_i8080.h:  config_host.h error_i8080.h  registers_i8080.h
config_z80.h:    config_host.h error_z80.h    registers_z80.h
config_host.h:
dis_hd6309.h: dis_hd6309_impl.h config_hd6309.h error_reporter.h insn_hd6309.h memory.h symbol_table.h
dis_hd6309_impl.h: string_utils.h table_hd6309.h
dis_r65c02.h: dis_r65c02_impl.h config_r65c02.h error_reporter.h insn_r65c02.h memory.h symbol_table.h
dis_r65c02_impl.h: string_utils.h table_r65c02.h
dis_i8080.h:  config_i8080.h  error_reporter.h insn_i8080.h  memory.h symbol_table.h
dis_z80.h:    config_z80.h    error_reporter.h insn_z80.h    memory.h symbol_table.h
entry_hd6309.h: config_hd6309.h
entry_r65c02.h: config_r65c02.h
entry_i8080.h:  config_i8080.h
entry_z80.h:    config_z80.h
error_hd6309.h: config_host.h
error_r65c02.h: config_host.h
error_i8080.h:  config_host.h
error_z80.h:    config_host.h
error_reporter.h : config_host.h
insn_hd6309.h: entry_hd6309.h
insn_r65c02.h: entry_r65c02.h
insn_i8080.h:  entry_i8080.h
insn_z80.h:    entry_z80.h
memory.h:
string_utils.h:
symbol_table.h: config_host.h
table_hd6309.h: config_hd6309.h insn_hd6309.h
table_r65c02.h: config_r65c02.h insn_r65c02.h
table_i8080.h:  config_i8080.h  insn_i8080.h
table_z80.h:    config_z80.h    insn_z80.h
text_hd6309.h: config_host.h
text_r65c02.h: config_host.h
text_i8080.h:  config_host.h
text_z80.h:    config_host.h

clean-objs:
	-rm -f $(OBJS_hd6309) $(OBJS_r65c02) $(OBJS_i8080) $(OBJS_z80)

clean: clean-objs
