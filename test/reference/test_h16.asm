;;; Copyright 2026 Tadashi G. Takaoka
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at
;;;
;;;     http://www.apache.org/licenses/LICENSE-2.0
;;;
;;; Unless required by applicable law or agreed to in writing, software
;;; distributed under the License is distributed on an "AS IS" BASIS,
;;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;;; See the License for the specific language governing permissions and
;;; limitations under the License.

        .cpu     H16
        .org     0
        .include "test_h16.inc"

;;; BIT dynamic forms (0x68-0x6A per the HD641016 manual): libasm
;;; assembles the mnemonics directly.  ASL diverges (0x64-0x66), so its
;;; wrapper emits the manual's bytes as raw data instead.
        bset.b  r2,@r1                  ; 68 02 01
        bnot.b  r2,@r1                  ; 68 22 01
        bclr.b  r2,@r1                  ; 68 42 01
        btst.b  r2,@r1                  ; 68 62 01
        bset.w  r2,@r1                  ; 69 02 01
        bset.l  r2,@r1                  ; 6A 02 01
        bset.b  r14,@($10:8,r0)         ; 68 0E 10 10
