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

;;; HD641016 (H16) instruction set reference.
;;;
;;; Accumulator form: when destination is R0, the EAs byte has bit 7 set
;;; and no EAd byte follows.  This saves one byte for R0 destinations.

;;; ============================================================
;;; ADD:G (0x00-0x02), SUB:G (0x04-0x06)
;;; CMP:G (0x08-0x0A), MOV:G (0x0C-0x0E)
;;; ============================================================

;;; Register direct
        add:g.b r0,r1                   ; 00 40 41
        add:g.w r0,r1                   ; 01 40 41
        add:g.l r0,r1                   ; 02 40 41
        add:g.b r15,r14                 ; 00 4F 4E
;;; Accumulator form (dest = R0, EAs bit7=1, no EAd)
        add:g.w r1,r0                   ; 01 C1
        add:g.l r1,r0                   ; 02 C1

;;; Register indirect @Rn (Sd=00)
        add:g.w @r0,r1                  ; 01 00 41
        add:g.w @r1,r0                  ; 01 81
;;; Register indirect @(d:8,Rn) (Sd=01)
        add:g.w @($10:8,r0),r1        ; 01 10 10 41
        add:g.w @($10:8,r1),r0        ; 01 91 10
;;; Register indirect @(d:16,Rn) (Sd=10)
        add:g.w @($1000:16,r0),r1     ; 01 20 10 00 41
        add:g.w @($1000:16,r1),r0     ; 01 A1 10 00
;;; Register indirect @(d:32,Rn) (Sd=11)
        add:g.w @($10000:32,r0),r1    ; 01 30 00 01 00 00 41

;;; Auto-increment @Rn+
        add:g.w @r0+,r1                 ; 01 50 41
        add:g.w @r1+,r0                 ; 01 D1
;;; Auto-decrement @-Rn
        add:g.w @-r0,r1                 ; 01 60 41
        add:g.w @-r1,r0                 ; 01 E1

;;; Immediate #imm (Si=01 byte, 10 word, 11 long)
        add:g.w #$12:8,r0             ; 01 F1 12
        add:g.w #$1234:16,r0          ; 01 F2 12 34
        add:g.w #$12345678:32,r0      ; 01 F3 12 34 56 78
        add:g.w #$12:8,r1            ; 01 71 12 41
        add:g.w #$1234:16,r1         ; 01 72 12 34 41

;;; Absolute @addr (Sa=01 byte, 10 word, 11 long)
        add:g.w @$12:8,r0             ; 01 F5 12
        add:g.w @$1234:16,r0          ; 01 F6 12 34
        add:g.w @$123456:32,r0        ; 01 F7 00 12 34 56
        add:g.w @$12:8,r1            ; 01 75 12 41
        add:g.w @$1234:16,r1         ; 01 76 12 34 41

;;; Register indirect with scale @Rn*Sf
        add:g.w @r0*2,r1               ; 01 78 10 41
        add:g.w @r0*4,r1               ; 01 78 20 41
        add:g.w @r0*8,r1               ; 01 78 30 41
;;; Register indirect with scale @(d:8,Rn*Sf)
        add:g.w @($10:8,r0*2),r1     ; 01 79 10 10 41
        add:g.w @($10:8,r0*4),r0     ; 01 F9 20 10

;;; Register indirect with index @([d,]Xm[.Sz][*Sf],Rn)
        add:g.w @(r2,r0),r1            ; 01 7C 42 00 41
        add:g.w @($10:8,r2.w*2,r0),r1 ; 01 7C 10 12 10 41

;;; PC-relative @(disp,PC): displacement is computed from absolute address in ASL
;;; Use a forward label to get a known displacement
pc_rel_test:
        add:g.w @(pc_rel_end:8,pc),r1 ; 01 7D 90 01 41  (disp = pc_rel_end - PC, PC=EAd byte)
pc_rel_end:

;;; Register double indirect @@Rn
        add:g.w @@r0,r1                ; 01 7E 50 00 00 41
        add:g.w @($10:8,@($20:8,r0)),r1 ; 01 7E 50 20 10 41

