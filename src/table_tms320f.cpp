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

#define A3(_opc, _cf, _lf, _name, _opr1, _opr2, _opr3, _unary) \
    {_opc, Entry::Flags::create(_cf, _lf, _opr1, _opr2, _opr3, _unary), _name}
#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    A3(_opc, _cf, LF_XX, _name, _opr1, _opr2, _opr3, false)
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)
#define U2(_opc, _cf, _name, _opr1, _opr2) A3(_opc, _cf, LF_XX, _name, _opr1, _opr2, M_NONE, true)
#define X3(_opc, _cf, _name, _opr1, _opr2, _opr3, _lf) \
    A3(_opc, _cf, _lf, _name, _opr1, _opr2, _opr3, false)
#define X2(_opc, _cf, _name, _opr1, _opr2, _lf) X3(_opc, _cf, _name, _opr1, _opr2, M_NONE, _lf)
#define X1(_opc, _cf, _name, _opr1, _lf) X2(_opc, _cf, _name, _opr1, M_NONE, _lf)
#define X0(_opc, _cf, _name, _lf) X1(_opc, _cf, _name, M_NONE, _lf)

// clang-format off
constexpr Entry TABLE_TMS320C30[] PROGMEM = {
    U2(0x0000, CF_GG, TEXT_ABSF,  M_FGEN, M_FREG),
    U2(0x0080, CF_GG, TEXT_ABSI,  M_IGEN, M_IREG),
    E2(0x0100, CF_GG, TEXT_ADDC,  M_IGEN, M_IREG),
    E2(0x0180, CF_GG, TEXT_ADDF,  M_FGEN, M_FREG),
    E2(0x0200, CF_GG, TEXT_ADDI,  M_IGEN, M_IREG),
    E2(0x0280, CF_GG, TEXT_AND,   M_UGEN, M_IREG),
    E2(0x0300, CF_GG, TEXT_ANDN,  M_UGEN, M_IREG),
    E2(0x0380, CF_GG, TEXT_ASH,   M_GCNT, M_IREG),
    E2(0x0400, CF_GG, TEXT_CMPF,  M_FGEN, M_FREG),
    E2(0x0480, CF_GG, TEXT_CMPI,  M_IGEN, M_IREG),
    U2(0x0500, CF_GG, TEXT_FIX,   M_FGEN, M_IREG),
    U2(0x0580, CF_GG, TEXT_FLOAT, M_IGEN, M_FREG),
    X0(0x0600, CF_00, TEXT_IDLE,  LF_00),
    E2(0x0680, CF_GG, TEXT_LDE,   M_FGEN, M_FREG),
    E2(0x0700, CF_GG, TEXT_LDF,   M_FGEN, M_FREG),
    E2(0x0780, CF_GG, TEXT_LDFI,  M_MEM,  M_FREG),
    E1(0x0870, CF_00, TEXT_LDP,   M_MSBA),
    E2(0x0870, CF_00, TEXT_LDP,   M_MSBA, R_DP),
    E2(0x0800, CF_GG, TEXT_LDI,   M_IGEN, M_IREG),
    E2(0x0880, CF_GG, TEXT_LDII,  M_MEM,  M_IREG),
    E2(0x0900, CF_GG, TEXT_LDM,   M_FLDM, M_FREG),
    E2(0x0980, CF_GG, TEXT_LSH,   M_GCNT, M_IREG),
    E2(0x0A00, CF_GG, TEXT_MPYF,  M_FGEN, M_FREG),
    E2(0x0A80, CF_GG, TEXT_MPYI,  M_IGEN, M_IREG),
    U2(0x0B00, CF_GG, TEXT_NEGB,  M_IGEN, M_IREG),
    U2(0x0B80, CF_GG, TEXT_NEGF,  M_FGEN, M_FREG),
    U2(0x0C00, CF_GG, TEXT_NEGI,  M_IGEN, M_IREG),
    E1(0x0C80, CF_0G, TEXT_NOP,   M_MREG),
    X0(0x0C80, CF_00, TEXT_NOP,   LF_00),
    U2(0x0D00, CF_GG, TEXT_NORM,  M_FGEN, M_FREG),
    U2(0x0D80, CF_GG, TEXT_NOT,   M_UGEN, M_IREG),
    X1(0x0E20, CF_GX, TEXT_POP,   M_IREG, LF_00),
    X1(0x0EA0, CF_GX, TEXT_POPF,  M_FREG, LF_00),
    X1(0x0F20, CF_GX, TEXT_PUSH,  M_IREG, LF_00),
    X1(0x0FA0, CF_GX, TEXT_PUSHF, M_FREG, LF_00),
    E2(0x1000, CF_GG, TEXT_OR,    M_UGEN, M_IREG),
    U2(0x1100, CF_GG, TEXT_RND,   M_FGEN, M_FREG),
    X1(0x11E0, CF_GX, TEXT_ROL,   M_IREG, LF_01),
    X1(0x1260, CF_GX, TEXT_ROLC,  M_IREG, LF_01),
    X1(0x12E0, CF_GX, TEXT_ROR,   M_IREG, LF_FF),
    X1(0x1360, CF_GX, TEXT_RORC,  M_IREG, LF_FF),
    E1(0x139B, CF_0G, TEXT_RPTS,  M_UGEN),
    E2(0x1400, CF_GG, TEXT_STF,   M_FREG, M_MEM),
    E2(0x1480, CF_GG, TEXT_STFI,  M_FREG, M_MEM),
    E2(0x1500, CF_GG, TEXT_STI,   M_IREG, M_MEM),
    E2(0x1580, CF_GG, TEXT_STII,  M_IREG, M_MEM),
    X0(0x1600, CF_00, TEXT_SIGI,  LF_00),
    E2(0x1680, CF_GG, TEXT_SUBB,  M_IGEN, M_IREG),
    E2(0x1700, CF_GG, TEXT_SUBC,  M_UGEN, M_IREG),
    E2(0x1780, CF_GG, TEXT_SUBF,  M_FGEN, M_FREG),
    E2(0x1800, CF_GG, TEXT_SUBI,  M_IGEN, M_IREG),
    E2(0x1880, CF_GG, TEXT_SUBRB, M_IGEN, M_IREG),
    E2(0x1900, CF_GG, TEXT_SUBRF, M_FGEN, M_FREG),
    E2(0x1980, CF_GG, TEXT_SUBRI, M_IGEN, M_IREG),
    E2(0x1A00, CF_GG, TEXT_TSTB,  M_UGEN, M_IREG),
    E2(0x1A80, CF_GG, TEXT_XOR,   M_UGEN, M_IREG),
    E1(0x1B00, CF_0G, TEXT_IACK,  M_MEM),
    E3(0x2000, CF_TT, TEXT_ADDC3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2000, CF_TT, TEXT_ADDC,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2080, CF_TT, TEXT_ADDF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2080, CF_TT, TEXT_ADDF,  M_FDAT, M_FDAT, M_FREG),
    E3(0x2100, CF_TT, TEXT_ADDI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2100, CF_TT, TEXT_ADDI,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2180, CF_TT, TEXT_AND3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2180, CF_TT, TEXT_AND,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2200, CF_TT, TEXT_ANDN3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2200, CF_TT, TEXT_ANDN,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2280, CF_TT, TEXT_ASH3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2280, CF_TT, TEXT_ASH,   M_IDAT, M_IDAT, M_IREG),
    E2(0x2300, CF_T0, TEXT_CMPF3, M_FDAT, M_FDAT),
    E2(0x2300, CF_T0, TEXT_CMPF,  M_FDAT, M_FDAT),
    E2(0x2380, CF_T0, TEXT_CMPI3, M_IDAT, M_IDAT),
    E2(0x2380, CF_T0, TEXT_CMPI,  M_IDAT, M_IDAT),
    E3(0x2400, CF_TT, TEXT_LSH3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2400, CF_TT, TEXT_LSH,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2480, CF_TT, TEXT_MPYF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2480, CF_TT, TEXT_MPYF,  M_FDAT, M_FDAT, M_FREG),
    E3(0x2500, CF_TT, TEXT_MPYI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2500, CF_TT, TEXT_MPYI,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2580, CF_TT, TEXT_OR3,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2580, CF_TT, TEXT_OR,    M_IDAT, M_IDAT, M_IREG),
    E3(0x2600, CF_TT, TEXT_SUBB3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2600, CF_TT, TEXT_SUBB,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBF3, M_FDAT, M_FDAT, M_FREG),
    E3(0x2680, CF_TT, TEXT_SUBF,  M_FDAT, M_FDAT, M_FREG),
    E3(0x2700, CF_TT, TEXT_SUBI3, M_IDAT, M_IDAT, M_IREG),
    E3(0x2700, CF_TT, TEXT_SUBI,  M_IDAT, M_IDAT, M_IREG),
    E2(0x2780, CF_TT, TEXT_TSTB3, M_IDAT, M_IDAT),
    E2(0x2780, CF_TT, TEXT_TSTB,  M_IDAT, M_IDAT),
    E3(0x2800, CF_TT, TEXT_XOR3,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2800, CF_TT, TEXT_XOR,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2000, CF_TT, TEXT_ADDC, M_IDAT, M_IDAT, M_IREG),
    E3(0x2080, CF_TT, TEXT_ADDF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2100, CF_TT, TEXT_ADDI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2180, CF_TT, TEXT_AND,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2200, CF_TT, TEXT_ANDN, M_IDAT, M_IDAT, M_IREG),
    E3(0x2280, CF_TT, TEXT_ASH,  M_IDAT, M_IDAT, M_IREG),
    E2(0x2300, CF_T0, TEXT_CMPF, M_FDAT, M_FDAT),
    E2(0x2380, CF_T0, TEXT_CMPI, M_IDAT, M_IDAT),
    E3(0x2400, CF_TT, TEXT_LSH,  M_IDAT, M_IDAT, M_IREG),
    E3(0x2480, CF_TT, TEXT_MPYF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2500, CF_TT, TEXT_MPYI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2580, CF_TT, TEXT_OR,   M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBB, M_IDAT, M_IDAT, M_IREG),
    E3(0x2680, CF_TT, TEXT_SUBF, M_FDAT, M_FDAT, M_FREG),
    E3(0x2700, CF_TT, TEXT_SUBI, M_IDAT, M_IDAT, M_IREG),
    E3(0x2780, CF_TT, TEXT_TSTB, M_IDAT, M_IDAT, M_IREG),
    E3(0x2800, CF_TT, TEXT_XOR,  M_IDAT, M_IDAT, M_IREG),
    E1(0x6000, CF_BR, TEXT_BR,   M_AD24),
    E1(0x6100, CF_BR, TEXT_BRD,  M_AD24),
    E1(0x6200, CF_BR, TEXT_CALL, M_AD24),
    E1(0x6400, CF_BR, TEXT_RPTB, M_AD24),
    X0(0x6600, CF_00, TEXT_SWI,  LF_00),
};

