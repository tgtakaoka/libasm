CXXFLAGS=-Wall -g -O -std=c++17
CPPFLAGS=-MMD

OBJS_mc6809  = asm_mc6809.o  dis_mc6809.o  table_mc6809.o reg_mc6809.o
OBJS_m6502   = asm_m6502.o   dis_m6502,o   table_m6502.o
OBJS_i8080   = asm_i8080.o   dis_i8080.o   table_i8080.o  reg_i8080.o
OBJS_tms9900 = asm_tms9900.o dis_tms9900.o table_tms9900.o
OBJS_z80     = asm_z80.o     dis_z80.o     table_z80.o    reg_z80.o
OBJS = \
	$(OBJS_mc6809)  \
	$(OBJS_m6502)   \
	$(OBJS_i8080)   \
	$(OBJS_z80)     \
	$(OBJS_tms9900)

.PHONY: all objs clean test

all: objs
objs: $(OBJS)

-include $(OBJS:.o=.d)

clean:
	-rm -f $(OBJS) $(OBJS:.o=.d)
