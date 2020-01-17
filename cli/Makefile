VPATH=../src
CXXFLAGS=-Wall -g -O -std=c++17 -I../src
CPPFLAGS=-MMD

OBJS_common  = file_util.o
OBJS_asm     = asm_operand.o
OBJS_dis     = dis_operand.o
OBJS_mc6809  = table_mc6809.o  reg_hd6309.o $(OBJS_common)
OBJS_hd6309  = table_hd6309.o  $(OBJS_mc6809)
OBJS_r6502   = table_r6502.o   $(OBJS_common)
OBJS_r65c02  = table_r65c02.o  $(OBJS_r6502)
OBJS_i8080   = table_i8080.o   reg_i8080.o  $(OBJS_common)
OBJS_z80     = table_z80.o     reg_z80.o    $(OBJS_common)
OBJS_tms9995 = table_tms9995.o $(OBJS_common)
OBJS_as6809   = as6809.o   $(OBJS_asm) $(OBJS_mc6809)  $(OBJS_common)
OBJS_as6309   = as6309.o   $(OBJS_asm) $(OBJS_hd6309)  $(OBJS_common)
OBJS_as6502   = as6502.o   $(OBJS_asm) $(OBJS_r6502)   $(OBJS_common)
OBJS_as65c02  = as65c02.o  $(OBJS_asm) $(OBJS_r65c02)  $(OBJS_common)
OBJS_as8080   = as8080.o   asm_i8080.o   $(OBJS_asm) $(OBJS_i8080)   $(OBJS_common)
OBJS_as80     = as80.o     asm_z80.o     $(OBJS_asm) $(OBJS_z80)     $(OBJS_common)
OBJS_as9995   = as9995.o   asm_tms9995.o $(OBJS_asm) $(OBJS_tms9995) $(OBJS_common)
OBJS_dis6809  = dis6809.o  $(OBJS_dis) $(OBJS_mc6809)  $(OBJS_common)
OBJS_dis6309  = dis6309.o  $(OBJS_dis) $(OBJS_hd6309)  $(OBJS_common)
OBJS_dis6502  = dis6502.o  $(OBJS_dis) $(OBJS_r6502)   $(OBJS_common)
OBJS_dis65c02 = dis65c02.o $(OBJS_dis) $(OBJS_r65c02)  $(OBJS_common)
OBJS_dis8080  = dis8080.o  dis_i8080.o   $(OBJS_dis) $(OBJS_i8080)   $(OBJS_common)
OBJS_dis80    = dis80.o    dis_z80.o     $(OBJS_dis) $(OBJS_z80)     $(OBJS_common)
OBJS_dis9995  = dis9995.o  dis_tms9995.o $(OBJS_dis) $(OBJS_tms9995) $(OBJS_common)

OBJS =	$(OBJS_as6809)  $(OBJS_dis6809)  \
	$(OBJS_as6309)  $(OBJS_dis6309)  \
	$(OBJS_as6502)  $(OBJS_dis6502)  \
	$(OBJS_as65c02) $(OBJS_dis65c02) \
	$(OBJS_as8080)  $(OBJS_dis8080)  \
	$(OBJS_as80)    $(OBJS_dis80)    \
	$(OBJS_as9995)  $(OBJS_dis9995)

OBJS_driver  = \
	as6809.o   \
	as6309.o   \
	as6502.o   \
	as65c02.o  \
	as8080.o   \
	as80.o     \
	as9995.o   \
	dis6809.o  \
	dis6309.o  \
	dis6502.o  \
	dis65c02.o \
	dis8080.o  \
	dis80.o    \
	dis9995.o

DRIVERS = $(OBJS_driver:.o=)

.PHONY: all clean clobber

all: $(OBJS_driver:.o=)

-include $(OBJS:.o=.d)

as6809:   $(OBJS_as6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
as6309:   $(OBJS_as6309)
	$(CXX) $(CXXFLAGS) $^ -o $@
as6502:   $(OBJS_as6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
as65c02:  $(OBJS_as65c02)
	$(CXX) $(CXXFLAGS) $^ -o $@
as8080:   $(OBJS_as8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
as80:     $(OBJS_as80)
	$(CXX) $(CXXFLAGS) $^ -o $@
as9995:   $(OBJS_as9995)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6809:  $(OBJS_dis6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6309:  $(OBJS_dis6309)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6502:  $(OBJS_dis6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis65c02: $(OBJS_dis65c02)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis8080:  $(OBJS_dis8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis80:    $(OBJS_dis80)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis9995:  $(OBJS_dis9995)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	-rm -f *.o *.d
clobber: clean
	-rm -f $(DRIVERS)