;;; SUB:G
        sub:g.b r0,r1                   ; 04 40 41
        sub:g.w r0,r1                   ; 05 40 41
        sub:g.l r0,r1                   ; 06 40 41
        sub:g.w @r0,r1                  ; 05 00 41
        sub:g.w r1,r0                   ; 05 C1
        sub:g.w #$10:8,r0             ; 05 F1 10
        sub:g.w @$1234:16,r1         ; 05 76 12 34 41

;;; CMP:G
        cmp:g.b r0,r1                   ; 08 40 41
        cmp:g.w r0,r1                   ; 09 40 41
        cmp:g.l r0,r1                   ; 0A 40 41
        cmp:g.w @r0,r1                  ; 09 00 41
        cmp:g.w r1,r0                   ; 09 C1
        cmp:g.w #$10:8,r0             ; 09 F1 10
        cmp:g.w #$1234:16,r1         ; 09 72 12 34 41

;;; MOV:G
        mov:g.b r0,r1                   ; 0C 40 41
        mov:g.w r0,r1                   ; 0D 40 41
        mov:g.l r0,r1                   ; 0E 40 41
        mov:g.w r0,@r1                  ; 0D 40 01
        mov:g.w r0,@-r1                 ; 0D 40 61
        mov:g.w r0,@r1+                 ; 0D 40 51
        mov:g.w @r0,r1                  ; 0D 00 41
        mov:g.w @r0+,r1                 ; 0D 50 41
        mov:g.w @-r0,r1                 ; 0D 60 41
        mov:g.w r1,r0                   ; 0D C1
        mov:g.w @r1,r0                  ; 0D 81
        mov:g.w #$10:8,r0             ; 0D F1 10
        mov:g.w #$1234:16,r0          ; 0D F2 12 34
        mov:g.w #$12345678:32,r0      ; 0D F3 12 34 56 78
        mov:g.w @$1234:16,r0          ; 0D F6 12 34
        mov:g.w r0,@$1234:16          ; 0D 40 76 12 34

;;; ============================================================
;;; ADD:Q (0x10-0x12) -- quick immediate (8-bit signed)
;;; CLR (0x14-0x16)
;;; CMP:Q (0x18-0x1A)
;;; MOV:Q (0x1C-0x1E)
;;; Format: [opcode][imm8][EAd]
;;; ============================================================
        add:q.b #$10,r0                 ; 10 10 40
        add:q.w #$10,r0                 ; 11 10 40
        add:q.l #$10,r0                 ; 12 10 40
        add:q.b #-$10,r0               ; 10 F0 40
        add:q.w #$7F,r1               ; 11 7F 41
        add:q.w #$10,@r0               ; 11 10 00

        clr.b r0                        ; 14 40
        clr.w r0                        ; 15 40
        clr.l r0                        ; 16 40
        clr.b r1                        ; 14 41
        clr.w @r0                       ; 15 00

        cmp:q.b #$10,r0                 ; 18 10 40
        cmp:q.w #$10,r0                 ; 19 10 40
        cmp:q.l #$10,r0                 ; 1A 10 40
        cmp:q.b #-$10,r0               ; 18 F0 40
        cmp:q.w #$10,@r0               ; 19 10 00

        mov:q.b #$10,r0                 ; 1C 10 40
        mov:q.w #$10,r0                 ; 1D 10 40
        mov:q.l #$10,r0                 ; 1E 10 40
        mov:q.w #-$10,r1               ; 1D F0 41

;;; ============================================================
;;; ADD:R (0x20-0x22) SUB:R (0x24-0x26)
;;; CMP:R (0x28-0x2A) MOV:R (0x2C-0x2E)
;;; Format: [opcode][Rns(4)|Rnd(4)]
;;; ============================================================
        add:r.b r0,r1                   ; 20 01
        add:r.w r0,r1                   ; 21 01
        add:r.l r0,r1                   ; 22 01
        add:r.w r7,r15                  ; 21 7F
        sub:r.b r0,r1                   ; 24 01
        sub:r.w r0,r1                   ; 25 01
        sub:r.l r0,r1                   ; 26 01
        cmp:r.b r0,r1                   ; 28 01
        cmp:r.w r0,r1                   ; 29 01
        cmp:r.l r0,r1                   ; 2A 01
        mov:r.b r0,r1                   ; 2C 01
        mov:r.w r0,r1                   ; 2D 01
        mov:r.l r0,r1                   ; 2E 01

