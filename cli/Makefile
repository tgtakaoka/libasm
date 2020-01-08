VPATH=../src
CXXFLAGS=-Wall -g -O -std=c++17 -I../src
CPPFLAGS=-MMD

OBJS_common = asm_operand.o file_util.o
OBJS_mc6809 = table_mc6809.o  reg_hd6309.o $(OBJS_common)
OBJS_hd6309 = table_hd6309.o  $(OBJS_mc6809)
OBJS_r6502  = table_r6502.o                $(OBJS_common)
OBJS_r65c02 = table_r65c02.o  $(OBJS_r6502)
OBJS_i8080  = asm_i8080.o   table_i8080.o   reg_i8080.o  $(OBJS_common)
OBJS_z80    = asm_z80.o     table_z80.o     reg_z80.o    $(OBJS_common)
OBJS_tms9995= asm_tms9995.o table_tms9995.o              $(OBJS_common)
OBJS_driver =    \
	as6809.o \
	as6309.o \
	as6502.o \
	as65c02.o \
	as8080.o \
	as80.o \
	as9995.o

OBJS =	$(OBJS_common) \
	$(OBJS_driver) \
	$(OBJS_hd6309) \
	$(OBJS_r65c02) \
	$(OBJS_i8080)  \
	$(OBJS_z80)    \
	$(OBJS_tms9995)

DRIVERS = $(OBJS_driver:.o=)

.PHONY: all clean test

all: $(OBJS_driver:.o=)

-include $(OBJS:.o=.d)

as6809:  as6809.o  $(OBJS_mc6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
as6309:  as6309.o  $(OBJS_hd6309)
	$(CXX) $(CXXFLAGS) $^ -o $@
as6502:  as6502.o  $(OBJS_r6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
as65c02: as65c02.o $(OBJS_r65c02)
	$(CXX) $(CXXFLAGS) $^ -o $@
as8080:  as8080.o  $(OBJS_i8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
as80:   as80.o     $(OBJS_z80)
	$(CXX) $(CXXFLAGS) $^ -o $@
as9995:  as9995.o  $(OBJS_tms9995)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	-rm -f *.o *.d
	-rm -f $(DRIVERS)
