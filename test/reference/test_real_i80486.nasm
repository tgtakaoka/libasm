;### Apache 2.0 — see test_i8086.nasm for full header
[bits 16]
[org 0x1000]
label_abs equ 0x1234
%include "test_i8086.i486.ninc"
%include "test_i80186.i486.ninc"
%include "test_i80286.i486.ninc"
%include "test_i80386.i486.ninc"
%include "test_i80486.i486.ninc"
%include "test_i8087.i486.ninc"
%include "test_i80287.i486.ninc"
%include "test_i80387.i486.ninc"