;;; ============================================================
;;; ADD:RQ (0x30-0x32) SUB:RQ (0x34-0x36)
;;; CMP:RQ (0x38-0x3A) MOV:RQ (0x3C-0x3E)
;;; Format: [opcode][Rnd(4)|Imm4(4)]
;;; ============================================================
        add:rq.b #5,r0                  ; 30 05
        add:rq.w #5,r0                  ; 31 05
        add:rq.l #5,r0                  ; 32 05
        add:rq.w #7,r1                  ; 31 17
        sub:rq.b #5,r0                  ; 34 05
        sub:rq.w #5,r0                  ; 35 05
        sub:rq.l #5,r0                  ; 36 05
        cmp:rq.b #5,r0                  ; 38 05
        cmp:rq.w #5,r0                  ; 39 05
        cmp:rq.l #5,r0                  ; 3A 05
        mov:rq.b #5,r0                  ; 3C 05
        mov:rq.w #5,r0                  ; 3D 05
        mov:rq.l #5,r0                  ; 3E 05

;;; ============================================================
;;; ADDS (0x40-0x42) SUBS (0x44-0x46)
;;; CMPS (0x48-0x4A) MOVS (0x4C-0x4E)
;;; ADDX (0x50-0x52) SUBX (0x54-0x56)
;;; ============================================================
        adds.b r0,r1                    ; 40 40 41
        adds.w r0,r1                    ; 41 40 41
        adds.l r0,r1                    ; 42 40 41
        adds.b r1,r0                    ; 40 C1
        subs.b r0,r1                    ; 44 40 41
        subs.w r0,r1                    ; 45 40 41
        cmps.b r0,r1                    ; 48 40 41
        cmps.w r0,r1                    ; 49 40 41
        movs.b r0,r1                    ; 4C 40 41
        movs.w r0,r1                    ; 4D 40 41

        addx.b r0,r1                    ; 50 40 41
        addx.w r0,r1                    ; 51 40 41
        addx.l r0,r1                    ; 52 40 41
        addx.b r1,r0                    ; 50 C1
        addx.w @r0,r1                   ; 51 00 41
        subx.b r0,r1                    ; 54 40 41
        subx.w r0,r1                    ; 55 40 41
        subx.l r0,r1                    ; 56 40 41

;;; ============================================================
;;; TST (0x58-0x5A)  MOVF (0x5C-0x5E)
;;; ============================================================
        tst.b r0                        ; 58 40
        tst.w r0                        ; 59 40
        tst.l r0                        ; 5A 40
        tst.b @r0                       ; 58 00
        tst.w @($10:8,r0)             ; 59 10 10

        movf.b r1                       ; 5C 41
        movf.w r1                       ; 5D 41
        movf.l r1                       ; 5E 41
        movf.w @r1                      ; 5D 01

;;; ============================================================
;;; SFT -- Shift/rotate (2-byte opcode)
;;; Dynamic (0x60-0x62): [opcode][MODE(3)*Rn(4)][EAd]
;;; Static  (0x64-0x66): [opcode][MODE(3)Imm5(5)][EAd]
;;; MODE: SHAR=000 SHLR=001 ROTR=010 ROTXR=011
;;;       SHAL=100 SHLL=101 ROTL=110 ROTXL=111
;;; ============================================================

;;; Dynamic: shift count in register
        shar.b r2,r1                    ; 60 02 41
        shlr.b r2,r1                    ; 60 22 41
        rotr.b r2,r1                    ; 60 42 41
        rotxr.b r2,r1                   ; 60 62 41
        shal.b r2,r1                    ; 60 82 41
        shll.b r2,r1                    ; 60 A2 41
        rotl.b r2,r1                    ; 60 C2 41
        rotxl.b r2,r1                   ; 60 E2 41
        shal.w r2,r1                    ; 61 82 41
        shal.l r2,r1                    ; 62 82 41
        shal.w r2,@r0                   ; 61 82 00