constexpr uint8_t INDEX_TMS320C30[] PROGMEM = {
    /* empty */
};

constexpr Entry TABLE_TMS320C31[] PROGMEM = {
    X0(0x0600, CF_00, TEXT_IDLE2, LF_01),
    X0(0x1080, CF_00, TEXT_LOPOWER,  LF_01),
    X0(0x1080, CF_00, TEXT_MAXSPEED, LF_00),
};

constexpr uint8_t INDEX_TMS320C31[] PROGMEM = {
    /* empty */
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
    /* empty */
};

constexpr Entry TABLE_BRCOND[] PROGMEM = {
    E1(0x6800, CF_BB, TEXT_BU,    M_IREL),
    E1(0x6800, CF_BB, TEXT_B,     M_IREL),
    E1(0x6801, CF_BB, TEXT_BLO,   M_IREL),
    E1(0x6801, CF_BB, TEXT_BC,    M_IREL),
    E1(0x6802, CF_BB, TEXT_BLS,   M_IREL),
    E1(0x6803, CF_BB, TEXT_BHI,   M_IREL),
    E1(0x6804, CF_BB, TEXT_BHS,   M_IREL),
    E1(0x6804, CF_BB, TEXT_BNC,   M_IREL),
    E1(0x6805, CF_BB, TEXT_BEQ,   M_IREL),
    E1(0x6805, CF_BB, TEXT_BZ,    M_IREL),
    E1(0x6806, CF_BB, TEXT_BNE,   M_IREL),
    E1(0x6806, CF_BB, TEXT_BNZ,   M_IREL),
    E1(0x6807, CF_BB, TEXT_BLT,   M_IREL),
    E1(0x6807, CF_BB, TEXT_BN,    M_IREL),
    E1(0x6808, CF_BB, TEXT_BLE,   M_IREL),
    E1(0x6809, CF_BB, TEXT_BGT,   M_IREL),
    E1(0x6809, CF_BB, TEXT_BP,    M_IREL),
    E1(0x680A, CF_BB, TEXT_BGE,   M_IREL),
    E1(0x680A, CF_BB, TEXT_BNN,   M_IREL),
    E1(0x680C, CF_BB, TEXT_BNV,   M_IREL),
    E1(0x680D, CF_BB, TEXT_BV,    M_IREL),
    E1(0x680E, CF_BB, TEXT_BNUF,  M_IREL),
    E1(0x680F, CF_BB, TEXT_BUF,   M_IREL),
    E1(0x6810, CF_BB, TEXT_BNLV,  M_IREL),
    E1(0x6811, CF_BB, TEXT_BLV,   M_IREL),
    E1(0x6812, CF_BB, TEXT_BNLUF, M_IREL),
    E1(0x6813, CF_BB, TEXT_BLUF,  M_IREL),
    E1(0x6814, CF_BB, TEXT_BZUF,  M_IREL),
    E1(0x6820, CF_BB, TEXT_BUD,    M_DREL),
    E1(0x6820, CF_BB, TEXT_BD,     M_DREL),
    E1(0x6821, CF_BB, TEXT_BLOD,   M_DREL),
    E1(0x6821, CF_BB, TEXT_BCD,    M_DREL),
    E1(0x6822, CF_BB, TEXT_BLSD,   M_DREL),
    E1(0x6823, CF_BB, TEXT_BHID,   M_DREL),
    E1(0x6824, CF_BB, TEXT_BHSD,   M_DREL),
    E1(0x6824, CF_BB, TEXT_BNCD,   M_DREL),
    E1(0x6825, CF_BB, TEXT_BEQD,   M_DREL),
    E1(0x6825, CF_BB, TEXT_BZD,    M_DREL),
    E1(0x6826, CF_BB, TEXT_BNED,   M_DREL),
    E1(0x6826, CF_BB, TEXT_BNZD,   M_DREL),
    E1(0x6827, CF_BB, TEXT_BLTD,   M_DREL),
    E1(0x6827, CF_BB, TEXT_BND,    M_DREL),
    E1(0x6828, CF_BB, TEXT_BLED,   M_DREL),
    E1(0x6829, CF_BB, TEXT_BGTD,   M_DREL),
    E1(0x6829, CF_BB, TEXT_BPD,    M_DREL),
    E1(0x682A, CF_BB, TEXT_BGED,   M_DREL),
    E1(0x682A, CF_BB, TEXT_BNND,   M_DREL),
    E1(0x682C, CF_BB, TEXT_BNVD,   M_DREL),
    E1(0x682D, CF_BB, TEXT_BVD,    M_DREL),
    E1(0x682E, CF_BB, TEXT_BNUFD,  M_DREL),
    E1(0x682F, CF_BB, TEXT_BUFD,   M_DREL),
    E1(0x6830, CF_BB, TEXT_BNLVD,  M_DREL),
    E1(0x6831, CF_BB, TEXT_BLVD,   M_DREL),
    E1(0x6832, CF_BB, TEXT_BNLUFD, M_DREL),
    E1(0x6833, CF_BB, TEXT_BLUFD,  M_DREL),
    E1(0x6834, CF_BB, TEXT_BZUFD,  M_DREL),
    E2(0x6C00, CF_DB, TEXT_DBU,    R_ARN, M_IREL),
    E2(0x6C00, CF_DB, TEXT_DB,     R_ARN, M_IREL),
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
    E2(0x6C20, CF_DB, TEXT_DBUD,    R_ARN, M_DREL),
    E2(0x6C20, CF_DB, TEXT_DBD,     R_ARN, M_DREL),
    E2(0x6C21, CF_DB, TEXT_DBLOD,   R_ARN, M_DREL),
    E2(0x6C21, CF_DB, TEXT_DBCD,    R_ARN, M_DREL),
    E2(0x6C22, CF_DB, TEXT_DBLSD,   R_ARN, M_DREL),
    E2(0x6C23, CF_DB, TEXT_DBHID,   R_ARN, M_DREL),
    E2(0x6C24, CF_DB, TEXT_DBHSD,   R_ARN, M_DREL),
    E2(0x6C24, CF_DB, TEXT_DBNCD,   R_ARN, M_DREL),
    E2(0x6C25, CF_DB, TEXT_DBEQD,   R_ARN, M_DREL),
    E2(0x6C25, CF_DB, TEXT_DBZD,    R_ARN, M_DREL),
    E2(0x6C26, CF_DB, TEXT_DBNED,   R_ARN, M_DREL),
    E2(0x6C26, CF_DB, TEXT_DBNZD,   R_ARN, M_DREL),
    E2(0x6C27, CF_DB, TEXT_DBLTD,   R_ARN, M_DREL),
    E2(0x6C27, CF_DB, TEXT_DBND,    R_ARN, M_DREL),
    E2(0x6C28, CF_DB, TEXT_DBLED,   R_ARN, M_DREL),
    E2(0x6C29, CF_DB, TEXT_DBGTD,   R_ARN, M_DREL),
    E2(0x6C29, CF_DB, TEXT_DBPD,    R_ARN, M_DREL),
    E2(0x6C2A, CF_DB, TEXT_DBGED,   R_ARN, M_DREL),
    E2(0x6C2A, CF_DB, TEXT_DBNND,   R_ARN, M_DREL),
    E2(0x6C2C, CF_DB, TEXT_DBNVD,   R_ARN, M_DREL),
    E2(0x6C2D, CF_DB, TEXT_DBVD,    R_ARN, M_DREL),
    E2(0x6C2E, CF_DB, TEXT_DBNUFD,  R_ARN, M_DREL),
    E2(0x6C2F, CF_DB, TEXT_DBUFD,   R_ARN, M_DREL),
    E2(0x6C30, CF_DB, TEXT_DBNLVD,  R_ARN, M_DREL),
    E2(0x6C31, CF_DB, TEXT_DBLVD,   R_ARN, M_DREL),
    E2(0x6C32, CF_DB, TEXT_DBNLUFD, R_ARN, M_DREL),
    E2(0x6C33, CF_DB, TEXT_DBLUFD,  R_ARN, M_DREL),
    E2(0x6C34, CF_DB, TEXT_DBZUFD,  R_ARN, M_DREL),
};

