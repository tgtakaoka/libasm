;### Copyright 2026 Tadashi G. Takaoka
;###
;### Licensed under the Apache License, Version 2.0 (the "License");
;### you may not use this file except in compliance with the License.
;### You may obtain a copy of the License at
;###
;###     http://www.apache.org/licenses/LICENSE-2.0
;###
;### Unless required by applicable law or agreed to in writing, software
;### distributed under the License is distributed on an "AS IS" BASIS,
;### WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;### See the License for the specific language governing permissions and
;### limitations under the License.

;### nasm reference wrapper for the i8086 corpus.  The .ginc files are shared
;### with the GAS reference (test_i8086.gas) and the libasm reference
;### (test_i8086.asm); test/Makefile.nasm transforms them through
;### ginc-to-nasm.sed before %include here.
[bits 16]
[org 0x1000]
%include "test_i8086.ninc"
%include "test_i8086_far.ninc"
%include "test_i8087.ninc"