;;; Static: shift count as 5-bit immediate (0-31)
        shar.b #3,r1                    ; 64 03 41
        shlr.b #3,r1                    ; 64 23 41
        rotr.b #3,r1                    ; 64 43 41
        rotxr.b #3,r1                   ; 64 63 41
        shal.b #3,r1                    ; 64 83 41
        shll.b #3,r1                    ; 64 A3 41
        rotl.b #3,r1                    ; 64 C3 41
        rotxl.b #3,r1                   ; 64 E3 41
        shal.w #5,r1                    ; 65 85 41
        shal.l #5,r1                    ; 66 85 41
        shal.b #$1F,r1               ; 64 9F 41

;;; ============================================================
;;; BIT -- Bit manipulation (2-byte opcode)
;;; Dynamic (0x68-0x6A): [opcode][*MODE(2)*Rn(4)][EAd]
;;; Static  (0x6C-0x6E): [opcode][*MODE(2)Imm5(5)][EAd]
;;; MODE: BSET=00 BNOT=01 BCLR=10 BTST=11
;;;
;;; Note: ASL encodes BIT dynamic with 0x64-0x66 (same as SFT static).
;;; libasm uses 0x68-0x6A per the HD641016 manual.  ASL assembles BIT
;;; dynamic instructions using raw dc.b directives below.
;;; ============================================================

