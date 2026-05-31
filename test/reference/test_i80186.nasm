;### Copyright 2026 Tadashi G. Takaoka
;### (Apache 2.0 — see test_i8086.nasm for full header)
[bits 16]
[org 0x1000]
%include "test_i8086.ninc"
%include "test_i8086_far.ninc"
%include "test_i80186.ninc"
%include "test_i8087.ninc"
%include "test_i80c187.ninc"
