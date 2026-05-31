;### Apache 2.0 — see test_i8086.nasm for full header
[bits 32]
[org 0x1000]
label_abs equ 0x12345678
%include "test_i8086.b32.ninc"
%include "test_i80186.b32.ninc"
%include "test_i80286.b32.ninc"
%include "test_i80386.b32.ninc"
%include "test_i8087.b32.ninc"
%include "test_i80287.b32.ninc"
%include "test_i80387.b32.ninc"
