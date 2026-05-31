;### Apache 2.0 -- see test_i8086.nasm for full header
[bits 16]
[org 0x1000]
label_abs equ 0x1234
%include "test_i8086.ninc"
%include "test_i80186.ninc"
%include "test_i80286.ninc"
%include "test_i80386.ninc"
%include "test_i8087.ninc"
%include "test_i80287.ninc"
%include "test_i80387.ninc"