;;; Dynamic: bit number in register (0x68-0x6A per manual)
        dc.b $68,$02,$01                ; 68 02 01  bset.b r2,@r1
        dc.b $68,$22,$01                ; 68 22 01  bnot.b r2,@r1
        dc.b $68,$42,$01                ; 68 42 01  bclr.b r2,@r1
        dc.b $68,$62,$01                ; 68 62 01  btst.b r2,@r1
        dc.b $69,$02,$01                ; 69 02 01  bset.w r2,@r1
        dc.b $6A,$02,$01                ; 6A 02 01  bset.l r2,@r1
        dc.b $68,$0E,$10,$10            ; 68 0E 10 10  bset.b r14,@(H'10:8,r0)

;;; Static: bit number as 5-bit immediate
        bset.b #3,@r1                   ; 6C 03 01
        bnot.b #3,@r1                   ; 6C 23 01
        bclr.b #3,@r1                   ; 6C 43 01
        btst.b #3,@r1                   ; 6C 63 01
        bset.w #$F,@r1               ; 6D 0F 01
        bset.l #$1F,@r1             ; 6E 1F 01
        bclr.b #0,r0                    ; 6C 40 40

;;; ============================================================
;;; MOVTPE (0x78-0x7A)  MOVFPE (0x7C-0x7E) -- peripheral with E-clock
;;; ============================================================
        movtpe.w r0,@r1                 ; 79 40 01
        movfpe.w @r0,r1                 ; 7D 00 41

;;; ============================================================
;;; AND (0x80-0x82)  DADD (0x83)
;;; XOR (0x84-0x86)  DSUB (0x87)
;;; OR  (0x88-0x8A)  NEG  (0x8C-0x8E)
;;; ============================================================
        and.b r0,r1                     ; 80 40 41
        and.w r0,r1                     ; 81 40 41
        and.l r0,r1                     ; 82 40 41
        and.b r1,r0                     ; 80 C1
        and.w #$FF:8,r0              ; 81 F1 FF
        and.w @r0,r1                    ; 81 00 41

        dadd r0,r1                      ; 83 40 41
        dadd r1,r0                      ; 83 C1

        xor.b r0,r1                     ; 84 40 41
        xor.w r0,r1                     ; 85 40 41
        xor.l r0,r1                     ; 86 40 41
        xor.w r1,r0                     ; 85 C1

        dsub r0,r1                      ; 87 40 41
        dsub r1,r0                      ; 87 C1

        or.b r0,r1                      ; 88 40 41
        or.w r0,r1                      ; 89 40 41
        or.l r0,r1                      ; 8A 40 41
        or.w r1,r0                      ; 89 C1

        neg.b r0                        ; 8C 40
        neg.w r0                        ; 8D 40
        neg.l r0                        ; 8E 40
        neg.b @r0                       ; 8C 00

;;; ============================================================
;;; NOT (0x90-0x92)
;;; BRA (0x98-0x9A)  JMP (0x9B)
;;; NEGX (0x9C-0x9E)
;;; ============================================================
        not.b r0                        ; 90 40
        not.w r0                        ; 91 40
        not.l r0                        ; 92 40
        not.b @r0                       ; 90 00

not_end:
        bra.b *+2                       ; 98 00
        bra.w *+3                       ; 99 00 00
        bra.b *+$A                     ; 98 08

        jmp @r0                         ; 9B 00
        jmp @($1234:16,r0)            ; 9B 20 12 34
        jmp @r15                        ; 9B 0F

        negx.b r0                       ; 9C 40
        negx.w r0                       ; 9D 40
        negx.l r0                       ; 9E 40
        negx.b @r0                      ; 9C 00

;;; ============================================================
;;; BEQ (0xA0-0xA2)
;;; Bcc:G (0xA4-0xA6): [opcode][cc(8)][disp]
;;; BSR (0xA8-0xAA)  JSR (0xAB)
;;; EXTU (0xAC-0xAE)  DNEG (0xAF)
;;; ============================================================
        beq.b *+2                       ; A0 00
        beq.w *+3                       ; A1 00 00
        beq.b *+$A                     ; A0 08

;;; Bcc:G: cc byte uses condition code from Table 16-8
;;; T=0x00 F=0x01 HI=0x02 LS=0x03 CC=0x04 CS=0x05
;;; NE=0x06 EQ=0x07 VC=0x08 VS=0x09 PL=0x0A MI=0x0B
;;; GE=0x0C LT=0x0D GT=0x0E LE=0x0F
        bhi:g.b *+3                     ; A4 02 00
        bls:g.b *+3                     ; A4 03 00
        bcc:g.b *+3                     ; A4 04 00
        bcs:g.b *+3                     ; A4 05 00
        bne:g.b *+3                     ; A4 06 00
        beq:g.b *+3                     ; A4 07 00
        bge:g.b *+3                     ; A4 0C 00
        blt:g.b *+3                     ; A4 0D 00
        bgt:g.b *+3                     ; A4 0E 00
        ble:g.b *+3                     ; A4 0F 00
        bpl:g.b *+3                     ; A4 0A 00
        bmi:g.b *+3                     ; A4 0B 00
        bvc:g.b *+3                     ; A4 08 00
        bvs:g.b *+3                     ; A4 09 00
        bt:g.b *+3                      ; A4 00 00
        bf:g.b *+3                      ; A4 01 00
        bhi:g.w *+4                     ; A5 02 00 00

        bsr.b *+2                       ; A8 00
        bsr.w *+3                       ; A9 00 00
        bsr.b *+$A                     ; A8 08

        jsr @r0                         ; AB 00
        jsr @($1234:16,r0)            ; AB 20 12 34
        jsr @$1234:16                   ; AB 76 12 34

;;; EXTU: extend lower part as unsigned (zero-extend)
        extu.w r0                       ; AC 00
        extu.l r0                       ; AD 00
        extu.b r0                       ; AE 00
        extu.w r7                       ; AC 07

;;; DNEG: decimal negate with CX flag
        dneg.b r0                       ; AF 40
        dneg.b @r0                      ; AF 00

;;; ============================================================
;;; BNE (0xB0-0xB2)  XCH (0xB3)
;;; RTD (0xB8-0xBA)  RTS (0xBB)
;;; EXTS (0xBC-0xBE)  MOVA (0xBF)
;;; ============================================================
        bne.b *+2                       ; B0 00
        bne.w *+3                       ; B1 00 00
        bne.b *+$A                     ; B0 08

;;; XCH: exchange two registers (long word only)
        xch.l r0,r1                     ; B3 01
        xch.l r1,r14                    ; B3 1E

;;; RTD: return and deallocate (Sd=disp size)
        rtd #$10                        ; B8 10
        rtd #$200                       ; B9 02 00

        rts                             ; BB

;;; EXTS: extend lower part as signed (sign-extend)
        exts.w r0                       ; BC 00
        exts.l r0                       ; BD 00
        exts.b r0                       ; BE 00
        exts.w r7                       ; BC 07

;;; MOVA: move effective address into R0 (dest must be R0)
        mova @r1,r0                     ; BF 81
        mova @($1234:16,r0),r0        ; BF A0 12 34

;;; ============================================================
;;; LINK (0xD0-0xD2): [opcode][Rn(8)][frame-bytes]
;;; UNLK (0xD3): [0xD3][Rn(8)]
;;; ============================================================
        link r0,#$10                    ; D0 00 10
        link r7,#-$100                  ; D1 07 FF 00
        link r15,#$10000              ; D2 0F 00 01 00 00

        unlk r0                         ; D3 00
        unlk r7                         ; D3 07

;;; ============================================================
;;; MOVTP (0xE0): word-only, no E-clock
;;; MOVFP (0xE2): word-only, no E-clock
;;; SWAP (0xEB): swap register word halves
;;; ============================================================
        movtp.w r0,@r1                  ; E0 40 01
        movfp.w @r0,r1                  ; E2 00 41

        swap.w r0                       ; EB 40
        swap.w r1                       ; EB 41

;;; ============================================================
;;; CGBN (0xE4): push and change global bank number
;;; Format: [0xE4][Rn(8)]
;;; ============================================================
        cgbn r0                         ; E4 00
        cgbn r1                         ; E4 01
        cgbn r15                        ; E4 0F

;;; ============================================================
;;; MULXS (0xEE 2-byte)  DIVXS (0xEF 2-byte)
;;; Second byte: [MODE(4)][*4]
;;; MULXS.B=0x00 MULXS.W=0x10 MULXU.B=0x40 MULXU.W=0x50
;;; DIVXS.B=0x00 DIVXS.W=0x10 DIVXU.B=0x40 DIVXU.W=0x50
;;; Format: [0xEE/EF][mode][EAs][EAd]
;;; ============================================================
        mulxs.b r0,r1                   ; EE 00 40 41
        mulxs.w r0,r1                   ; EE 10 40 41
        mulxu.b r0,r1                   ; EE 40 40 41
        mulxu.w r0,r1                   ; EE 50 40 41
        divxs.b r0,r1                   ; EF 00 40 41
        divxu.b r0,r1                   ; EF 40 40 41

;;; ============================================================
;;; RESET (0xF0)  RTE (0xF1)
;;; TRAPA (0xF2): [0xF2][imm4(8)]
;;; RTR (0xF4)  SLEEP (0xF5)
;;; ============================================================
        reset                           ; F0
        rte                             ; F1
        trapa #0                        ; F2 00
        trapa #5                        ; F2 05
        trapa #$F                     ; F2 0F
        rtr                             ; F4
        sleep                           ; F5

;;; ============================================================
;;; ANDC (0xF8)  ORC (0xF9)  XORC (0xFA)
;;; LDC  (0xFB)  STC  (0xFC)
;;; Format: [opcode][CR-code(8)][EA]
;;; CR codes: CCR=0x20 VBNR=0x01 CBNR=0x40 BSP=0x41
;;;           BMR=0x80 GBNR=0x81 SR=0xA0
;;;           EBR=0xC0 RBR=0xC1 USP=0xC2 IBR=0xC3
;;; ============================================================
        andc r0,ccr                     ; F8 20 40
        andc r0,sr                      ; F8 A0 40
        andc r0,cbnr                    ; F8 40 40
        orc r0,ccr                      ; F9 20 40
        orc r0,sr                       ; F9 A0 40
        xorc r0,ccr                     ; FA 20 40
        xorc r0,sr                      ; FA A0 40
        ldc r0,cbnr                     ; FB 40 40
        ldc r0,vbnr                     ; FB 01 40
        ldc @r0,cbnr                    ; FB 40 00
        stc cbnr,r0                     ; FC 40 40
        stc vbnr,r0                     ; FC 01 40
        stc cbnr,@r0                    ; FC 40 00

;;; ============================================================
;;; ICBN (0xFD)  DCBN (0xFE)  NOP (0xFF)
;;; ============================================================
        icbn                            ; FD
        dcbn                            ; FE
        nop                             ; FF
