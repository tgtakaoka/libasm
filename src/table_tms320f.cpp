/*
 * Copyright 2025 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "table_tms320f.h"
#include "entry_table.h"
#include "entry_tms320f.h"
#include "text_tms320f.h"

using namespace libasm::text::tms320f;

namespace libasm {
namespace tms320f {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, LF_XX, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define X3(_opc, _cf, _name, _opr1, _opr2, _opr3, _lf) \
    {_opc, Entry::Flags::create(_cf, _lf, _opr1, _opr2, _opr3), _name}
#define X2(_opc, _cf, _name, _opr1, _opr2, _lf) X3(_opc, _cf, _name, _opr1, _opr2, M_NONE, _lf)
#define X1(_opc, _cf, _name, _opr1, _lf) X2(_opc, _cf, _name, _opr1, M_NONE, _lf)
#define X0(_opc, _cf, _name, _lf) X1(_opc, _cf, _name, M_NONE, _lf)

// clang-format off
constexpr Entry TABLE_TMS320C3X[] PROGMEM = {
    E2(0x0000, CF_GG, TEXT_ABSF,  M_FGEN, M_FREG),
    E2(0x0080, CF_GG, TEXT_ABSI,  M_IGEN, M_IREG),
    E2(0x0100, CF_GG, TEXT_ADDC,  M_IGEN, M_IREG),
    E2(0x0180, CF_GG, TEXT_ADDF,  M_FGEN, M_FREG),
    E2(0x0200, CF_GG, TEXT_ADDI,  M_IGEN, M_IREG),
    E2(0x0280, CF_GG, TEXT_AND,   M_UGEN, M_IREG),
    E2(0x0300, CF_GG, TEXT_ANDN,  M_UGEN, M_IREG),
    E2(0x0380, CF_GG, TEXT_ASH,   M_SCNT, M_IGEN),
    E2(0x0400, CF_GG, TEXT_CMPF,  M_IGEN, M_FREG),
    E2(0x0480, CF_GG, TEXT_CMPI,  M_IGEN, M_IREG),
    E2(0x0500, CF_GG, TEXT_FIX,   M_FGEN, M_IREG),
    E2(0x0580, CF_GG, TEXT_FLOAT, M_IGEN, M_FREG),
    X0(0x0600, CF_00, TEXT_IDLE,  LF_00),
    X0(0x0600, CF_00, TEXT_IDLE2, LF_01),
    E2(0x0680, CF_GG, TEXT_LDE,   M_FGU8, M_FREG),
    E2(0x0700, CF_GG, TEXT_LDF,   M_FGEN, M_FREG),
    E2(0x0780, CF_GG, TEXT_LDFI,  M_MEM,  M_FREG),
    E2(0x0800, CF_GG, TEXT_LDI,   M_IGEN, M_IREG),
    X2(0x0870, CF_GG, TEXT_LDP,   M_MSBA, R_DP,   LF_FF),
    E2(0x0880, CF_GG, TEXT_LDII,  M_MEM,  M_IREG),
    E2(0x0900, CF_GG, TEXT_LDM,   M_FGEN, M_FREG),
    E2(0x0980, CF_GG, TEXT_LSH,   M_SCNT, M_IGEN),
    E2(0x0A00, CF_GG, TEXT_MPYF,  M_FGEN, M_FREG),
    E2(0x0A80, CF_GG, TEXT_MPYI,  M_IGEN, M_IREG),
    E2(0x0B00, CF_GG, TEXT_NEGB,  M_IGEN, M_IREG),
    E2(0x0B80, CF_GG, TEXT_NEGF,  M_FGEN, M_FREG),
    E2(0x0C00, CF_GG, TEXT_NEGI,  M_IGEN, M_IREG),
    E1(0x0C80, CF_0G, TEXT_NOP,   M_MREG),
    X0(0x0C80, CF_00, TEXT_NOP,   LF_00),
    E2(0x0D00, CF_GG, TEXT_NORM,  M_IGEN, M_FREG),
    E2(0x0D80, CF_GG, TEXT_NOT,   M_IGEN, M_IREG),
    X1(0x0E20, CF_G0, TEXT_POP,   M_IREG, LF_00),
    X1(0x0EA0, CF_G0, TEXT_POPF,  M_FREG, LF_00),
    X1(0x0F20, CF_G0, TEXT_PUSH,  M_IREG, LF_00),
    X1(0x0FA0, CF_G0, TEXT_PUSHF, M_FREG, LF_00),
    E2(0x1000, CF_GG, TEXT_OR,    M_UGEN, M_IREG),
    X0(0x1080, CF_00, TEXT_MAXSPEED, LF_00),
    X0(0x1080, CF_00, TEXT_LOPOWER,  LF_01),
    E2(0x1100, CF_GG, TEXT_RND,   M_FGEN, M_FREG),
    X1(0x11E0, CF_G0, TEXT_ROL,   M_IREG, LF_00),
    X1(0x1260, CF_G0, TEXT_ROLC,  M_IREG, LF_00),
    X1(0x12E0, CF_G0, TEXT_ROR,   M_IREG, LF_00),
    X1(0x1360, CF_G0, TEXT_RORC,  M_IREG, LF_00),
    E1(0x139B, CF_0G, TEXT_RPTS,  M_IREG),
    E2(0x1400, CF_GG, TEXT_STF,   M_FREG, M_MEM),
    E2(0x1480, CF_GG, TEXT_STFI,  M_FREG, M_MEM),
    E2(0x1500, CF_GG, TEXT_STI,   M_IREG, M_MEM),
    E2(0x1580, CF_GG, TEXT_STII,  M_IREG, M_MEM),
    X0(0x1600, CF_00, TEXT_SIGI,  LF_00),
    E2(0x1680, CF_GG, TEXT_SUBB,  M_IGEN, M_IREG),
    E2(0x1700, CF_GG, TEXT_SUBC,  M_IGEN, M_IREG),
    E2(0x1780, CF_GG, TEXT_SUBF,  M_FGEN, M_FREG),
    E2(0x1800, CF_GG, TEXT_SUBI,  M_IGEN, M_IREG),
    E2(0x1880, CF_GG, TEXT_SUBRB, M_IGEN, M_IREG),
    E2(0x1900, CF_GG, TEXT_SUBRF, M_FGEN, M_FREG),
    E2(0x1980, CF_GG, TEXT_SUBRI, M_IGEN, M_IREG),
    E2(0x1A00, CF_GG, TEXT_TSTB,  M_IGEN, M_IREG),
    E2(0x1A80, CF_GG, TEXT_XOR,   M_IGEN, M_IREG),
    E1(0x1B00, CF_G0, TEXT_IACK,  M_MEM),
    E3(0x2000, CF_TT, TEXT_ADDC3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2080, CF_TT, TEXT_ADDF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2100, CF_TT, TEXT_ADDI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2180, CF_TT, TEXT_AND3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2200, CF_TT, TEXT_ANDN3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2280, CF_TT, TEXT_ASH3,  M_CNT8, M_IDAT, M_IREG),
    E2(0x2300, CF_T0, TEXT_CMPF3, M_FDAT, M_FDAT),
    E2(0x2380, CF_T0, TEXT_CMPI3, M_IDAT, M_IDAT),
    E3(0x2400, CF_TT, TEXT_LSH3,  M_CNT8, M_IDAT, M_IREG),
    E3(0x2480, CF_TT, TEXT_MPYF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2500, CF_TT, TEXT_MPYI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2580, CF_TT, TEXT_OR3,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBB3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2700, CF_TT, TEXT_SUBI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2780, CF_TT, TEXT_TSTB3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2800, CF_TT, TEXT_XOR3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2000, CF_TT, TEXT_ADDC, M_IDAT, M_IDAT, M_IREG),
    E3(0x2080, CF_TT, TEXT_ADDF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2100, CF_TT, TEXT_ADDI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2180, CF_TT, TEXT_AND,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2200, CF_TT, TEXT_ANDN, M_IDAT, M_IDAT, M_IREG),
    E3(0x2280, CF_TT, TEXT_ASH,  M_CNT8, M_IDAT, M_IREG),
    E2(0x2300, CF_T0, TEXT_CMPF, M_FDAT, M_FDAT),
    E2(0x2380, CF_T0, TEXT_CMPI, M_IDAT, M_IDAT),
    E3(0x2400, CF_TT, TEXT_LSH,  M_CNT8, M_IDAT, M_IREG),
    E3(0x2480, CF_TT, TEXT_MPYF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2500, CF_TT, TEXT_MPYI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2580, CF_TT, TEXT_OR,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBB, M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2700, CF_TT, TEXT_SUBI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2780, CF_TT, TEXT_TSTB, M_IDAT, M_IDAT, M_IREG),
    E3(0x2800, CF_TT, TEXT_XOR,  M_IDAT, M_IDAT, M_IREG),
    E1(0x6000, CF_BR, TEXT_BR,    M_AD24),
    E1(0x6100, CF_BR, TEXT_BRD,   M_AD24),
    E1(0x6200, CF_BR, TEXT_CALL,  M_AD24),
    E1(0x6400, CF_BR, TEXT_RPTB,  M_AD24),
    X0(0x6600, CF_00, TEXT_SWI,   LF_00),
};
    
constexpr uint8_t INDEX_TMS320C3X[] PROGMEM = {
      0,  // TEXT_ABSF
      1,  // TEXT_ABSI
      2,  // TEXT_ADDC
     76,  // TEXT_ADDC
     59,  // TEXT_ADDC3
      3,  // TEXT_ADDF
     77,  // TEXT_ADDF
     60,  // TEXT_ADDF3
      4,  // TEXT_ADDI
     78,  // TEXT_ADDI
     61,  // TEXT_ADDI3
      5,  // TEXT_AND
     79,  // TEXT_AND
     62,  // TEXT_AND3
      6,  // TEXT_ANDN
     80,  // TEXT_ANDN
     63,  // TEXT_ANDN3
      7,  // TEXT_ASH
     81,  // TEXT_ASH
     64,  // TEXT_ASH3
     93,  // TEXT_BR
     94,  // TEXT_BRD
     95,  // TEXT_CALL
      8,  // TEXT_CMPF
     82,  // TEXT_CMPF
     65,  // TEXT_CMPF3
      9,  // TEXT_CMPI
     83,  // TEXT_CMPI
     66,  // TEXT_CMPI3
     10,  // TEXT_FIX
     11,  // TEXT_FLOAT
     58,  // TEXT_IACK
     12,  // TEXT_IDLE
     13,  // TEXT_IDLE2
     14,  // TEXT_LDE
     15,  // TEXT_LDF
     16,  // TEXT_LDFI
     17,  // TEXT_LDI
     19,  // TEXT_LDII
     20,  // TEXT_LDM
     18,  // TEXT_LDP
     37,  // TEXT_LOPOWER
     21,  // TEXT_LSH
     84,  // TEXT_LSH
     67,  // TEXT_LSH3
     36,  // TEXT_MAXSPEED
     22,  // TEXT_MPYF
     85,  // TEXT_MPYF
     68,  // TEXT_MPYF3
     23,  // TEXT_MPYI
     86,  // TEXT_MPYI
     69,  // TEXT_MPYI3
     24,  // TEXT_NEGB
     25,  // TEXT_NEGF
     26,  // TEXT_NEGI
     27,  // TEXT_NOP
     28,  // TEXT_NOP
     29,  // TEXT_NORM
     30,  // TEXT_NOT
     35,  // TEXT_OR
     87,  // TEXT_OR
     70,  // TEXT_OR3
     31,  // TEXT_POP
     32,  // TEXT_POPF
     33,  // TEXT_PUSH
     34,  // TEXT_PUSHF
     38,  // TEXT_RND
     39,  // TEXT_ROL
     40,  // TEXT_ROLC
     41,  // TEXT_ROR
     42,  // TEXT_RORC
     96,  // TEXT_RPTB
     43,  // TEXT_RPTS
     48,  // TEXT_SIGI
     44,  // TEXT_STF
     45,  // TEXT_STFI
     46,  // TEXT_STI
     47,  // TEXT_STII
     49,  // TEXT_SUBB
     88,  // TEXT_SUBB
     71,  // TEXT_SUBB3
     50,  // TEXT_SUBC
     51,  // TEXT_SUBF
     89,  // TEXT_SUBF
     72,  // TEXT_SUBF3
     52,  // TEXT_SUBI
     90,  // TEXT_SUBI
     73,  // TEXT_SUBI3
     53,  // TEXT_SUBRB
     54,  // TEXT_SUBRF
     55,  // TEXT_SUBRI
     97,  // TEXT_SWI
     56,  // TEXT_TSTB
     91,  // TEXT_TSTB
     74,  // TEXT_TSTB3
     57,  // TEXT_XOR
     92,  // TEXT_XOR
     75,  // TEXT_XOR3
};

constexpr Entry TABLE_LDCOND[] PROGMEM =  {
    E2(0x4000, CF_GG, TEXT_LDFU,    M_FGEN, M_FREG),
    E2(0x4080, CF_GG, TEXT_LDFLO,   M_FGEN, M_FREG),
    E2(0x4080, CF_GG, TEXT_LDFC,    M_FGEN, M_FREG),
    E2(0x4100, CF_GG, TEXT_LDFLS,   M_FGEN, M_FREG),
    E2(0x4180, CF_GG, TEXT_LDFHI,   M_FGEN, M_FREG),
    E2(0x4200, CF_GG, TEXT_LDFHS,   M_FGEN, M_FREG),
    E2(0x4200, CF_GG, TEXT_LDFNC,   M_FGEN, M_FREG),
    E2(0x4280, CF_GG, TEXT_LDFEQ,   M_FGEN, M_FREG),
    E2(0x4280, CF_GG, TEXT_LDFZ,    M_FGEN, M_FREG),
    E2(0x4300, CF_GG, TEXT_LDFNE,   M_FGEN, M_FREG),
    E2(0x4300, CF_GG, TEXT_LDFNZ,   M_FGEN, M_FREG),
    E2(0x4380, CF_GG, TEXT_LDFLT,   M_FGEN, M_FREG),
    E2(0x4380, CF_GG, TEXT_LDFN,    M_FGEN, M_FREG),
    E2(0x4400, CF_GG, TEXT_LDFLE,   M_FGEN, M_FREG),
    E2(0x4480, CF_GG, TEXT_LDFGT,   M_FGEN, M_FREG),
    E2(0x4480, CF_GG, TEXT_LDFP,    M_FGEN, M_FREG),
    E2(0x4500, CF_GG, TEXT_LDFGE,   M_FGEN, M_FREG),
    E2(0x4500, CF_GG, TEXT_LDFNN,   M_FGEN, M_FREG),
    E2(0x4600, CF_GG, TEXT_LDFNV,   M_FGEN, M_FREG),
    E2(0x4680, CF_GG, TEXT_LDFV,    M_FGEN, M_FREG),
    E2(0x4700, CF_GG, TEXT_LDFNUF,  M_FGEN, M_FREG),
    E2(0x4780, CF_GG, TEXT_LDFUF,   M_FGEN, M_FREG),
    E2(0x4800, CF_GG, TEXT_LDFNLV,  M_FGEN, M_FREG),
    E2(0x4880, CF_GG, TEXT_LDFLV,   M_FGEN, M_FREG),
    E2(0x4900, CF_GG, TEXT_LDFNLUF, M_FGEN, M_FREG),
    E2(0x4980, CF_GG, TEXT_LDFLUF,  M_FGEN, M_FREG),
    E2(0x4A00, CF_GG, TEXT_LDFZUF,  M_FGEN, M_FREG),
    E2(0x5000, CF_GG, TEXT_LDIU,    M_IGEN, M_IREG),
    E2(0x5080, CF_GG, TEXT_LDILO,   M_IGEN, M_IREG),
    E2(0x5080, CF_GG, TEXT_LDIC,    M_IGEN, M_IREG),
    E2(0x5100, CF_GG, TEXT_LDILS,   M_IGEN, M_IREG),
    E2(0x5180, CF_GG, TEXT_LDIHI,   M_IGEN, M_IREG),
    E2(0x5200, CF_GG, TEXT_LDIHS,   M_IGEN, M_IREG),
    E2(0x5200, CF_GG, TEXT_LDINC,   M_IGEN, M_IREG),
    E2(0x5280, CF_GG, TEXT_LDIEQ,   M_IGEN, M_IREG),
    E2(0x5280, CF_GG, TEXT_LDIZ,    M_IGEN, M_IREG),
    E2(0x5300, CF_GG, TEXT_LDINE,   M_IGEN, M_IREG),
    E2(0x5300, CF_GG, TEXT_LDINZ,   M_IGEN, M_IREG),
    E2(0x5380, CF_GG, TEXT_LDILT,   M_IGEN, M_IREG),
    E2(0x5380, CF_GG, TEXT_LDIN,    M_IGEN, M_IREG),
    E2(0x5400, CF_GG, TEXT_LDILE,   M_IGEN, M_IREG),
    E2(0x5480, CF_GG, TEXT_LDIGT,   M_IGEN, M_IREG),
    E2(0x5480, CF_GG, TEXT_LDIP,    M_IGEN, M_IREG),
    E2(0x5500, CF_GG, TEXT_LDIGE,   M_IGEN, M_IREG),
    E2(0x5500, CF_GG, TEXT_LDINN,   M_IGEN, M_IREG),
    E2(0x5600, CF_GG, TEXT_LDINV,   M_IGEN, M_IREG),
    E2(0x5680, CF_GG, TEXT_LDIV,    M_IGEN, M_IREG),
    E2(0x5700, CF_GG, TEXT_LDINUF,  M_IGEN, M_IREG),
    E2(0x5780, CF_GG, TEXT_LDIUF,   M_IGEN, M_IREG),
    E2(0x5800, CF_GG, TEXT_LDINLV,  M_IGEN, M_IREG),
    E2(0x5880, CF_GG, TEXT_LDILV,   M_IGEN, M_IREG),
    E2(0x5900, CF_GG, TEXT_LDINLUF, M_IGEN, M_IREG),
    E2(0x5980, CF_GG, TEXT_LDILUF,  M_IGEN, M_IREG),
    E2(0x5A00, CF_GG, TEXT_LDIZUF,  M_IGEN, M_IREG),
};

constexpr uint8_t INDEX_LDCOND[] PROGMEM = {
      2,  // TEXT_LDFC
      7,  // TEXT_LDFEQ
     16,  // TEXT_LDFGE
     14,  // TEXT_LDFGT
      4,  // TEXT_LDFHI
      5,  // TEXT_LDFHS
     13,  // TEXT_LDFLE
      1,  // TEXT_LDFLO
      3,  // TEXT_LDFLS
     11,  // TEXT_LDFLT
     25,  // TEXT_LDFLUF
     23,  // TEXT_LDFLV
     12,  // TEXT_LDFN
      6,  // TEXT_LDFNC
      9,  // TEXT_LDFNE
     24,  // TEXT_LDFNLUF
     22,  // TEXT_LDFNLV
     17,  // TEXT_LDFNN
     20,  // TEXT_LDFNUF
     18,  // TEXT_LDFNV
     10,  // TEXT_LDFNZ
     15,  // TEXT_LDFP
      0,  // TEXT_LDFU
     21,  // TEXT_LDFUF
     19,  // TEXT_LDFV
      8,  // TEXT_LDFZ
     26,  // TEXT_LDFZUF
     29,  // TEXT_LDIC
     34,  // TEXT_LDIEQ
     43,  // TEXT_LDIGE
     41,  // TEXT_LDIGT
     31,  // TEXT_LDIHI
     32,  // TEXT_LDIHS
     40,  // TEXT_LDILE
     28,  // TEXT_LDILO
     30,  // TEXT_LDILS
     38,  // TEXT_LDILT
     52,  // TEXT_LDILUF
     50,  // TEXT_LDILV
     39,  // TEXT_LDIN
     33,  // TEXT_LDINC
     36,  // TEXT_LDINE
     51,  // TEXT_LDINLUF
     49,  // TEXT_LDINLV
     44,  // TEXT_LDINN
     47,  // TEXT_LDINUF
     45,  // TEXT_LDINV
     37,  // TEXT_LDINZ
     42,  // TEXT_LDIP
     27,  // TEXT_LDIU
     48,  // TEXT_LDIUF
     46,  // TEXT_LDIV
     35,  // TEXT_LDIZ
     53,  // TEXT_LDIZUF
};

constexpr Entry TABLE_BRCOND[] PROGMEM = {
    E1(0x6800, CF_00, TEXT_BU,    M_IREL),
    E1(0x6801, CF_00, TEXT_BLO,   M_IREL),
    E1(0x6801, CF_00, TEXT_BC,    M_IREL),
    E1(0x6802, CF_00, TEXT_BLS,   M_IREL),
    E1(0x6803, CF_00, TEXT_BHI,   M_IREL),
    E1(0x6804, CF_00, TEXT_BHS,   M_IREL),
    E1(0x6804, CF_00, TEXT_BNC,   M_IREL),
    E1(0x6805, CF_00, TEXT_BEQ,   M_IREL),
    E1(0x6805, CF_00, TEXT_BZ,    M_IREL),
    E1(0x6806, CF_00, TEXT_BNE,   M_IREL),
    E1(0x6806, CF_00, TEXT_BNZ,   M_IREL),
    E1(0x6807, CF_00, TEXT_BLT,   M_IREL),
    E1(0x6807, CF_00, TEXT_BN,    M_IREL),
    E1(0x6808, CF_00, TEXT_BLE,   M_IREL),
    E1(0x6809, CF_00, TEXT_BGT,   M_IREL),
    E1(0x6809, CF_00, TEXT_BP,    M_IREL),
    E1(0x680A, CF_00, TEXT_BGE,   M_IREL),
    E1(0x680A, CF_00, TEXT_BNN,   M_IREL),
    E1(0x680C, CF_00, TEXT_BNV,   M_IREL),
    E1(0x680D, CF_00, TEXT_BV,    M_IREL),
    E1(0x680E, CF_00, TEXT_BNUF,  M_IREL),
    E1(0x680F, CF_00, TEXT_BUF,   M_IREL),
    E1(0x6810, CF_00, TEXT_BNLV,  M_IREL),
    E1(0x6811, CF_00, TEXT_BLV,   M_IREL),
    E1(0x6812, CF_00, TEXT_BNLUF, M_IREL),
    E1(0x6813, CF_00, TEXT_BLUF,  M_IREL),
    E1(0x6814, CF_00, TEXT_BZUF,  M_IREL),
    E1(0x6820, CF_00, TEXT_BUD,    M_IREL),
    E1(0x6821, CF_00, TEXT_BLOD,   M_IREL),
    E1(0x6821, CF_00, TEXT_BCD,    M_IREL),
    E1(0x6822, CF_00, TEXT_BLSD,   M_IREL),
    E1(0x6823, CF_00, TEXT_BHID,   M_IREL),
    E1(0x6824, CF_00, TEXT_BHSD,   M_IREL),
    E1(0x6824, CF_00, TEXT_BNCD,   M_IREL),
    E1(0x6825, CF_00, TEXT_BEQD,   M_IREL),
    E1(0x6825, CF_00, TEXT_BZD,    M_IREL),
    E1(0x6826, CF_00, TEXT_BNED,   M_IREL),
    E1(0x6826, CF_00, TEXT_BNZD,   M_IREL),
    E1(0x6827, CF_00, TEXT_BLTD,   M_IREL),
    E1(0x6827, CF_00, TEXT_BND,    M_IREL),
    E1(0x6828, CF_00, TEXT_BLED,   M_IREL),
    E1(0x6829, CF_00, TEXT_BGTD,   M_IREL),
    E1(0x6829, CF_00, TEXT_BPD,    M_IREL),
    E1(0x682A, CF_00, TEXT_BGED,   M_IREL),
    E1(0x682A, CF_00, TEXT_BNND,   M_IREL),
    E1(0x682C, CF_00, TEXT_BNVD,   M_IREL),
    E1(0x682D, CF_00, TEXT_BVD,    M_IREL),
    E1(0x682E, CF_00, TEXT_BNUFD,  M_IREL),
    E1(0x682F, CF_00, TEXT_BUFD,   M_IREL),
    E1(0x6830, CF_00, TEXT_BNLVD,  M_IREL),
    E1(0x6831, CF_00, TEXT_BLVD,   M_IREL),
    E1(0x6832, CF_00, TEXT_BNLUFD, M_IREL),
    E1(0x6833, CF_00, TEXT_BLUFD,  M_IREL),
    E1(0x6834, CF_00, TEXT_BZUFD,  M_IREL),
    E2(0x6C00, CF_DB, TEXT_DBU,    R_ARN, M_IREL),
    E2(0x6C01, CF_DB, TEXT_DBLO,   R_ARN, M_IREL),
    E2(0x6C01, CF_DB, TEXT_DBC,    R_ARN, M_IREL),
    E2(0x6C02, CF_DB, TEXT_DBLS,   R_ARN, M_IREL),
    E2(0x6C03, CF_DB, TEXT_DBHI,   R_ARN, M_IREL),
    E2(0x6C04, CF_DB, TEXT_DBHS,   R_ARN, M_IREL),
    E2(0x6C04, CF_DB, TEXT_DBNC,   R_ARN, M_IREL),
    E2(0x6C05, CF_DB, TEXT_DBEQ,   R_ARN, M_IREL),
    E2(0x6C05, CF_DB, TEXT_DBZ,    R_ARN, M_IREL),
    E2(0x6C06, CF_DB, TEXT_DBNE,   R_ARN, M_IREL),
    E2(0x6C06, CF_DB, TEXT_DBNZ,   R_ARN, M_IREL),
    E2(0x6C07, CF_DB, TEXT_DBLT,   R_ARN, M_IREL),
    E2(0x6C07, CF_DB, TEXT_DBN,    R_ARN, M_IREL),
    E2(0x6C08, CF_DB, TEXT_DBLE,   R_ARN, M_IREL),
    E2(0x6C09, CF_DB, TEXT_DBGT,   R_ARN, M_IREL),
    E2(0x6C09, CF_DB, TEXT_DBP,    R_ARN, M_IREL),
    E2(0x6C0A, CF_DB, TEXT_DBGE,   R_ARN, M_IREL),
    E2(0x6C0A, CF_DB, TEXT_DBNN,   R_ARN, M_IREL),
    E2(0x6C0C, CF_DB, TEXT_DBNV,   R_ARN, M_IREL),
    E2(0x6C0D, CF_DB, TEXT_DBV,    R_ARN, M_IREL),
    E2(0x6C0E, CF_DB, TEXT_DBNUF,  R_ARN, M_IREL),
    E2(0x6C0F, CF_DB, TEXT_DBUF,   R_ARN, M_IREL),
    E2(0x6C10, CF_DB, TEXT_DBNLV,  R_ARN, M_IREL),
    E2(0x6C11, CF_DB, TEXT_DBLV,   R_ARN, M_IREL),
    E2(0x6C12, CF_DB, TEXT_DBNLUF, R_ARN, M_IREL),
    E2(0x6C13, CF_DB, TEXT_DBLUF,  R_ARN, M_IREL),
    E2(0x6C14, CF_DB, TEXT_DBZUF,  R_ARN, M_IREL),
    E2(0x6C20, CF_DB, TEXT_DBUD,    R_ARN, M_IREL),
    E2(0x6C21, CF_DB, TEXT_DBLOD,   R_ARN, M_IREL),
    E2(0x6C21, CF_DB, TEXT_DBCD,    R_ARN, M_IREL),
    E2(0x6C22, CF_DB, TEXT_DBLSD,   R_ARN, M_IREL),
    E2(0x6C23, CF_DB, TEXT_DBHID,   R_ARN, M_IREL),
    E2(0x6C24, CF_DB, TEXT_DBHSD,   R_ARN, M_IREL),
    E2(0x6C24, CF_DB, TEXT_DBNCD,   R_ARN, M_IREL),
    E2(0x6C25, CF_DB, TEXT_DBEQD,   R_ARN, M_IREL),
    E2(0x6C25, CF_DB, TEXT_DBZD,    R_ARN, M_IREL),
    E2(0x6C26, CF_DB, TEXT_DBNED,   R_ARN, M_IREL),
    E2(0x6C26, CF_DB, TEXT_DBNZD,   R_ARN, M_IREL),
    E2(0x6C27, CF_DB, TEXT_DBLTD,   R_ARN, M_IREL),
    E2(0x6C27, CF_DB, TEXT_DBND,    R_ARN, M_IREL),
    E2(0x6C28, CF_DB, TEXT_DBLED,   R_ARN, M_IREL),
    E2(0x6C29, CF_DB, TEXT_DBGTD,   R_ARN, M_IREL),
    E2(0x6C29, CF_DB, TEXT_DBPD,    R_ARN, M_IREL),
    E2(0x6C2A, CF_DB, TEXT_DBGED,   R_ARN, M_IREL),
    E2(0x6C2A, CF_DB, TEXT_DBNND,   R_ARN, M_IREL),
    E2(0x6C2C, CF_DB, TEXT_DBNVD,   R_ARN, M_IREL),
    E2(0x6C2D, CF_DB, TEXT_DBVD,    R_ARN, M_IREL),
    E2(0x6C2E, CF_DB, TEXT_DBNUFD,  R_ARN, M_IREL),
    E2(0x6C2F, CF_DB, TEXT_DBUFD,   R_ARN, M_IREL),
    E2(0x6C30, CF_DB, TEXT_DBNLVD,  R_ARN, M_IREL),
    E2(0x6C31, CF_DB, TEXT_DBLVD,   R_ARN, M_IREL),
    E2(0x6C32, CF_DB, TEXT_DBNLUFD, R_ARN, M_IREL),
    E2(0x6C33, CF_DB, TEXT_DBLUFD,  R_ARN, M_IREL),
    E2(0x6C34, CF_DB, TEXT_DBZUFD,  R_ARN, M_IREL),
    E1(0x7000, CF_00, TEXT_CALLU,    M_IREL),
    E1(0x7001, CF_00, TEXT_CALLLO,   M_IREL),
    E1(0x7001, CF_00, TEXT_CALLC,    M_IREL),
    E1(0x7002, CF_00, TEXT_CALLLS,   M_IREL),
    E1(0x7003, CF_00, TEXT_CALLHI,   M_IREL),
    E1(0x7004, CF_00, TEXT_CALLHS,   M_IREL),
    E1(0x7004, CF_00, TEXT_CALLNC,   M_IREL),
    E1(0x7005, CF_00, TEXT_CALLEQ,   M_IREL),
    E1(0x7005, CF_00, TEXT_CALLZ,    M_IREL),
    E1(0x7006, CF_00, TEXT_CALLNE,   M_IREL),
    E1(0x7006, CF_00, TEXT_CALLNZ,   M_IREL),
    E1(0x7007, CF_00, TEXT_CALLLT,   M_IREL),
    E1(0x7007, CF_00, TEXT_CALLN,    M_IREL),
    E1(0x7008, CF_00, TEXT_CALLLE,   M_IREL),
    E1(0x7009, CF_00, TEXT_CALLGT,   M_IREL),
    E1(0x7009, CF_00, TEXT_CALLP,    M_IREL),
    E1(0x700A, CF_00, TEXT_CALLGE,   M_IREL),
    E1(0x700A, CF_00, TEXT_CALLNN,   M_IREL),
    E1(0x700C, CF_00, TEXT_CALLNV,   M_IREL),
    E1(0x700D, CF_00, TEXT_CALLV,    M_IREL),
    E1(0x700E, CF_00, TEXT_CALLNUF,  M_IREL),
    E1(0x700F, CF_00, TEXT_CALLUF,   M_IREL),
    E1(0x7010, CF_00, TEXT_CALLNLV,  M_IREL),
    E1(0x7011, CF_00, TEXT_CALLLV,   M_IREL),
    E1(0x7012, CF_00, TEXT_CALLNLUF, M_IREL),
    E1(0x7013, CF_00, TEXT_CALLLUF,  M_IREL),
    E1(0x7014, CF_00, TEXT_CALLZUF,  M_IREL),
    X1(0x7400, CF_00, TEXT_TRAPU,    M_TVEC, LF_20),
    X1(0x7401, CF_00, TEXT_TRAPLO,   M_TVEC, LF_20),
    X1(0x7401, CF_00, TEXT_TRAPC,    M_TVEC, LF_20),
    X1(0x7402, CF_00, TEXT_TRAPLS,   M_TVEC, LF_20),
    X1(0x7403, CF_00, TEXT_TRAPHI,   M_TVEC, LF_20),
    X1(0x7404, CF_00, TEXT_TRAPHS,   M_TVEC, LF_20),
    X1(0x7404, CF_00, TEXT_TRAPNC,   M_TVEC, LF_20),
    X1(0x7405, CF_00, TEXT_TRAPEQ,   M_TVEC, LF_20),
    X1(0x7405, CF_00, TEXT_TRAPZ,    M_TVEC, LF_20),
    X1(0x7406, CF_00, TEXT_TRAPNE,   M_TVEC, LF_20),
    X1(0x7406, CF_00, TEXT_TRAPNZ,   M_TVEC, LF_20),
    X1(0x7407, CF_00, TEXT_TRAPLT,   M_TVEC, LF_20),
    X1(0x7407, CF_00, TEXT_TRAPN,    M_TVEC, LF_20),
    X1(0x7408, CF_00, TEXT_TRAPLE,   M_TVEC, LF_20),
    X1(0x7409, CF_00, TEXT_TRAPGT,   M_TVEC, LF_20),
    X1(0x7409, CF_00, TEXT_TRAPP,    M_TVEC, LF_20),
    X1(0x740A, CF_00, TEXT_TRAPGE,   M_TVEC, LF_20),
    X1(0x740A, CF_00, TEXT_TRAPNN,   M_TVEC, LF_20),
    X1(0x740C, CF_00, TEXT_TRAPNV,   M_TVEC, LF_20),
    X1(0x740D, CF_00, TEXT_TRAPV,    M_TVEC, LF_20),
    X1(0x740E, CF_00, TEXT_TRAPNUF,  M_TVEC, LF_20),
    X1(0x740F, CF_00, TEXT_TRAPUF,   M_TVEC, LF_20),
    X1(0x7410, CF_00, TEXT_TRAPNLV,  M_TVEC, LF_20),
    X1(0x7411, CF_00, TEXT_TRAPLV,   M_TVEC, LF_20),
    X1(0x7412, CF_00, TEXT_TRAPNLUF, M_TVEC, LF_20),
    X1(0x7413, CF_00, TEXT_TRAPLUF,  M_TVEC, LF_20),
    X1(0x7414, CF_00, TEXT_TRAPZUF,  M_TVEC, LF_20),
    X0(0x7800, CF_00, TEXT_RETIU,    LF_00),
    X0(0x7801, CF_00, TEXT_RETILO,   LF_00),
    X0(0x7801, CF_00, TEXT_RETIC,    LF_00),
    X0(0x7802, CF_00, TEXT_RETILS,   LF_00),
    X0(0x7803, CF_00, TEXT_RETIHI,   LF_00),
    X0(0x7804, CF_00, TEXT_RETIHS,   LF_00),
    X0(0x7804, CF_00, TEXT_RETINC,   LF_00),
    X0(0x7805, CF_00, TEXT_RETIEQ,   LF_00),
    X0(0x7805, CF_00, TEXT_RETIZ,    LF_00),
    X0(0x7806, CF_00, TEXT_RETINE,   LF_00),
    X0(0x7806, CF_00, TEXT_RETINZ,   LF_00),
    X0(0x7807, CF_00, TEXT_RETILT,   LF_00),
    X0(0x7807, CF_00, TEXT_RETIN,    LF_00),
    X0(0x7808, CF_00, TEXT_RETILE,   LF_00),
    X0(0x7809, CF_00, TEXT_RETIGT,   LF_00),
    X0(0x7809, CF_00, TEXT_RETIP,    LF_00),
    X0(0x780A, CF_00, TEXT_RETIGE,   LF_00),
    X0(0x780A, CF_00, TEXT_RETINN,   LF_00),
    X0(0x780C, CF_00, TEXT_RETINV,   LF_00),
    X0(0x780D, CF_00, TEXT_RETIV,    LF_00),
    X0(0x780E, CF_00, TEXT_RETINUF,  LF_00),
    X0(0x780F, CF_00, TEXT_RETIUF,   LF_00),
    X0(0x7810, CF_00, TEXT_RETINLV,  LF_00),
    X0(0x7811, CF_00, TEXT_RETILV,   LF_00),
    X0(0x7812, CF_00, TEXT_RETINLUF, LF_00),
    X0(0x7813, CF_00, TEXT_RETILUF,  LF_00),
    X0(0x7814, CF_00, TEXT_RETIZUF,  LF_00),
    X0(0x7880, CF_00, TEXT_RETSU,    LF_00),
    X0(0x7881, CF_00, TEXT_RETSLO,   LF_00),
    X0(0x7881, CF_00, TEXT_RETSC,    LF_00),
    X0(0x7882, CF_00, TEXT_RETSLS,   LF_00),
    X0(0x7883, CF_00, TEXT_RETSHI,   LF_00),
    X0(0x7884, CF_00, TEXT_RETSHS,   LF_00),
    X0(0x7884, CF_00, TEXT_RETSNC,   LF_00),
    X0(0x7885, CF_00, TEXT_RETSEQ,   LF_00),
    X0(0x7885, CF_00, TEXT_RETSZ,    LF_00),
    X0(0x7886, CF_00, TEXT_RETSNE,   LF_00),
    X0(0x7886, CF_00, TEXT_RETSNZ,   LF_00),
    X0(0x7887, CF_00, TEXT_RETSLT,   LF_00),
    X0(0x7887, CF_00, TEXT_RETSN,    LF_00),
    X0(0x7888, CF_00, TEXT_RETSLE,   LF_00),
    X0(0x7889, CF_00, TEXT_RETSGT,   LF_00),
    X0(0x7889, CF_00, TEXT_RETSP,    LF_00),
    X0(0x788A, CF_00, TEXT_RETSGE,   LF_00),
    X0(0x788A, CF_00, TEXT_RETSNN,   LF_00),
    X0(0x788C, CF_00, TEXT_RETSNV,   LF_00),
    X0(0x788D, CF_00, TEXT_RETSV,    LF_00),
    X0(0x788E, CF_00, TEXT_RETSNUF,  LF_00),
    X0(0x788F, CF_00, TEXT_RETSUF,   LF_00),
    X0(0x7890, CF_00, TEXT_RETSNLV,  LF_00),
    X0(0x7891, CF_00, TEXT_RETSLV,   LF_00),
    X0(0x7892, CF_00, TEXT_RETSNLUF, LF_00),
    X0(0x7893, CF_00, TEXT_RETSLUF,  LF_00),
    X0(0x7894, CF_00, TEXT_RETSZUF,  LF_00),
};

constexpr uint8_t INDEX_BRCOND[] PROGMEM =  {
      2,  // TEXT_BC
     29,  // TEXT_BCD
      7,  // TEXT_BEQ
     34,  // TEXT_BEQD
     16,  // TEXT_BGE
     43,  // TEXT_BGED
     14,  // TEXT_BGT
     41,  // TEXT_BGTD
      4,  // TEXT_BHI
     31,  // TEXT_BHID
      5,  // TEXT_BHS
     32,  // TEXT_BHSD
     13,  // TEXT_BLE
     40,  // TEXT_BLED
      1,  // TEXT_BLO
     28,  // TEXT_BLOD
      3,  // TEXT_BLS
     30,  // TEXT_BLSD
     11,  // TEXT_BLT
     38,  // TEXT_BLTD
     25,  // TEXT_BLUF
     52,  // TEXT_BLUFD
     23,  // TEXT_BLV
     50,  // TEXT_BLVD
     12,  // TEXT_BN
      6,  // TEXT_BNC
     33,  // TEXT_BNCD
     39,  // TEXT_BND
      9,  // TEXT_BNE
     36,  // TEXT_BNED
     24,  // TEXT_BNLUF
     51,  // TEXT_BNLUFD
     22,  // TEXT_BNLV
     49,  // TEXT_BNLVD
     17,  // TEXT_BNN
     44,  // TEXT_BNND
     20,  // TEXT_BNUF
     47,  // TEXT_BNUFD
     18,  // TEXT_BNV
     45,  // TEXT_BNVD
     10,  // TEXT_BNZ
     37,  // TEXT_BNZD
     15,  // TEXT_BP
     42,  // TEXT_BPD
      0,  // TEXT_BU
     27,  // TEXT_BUD
     21,  // TEXT_BUF
     48,  // TEXT_BUFD
     19,  // TEXT_BV
     46,  // TEXT_BVD
      8,  // TEXT_BZ
     35,  // TEXT_BZD
     26,  // TEXT_BZUF
     53,  // TEXT_BZUFD
    110,  // TEXT_CALLC
    115,  // TEXT_CALLEQ
    124,  // TEXT_CALLGE
    122,  // TEXT_CALLGT
    112,  // TEXT_CALLHI
    113,  // TEXT_CALLHS
    121,  // TEXT_CALLLE
    109,  // TEXT_CALLLO
    111,  // TEXT_CALLLS
    119,  // TEXT_CALLLT
    133,  // TEXT_CALLLUF
    131,  // TEXT_CALLLV
    120,  // TEXT_CALLN
    114,  // TEXT_CALLNC
    117,  // TEXT_CALLNE
    132,  // TEXT_CALLNLUF
    130,  // TEXT_CALLNLV
    125,  // TEXT_CALLNN
    128,  // TEXT_CALLNUF
    126,  // TEXT_CALLNV
    118,  // TEXT_CALLNZ
    123,  // TEXT_CALLP
    108,  // TEXT_CALLU
    129,  // TEXT_CALLUF
    127,  // TEXT_CALLV
    116,  // TEXT_CALLZ
    134,  // TEXT_CALLZUF
     56,  // TEXT_DBC
     83,  // TEXT_DBCD
     61,  // TEXT_DBEQ
     88,  // TEXT_DBEQD
     70,  // TEXT_DBGE
     97,  // TEXT_DBGED
     68,  // TEXT_DBGT
     95,  // TEXT_DBGTD
     58,  // TEXT_DBHI
     85,  // TEXT_DBHID
     59,  // TEXT_DBHS
     86,  // TEXT_DBHSD
     67,  // TEXT_DBLE
     94,  // TEXT_DBLED
     55,  // TEXT_DBLO
     82,  // TEXT_DBLOD
     57,  // TEXT_DBLS
     84,  // TEXT_DBLSD
     65,  // TEXT_DBLT
     92,  // TEXT_DBLTD
     79,  // TEXT_DBLUF
    106,  // TEXT_DBLUFD
     77,  // TEXT_DBLV
    104,  // TEXT_DBLVD
     66,  // TEXT_DBN
     60,  // TEXT_DBNC
     87,  // TEXT_DBNCD
     93,  // TEXT_DBND
     63,  // TEXT_DBNE
     90,  // TEXT_DBNED
     78,  // TEXT_DBNLUF
    105,  // TEXT_DBNLUFD
     76,  // TEXT_DBNLV
    103,  // TEXT_DBNLVD
     71,  // TEXT_DBNN
     98,  // TEXT_DBNND
     74,  // TEXT_DBNUF
    101,  // TEXT_DBNUFD
     72,  // TEXT_DBNV
     99,  // TEXT_DBNVD
     64,  // TEXT_DBNZ
     91,  // TEXT_DBNZD
     69,  // TEXT_DBP
     96,  // TEXT_DBPD
     54,  // TEXT_DBU
     81,  // TEXT_DBUD
     75,  // TEXT_DBUF
    102,  // TEXT_DBUFD
     73,  // TEXT_DBV
    100,  // TEXT_DBVD
     62,  // TEXT_DBZ
     89,  // TEXT_DBZD
     80,  // TEXT_DBZUF
    107,  // TEXT_DBZUFD
    164,  // TEXT_RETIC
    169,  // TEXT_RETIEQ
    178,  // TEXT_RETIGE
    176,  // TEXT_RETIGT
    166,  // TEXT_RETIHI
    167,  // TEXT_RETIHS
    175,  // TEXT_RETILE
    163,  // TEXT_RETILO
    165,  // TEXT_RETILS
    173,  // TEXT_RETILT
    187,  // TEXT_RETILUF
    185,  // TEXT_RETILV
    174,  // TEXT_RETIN
    168,  // TEXT_RETINC
    171,  // TEXT_RETINE
    186,  // TEXT_RETINLUF
    184,  // TEXT_RETINLV
    179,  // TEXT_RETINN
    182,  // TEXT_RETINUF
    180,  // TEXT_RETINV
    172,  // TEXT_RETINZ
    177,  // TEXT_RETIP
    162,  // TEXT_RETIU
    183,  // TEXT_RETIUF
    181,  // TEXT_RETIV
    170,  // TEXT_RETIZ
    188,  // TEXT_RETIZUF
    191,  // TEXT_RETSC
    196,  // TEXT_RETSEQ
    205,  // TEXT_RETSGE
    203,  // TEXT_RETSGT
    193,  // TEXT_RETSHI
    194,  // TEXT_RETSHS
    202,  // TEXT_RETSLE
    190,  // TEXT_RETSLO
    192,  // TEXT_RETSLS
    200,  // TEXT_RETSLT
    214,  // TEXT_RETSLUF
    212,  // TEXT_RETSLV
    201,  // TEXT_RETSN
    195,  // TEXT_RETSNC
    198,  // TEXT_RETSNE
    213,  // TEXT_RETSNLUF
    211,  // TEXT_RETSNLV
    206,  // TEXT_RETSNN
    209,  // TEXT_RETSNUF
    207,  // TEXT_RETSNV
    199,  // TEXT_RETSNZ
    204,  // TEXT_RETSP
    189,  // TEXT_RETSU
    210,  // TEXT_RETSUF
    208,  // TEXT_RETSV
    197,  // TEXT_RETSZ
    215,  // TEXT_RETSZUF
    137,  // TEXT_TRAPC
    142,  // TEXT_TRAPEQ
    151,  // TEXT_TRAPGE
    149,  // TEXT_TRAPGT
    139,  // TEXT_TRAPHI
    140,  // TEXT_TRAPHS
    148,  // TEXT_TRAPLE
    136,  // TEXT_TRAPLO
    138,  // TEXT_TRAPLS
    146,  // TEXT_TRAPLT
    160,  // TEXT_TRAPLUF
    158,  // TEXT_TRAPLV
    147,  // TEXT_TRAPN
    141,  // TEXT_TRAPNC
    144,  // TEXT_TRAPNE
    159,  // TEXT_TRAPNLUF
    157,  // TEXT_TRAPNLV
    152,  // TEXT_TRAPNN
    155,  // TEXT_TRAPNUF
    153,  // TEXT_TRAPNV
    145,  // TEXT_TRAPNZ
    150,  // TEXT_TRAPP
    135,  // TEXT_TRAPU
    156,  // TEXT_TRAPUF
    154,  // TEXT_TRAPV
    143,  // TEXT_TRAPZ
    161,  // TEXT_TRAPZUF
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage TMS320C3X_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C3X), ARRAY_RANGE(INDEX_TMS320C3X)},
        {ARRAY_RANGE(TABLE_LDCOND), ARRAY_RANGE(INDEX_LDCOND)},
        {ARRAY_RANGE(TABLE_BRCOND), ARRAY_RANGE(INDEX_BRCOND)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS320C3X, TEXT_CPU_320C3X, ARRAY_RANGE(TMS320C3X_PAGES)},
};

const Cpu *cpu(CpuType) {
    return &CPU_TABLE[0];
}

bool acceptMode(AddrMode opr, AddrMode table) {
    return opr == table;
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2()) &&
           acceptMode(insn.op3.mode, table.mode3());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    opc &= flags.mask();
    return opc == entry->readOpCode();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms320f::listCpu_P() const {
    return TEXT_TMS320F_LIST;
}

const /*PROGMEM*/ char *TableTms320f::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms320f::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_TMS320F_LIST, 3);  // Ignore "TMS" if exist
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableTms320f TABLE;

}  // namespace tms320f
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
