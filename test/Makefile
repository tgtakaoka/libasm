VPATH=../src
CXXFLAGS=-Wall -g -std=c++17 -O -I$(VPATH)
TESTS=test_asm_mc6809 test_asm_hd6309 \
	test_dis_mc6809 test_dis_hd6309 \
	test_asm_i8080 test_dis_i8080
OBJS=asm_mc6809.o asm_hd6309.o dis_mc6809.o dis_hd6309.o table_mc6809.o table_hd6309.o\
	asm_i8080.o dis_i8080.o table_i8080.o string_utils.o

.PHONY: clean clean-obj test

all: test_mc6809 test_hd6309 test_i8080

test_mc6809: test_asm_mc6809 test_dis_mc6809
test_hd6309: test_asm_hd6309 test_dis_hd6309
test_i8080: test_asm_i8080 test_dis_i8080

test_asm_mc6809: test_asm_mc6809.o asm_mc6809.o table_mc6809.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_hd6309: test_asm_hd6309.o asm_hd6309.o table_hd6309.o asm_mc6809.o table_mc6809.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080: test_asm_i8080.o asm_i8080.o table_i8080.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809: test_dis_mc6809.o dis_mc6809.o table_mc6809.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_hd6309: test_dis_hd6309.o dis_hd6309.o table_hd6309.o dis_mc6809.o table_mc6809.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080: test_dis_i8080.o dis_i8080.o table_i8080.o string_utils.o
	$(CXX) -o $@ $(CXXFLAGS) $^

test_asm_mc6809.o: asm_mc6809.h test_asm_helper.h
test_asm_hd6309.o: asm_hd6309.h test_asm_helper.h
test_asm_i8080.o: asm_i8080.h test_asm_helper.h
test_dis_mc6809.o: dis_mc6809.h test_dis_helper.h
test_dis_hd6309.o: dis_hd6309.h test_dis_helper.h
test_dis_i8080.o: dis_i8080.h test_dis_helper.h

test_asm_helper.h: error_reporter.h symbol_table.h
test_dis_helper.h: error_reporter.h symbol_table.h

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o