constexpr uint8_t INDEX_BRCOND[] PROGMEM =  {
    /* empty */
};

constexpr Entry TABLE_CALLCOND[] PROGMEM = {
    E1(0x7000, CF_BB, TEXT_CALLU,    M_IREL),
    E1(0x7001, CF_BB, TEXT_CALLLO,   M_IREL),
    E1(0x7001, CF_BB, TEXT_CALLC,    M_IREL),
    E1(0x7002, CF_BB, TEXT_CALLLS,   M_IREL),
    E1(0x7003, CF_BB, TEXT_CALLHI,   M_IREL),
    E1(0x7004, CF_BB, TEXT_CALLHS,   M_IREL),
    E1(0x7004, CF_BB, TEXT_CALLNC,   M_IREL),
    E1(0x7005, CF_BB, TEXT_CALLEQ,   M_IREL),
    E1(0x7005, CF_BB, TEXT_CALLZ,    M_IREL),
    E1(0x7006, CF_BB, TEXT_CALLNE,   M_IREL),
    E1(0x7006, CF_BB, TEXT_CALLNZ,   M_IREL),
    E1(0x7007, CF_BB, TEXT_CALLLT,   M_IREL),
    E1(0x7007, CF_BB, TEXT_CALLN,    M_IREL),
    E1(0x7008, CF_BB, TEXT_CALLLE,   M_IREL),
    E1(0x7009, CF_BB, TEXT_CALLGT,   M_IREL),
    E1(0x7009, CF_BB, TEXT_CALLP,    M_IREL),
    E1(0x700A, CF_BB, TEXT_CALLGE,   M_IREL),
    E1(0x700A, CF_BB, TEXT_CALLNN,   M_IREL),
    E1(0x700C, CF_BB, TEXT_CALLNV,   M_IREL),
    E1(0x700D, CF_BB, TEXT_CALLV,    M_IREL),
    E1(0x700E, CF_BB, TEXT_CALLNUF,  M_IREL),
    E1(0x700F, CF_BB, TEXT_CALLUF,   M_IREL),
    E1(0x7010, CF_BB, TEXT_CALLNLV,  M_IREL),
    E1(0x7011, CF_BB, TEXT_CALLLV,   M_IREL),
    E1(0x7012, CF_BB, TEXT_CALLNLUF, M_IREL),
    E1(0x7013, CF_BB, TEXT_CALLLUF,  M_IREL),
    E1(0x7014, CF_BB, TEXT_CALLZUF,  M_IREL),
    E1(0x7400, CF_00, TEXT_TRAPU,    M_TVEC),
    E1(0x7401, CF_00, TEXT_TRAPLO,   M_TVEC),
    E1(0x7401, CF_00, TEXT_TRAPC,    M_TVEC),
    E1(0x7402, CF_00, TEXT_TRAPLS,   M_TVEC),
    E1(0x7403, CF_00, TEXT_TRAPHI,   M_TVEC),
    E1(0x7404, CF_00, TEXT_TRAPHS,   M_TVEC),
    E1(0x7404, CF_00, TEXT_TRAPNC,   M_TVEC),
    E1(0x7405, CF_00, TEXT_TRAPEQ,   M_TVEC),
    E1(0x7405, CF_00, TEXT_TRAPZ,    M_TVEC),
    E1(0x7406, CF_00, TEXT_TRAPNE,   M_TVEC),
    E1(0x7406, CF_00, TEXT_TRAPNZ,   M_TVEC),
    E1(0x7407, CF_00, TEXT_TRAPLT,   M_TVEC),
    E1(0x7407, CF_00, TEXT_TRAPN,    M_TVEC),
    E1(0x7408, CF_00, TEXT_TRAPLE,   M_TVEC),
    E1(0x7409, CF_00, TEXT_TRAPGT,   M_TVEC),
    E1(0x7409, CF_00, TEXT_TRAPP,    M_TVEC),
    E1(0x740A, CF_00, TEXT_TRAPGE,   M_TVEC),
    E1(0x740A, CF_00, TEXT_TRAPNN,   M_TVEC),
    E1(0x740C, CF_00, TEXT_TRAPNV,   M_TVEC),
    E1(0x740D, CF_00, TEXT_TRAPV,    M_TVEC),
    E1(0x740E, CF_00, TEXT_TRAPNUF,  M_TVEC),
    E1(0x740F, CF_00, TEXT_TRAPUF,   M_TVEC),
    E1(0x7410, CF_00, TEXT_TRAPNLV,  M_TVEC),
    E1(0x7411, CF_00, TEXT_TRAPLV,   M_TVEC),
    E1(0x7412, CF_00, TEXT_TRAPNLUF, M_TVEC),
    E1(0x7413, CF_00, TEXT_TRAPLUF,  M_TVEC),
    E1(0x7414, CF_00, TEXT_TRAPZUF,  M_TVEC),
    X0(0x7800, CF_00, TEXT_RETIU,    LF_00),
    X0(0x7800, CF_00, TEXT_RETI,     LF_00),
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
    X0(0x7880, CF_00, TEXT_RETS,     LF_00),
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

constexpr uint8_t INDEX_CALLCOND[] PROGMEM = {
    /* empty */
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage TMS320C30_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C30), ARRAY_RANGE(INDEX_TMS320C30)},
        {ARRAY_RANGE(TABLE_LDCOND), ARRAY_RANGE(INDEX_LDCOND)},
        {ARRAY_RANGE(TABLE_BRCOND), ARRAY_RANGE(INDEX_BRCOND)},
        {ARRAY_RANGE(TABLE_CALLCOND), ARRAY_RANGE(INDEX_CALLCOND)},
};

constexpr EntryPage TMS320C31_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C30), ARRAY_RANGE(INDEX_TMS320C30)},
        {ARRAY_RANGE(TABLE_TMS320C31), ARRAY_RANGE(INDEX_TMS320C31)},
        {ARRAY_RANGE(TABLE_LDCOND), ARRAY_RANGE(INDEX_LDCOND)},
        {ARRAY_RANGE(TABLE_BRCOND), ARRAY_RANGE(INDEX_BRCOND)},
        {ARRAY_RANGE(TABLE_CALLCOND), ARRAY_RANGE(INDEX_CALLCOND)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS320C30, TEXT_CPU_320C30, ARRAY_RANGE(TMS320C30_PAGES)},
        {TMS320C31, TEXT_CPU_320C31, ARRAY_RANGE(TMS320C31_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
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
