CXXFLAGS=-Wall -g -O -std=c++17
CPPFLAGS=-MMD

OBJS_hd6309  = table_mc6809.o table_hd6309.o reg_hd6309.o
OBJS_r65c02  = table_r6502.o table_r65c02.o
OBJS_i8080   = asm_i8080.o   dis_i8080.o   table_i8080.o reg_i8080.o
OBJS_tms9995 = asm_tms9995.o dis_tms9995.o table_tms9995.o
OBJS_z80     = asm_z80.o     dis_z80.o     table_z80.o   reg_z80.o
OBJS = $(OBJS_hd6309) $(OBJS_r65c02) $(OBJS_i8080) $(OBJS_tms9995) $(OBJS_z80)

.PHONY: all objs clean test

all: objs
objs: $(OBJS)

-include $(OBJS:.o=.d)

clean:
	-rm -f $(OBJS) $(OBJS:.o=.d)
