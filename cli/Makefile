VPATH=../src
CXXFLAGS=-Wall -g -O -std=c++17 -I../src
CPPFLAGS=-MMD

OBJS_common  = file_util.o
OBJS_asm     = asm_operand.o
OBJS_dis     = dis_operand.o
OBJS_mc6809  = table_mc6809.o  reg_mc6809.o  $(OBJS_common)
OBJS_m6502   = table_m6502.o   reg_m6502.o   $(OBJS_common)
OBJS_i8080   = table_i8080.o   reg_i8080.o   $(OBJS_common)
OBJS_z80     = table_z80.o     reg_z80.o     $(OBJS_common)
OBJS_tms9900 = table_tms9900.o reg_tms9900.o $(OBJS_common)
OBJS_mc68000 = table_mc68000.o reg_mc68000.o $(OBJS_common)
OBJS_as6809   = as6809.o   asm_mc6809.o  $(OBJS_asm) $(OBJS_mc6809)  $(OBJS_common)
OBJS_as6502   = as6502.o   asm_m6502.o   $(OBJS_asm) $(OBJS_m6502)   $(OBJS_common)
OBJS_as8080   = as8080.o   asm_i8080.o   $(OBJS_asm) $(OBJS_i8080)   $(OBJS_common)
OBJS_asz80    = asz80.o    asm_z80.o     $(OBJS_asm) $(OBJS_z80)     $(OBJS_common)
OBJS_as9900   = as9900.o   asm_tms9900.o $(OBJS_asm) $(OBJS_tms9900) $(OBJS_common)
OBJS_dis6809  = dis6809.o  dis_mc6809.o  $(OBJS_dis) $(OBJS_mc6809)  $(OBJS_common)
OBJS_dis6502  = dis6502.o  dis_m6502.o   $(OBJS_dis) $(OBJS_m6502)   $(OBJS_common)
OBJS_dis8080  = dis8080.o  dis_i8080.o   $(OBJS_dis) $(OBJS_i8080)   $(OBJS_common)
OBJS_disz80   = disz80.o   dis_z80.o     $(OBJS_dis) $(OBJS_z80)     $(OBJS_common)
OBJS_dis9900  = dis9900.o  dis_tms9900.o $(OBJS_dis) $(OBJS_tms9900) $(OBJS_common)
OBJS_dis68000 = dis68000.o dis_mc68000.o $(OBJS_dis) $(OBJS_mc68000) $(OBJS_common)

OBJS =	$(OBJS_as6809)  $(OBJS_dis6809)  \
	$(OBJS_as6502)  $(OBJS_dis6502)  \
	$(OBJS_as8080)  $(OBJS_dis8080)  \
	$(OBJS_asz80)   $(OBJS_disz80)   \
	$(OBJS_as9900)  $(OBJS_dis9900)

OBJS_driver  = \
	as6809.o   \
	as6502.o   \
	as8080.o   \
	asz80.o    \
	as9900.o   \
	dis6809.o  \
	dis6502.o  \
	dis8080.o  \
	disz80.o   \
	dis9900.o  \
	dis68000.o

DRIVERS = $(OBJS_driver:.o=)

.PHONY: all clean clobber

all: $(OBJS_driver:.o=)

-include $(OBJS:.o=.d)

as6809:   $(OBJS_as6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
as6502:   $(OBJS_as6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
as8080:   $(OBJS_as8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
asz80:    $(OBJS_asz80)
	$(CXX) $(CXXFLAGS) $^ -o $@
as9900:   $(OBJS_as9900)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6809:  $(OBJS_dis6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6502:  $(OBJS_dis6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis8080:  $(OBJS_dis8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
disz80:   $(OBJS_disz80)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis9900:  $(OBJS_dis9900)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis68000: $(OBJS_dis68000)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	-rm -f *.o *.d
clobber: clean
	-rm -f $(DRIVERS)
