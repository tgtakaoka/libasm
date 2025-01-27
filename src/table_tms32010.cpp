/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "table_tms32010.h"
#include "entry_table.h"
#include "entry_tms32010.h"
#include "text_tms32010.h"

using namespace libasm::text::tms32010;

namespace libasm {
namespace tms32010 {

#define E3(_opc, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_opr1, _opr2, _opr3), _name}
#define E2(_opc, _name, _opr1, _opr2) E3(_opc, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _name, _opr1) E2(_opc, _name, _opr1, M_NONE)
#define E0(_opc, _name) E1(_opc, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_TMS32010[] PROGMEM = {
    E3(0x0000, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x1000, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, TEXT_LAC,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x3800, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x4000, TEXT_IN,   M_MAM,  M_PA,   M_NARP),
    E3(0x4800, TEXT_OUT,  M_MAM,  M_PA,   M_NARP),
    E3(0x5000, TEXT_SACL, M_MAM,  M_LS0,  M_NARP),
    E3(0x5800, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E2(0x6000, TEXT_ADDH, M_MAM,  M_NARP),
    E2(0x6100, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x6200, TEXT_SUBH, M_MAM,  M_NARP),
    E2(0x6300, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x6400, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x6500, TEXT_ZALH, M_MAM,  M_NARP),
    E2(0x6600, TEXT_ZALS, M_MAM,  M_NARP),
    E2(0x6700, TEXT_TBLR, M_MAM,  M_NARP),
    E1(0x6880, TEXT_LARP, M_ARK),
    E2(0x6800, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x6900, TEXT_DMOV, M_MAM,  M_NARP),
    E2(0x6A00, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x6B00, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x6C00, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x6D00, TEXT_MPY,  M_MAM,  M_NARP),
    E1(0x6E00, TEXT_LDPK, M_IM1),
    E2(0x6F00, TEXT_LDP,  M_MAM,  M_NARP),
    E2(0x7000, TEXT_LARK, M_AR,   M_IM8),
    E2(0x7800, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x7900, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x7A00, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x7B00, TEXT_LST,  M_MAM,  M_NARP),
    E2(0x7C00, TEXT_SST,  M_MAM,  M_NARP),
    E2(0x7D00, TEXT_TBLW, M_MAM,  M_NARP),
    E1(0x7E00, TEXT_LACK, M_IM8),
    E0(0x7F80, TEXT_NOP),
    E0(0x7F81, TEXT_DINT),
    E0(0x7F82, TEXT_EINT),
    E0(0x7F88, TEXT_ABS),
    E0(0x7F89, TEXT_ZAC),
    E0(0x7F8A, TEXT_ROVM),
    E0(0x7F8B, TEXT_SOVM),
    E0(0x7F8C, TEXT_CALA),
    E0(0x7F8D, TEXT_RET),
    E0(0x7F8E, TEXT_PAC),
    E0(0x7F8F, TEXT_APAC),
    E0(0x7F90, TEXT_SPAC),
    E0(0x7F9C, TEXT_PUSH),
    E0(0x7F9D, TEXT_POP),
    E1(0x8000, TEXT_MPYK, M_IM13),
    E1(0xF400, TEXT_BANZ, M_PM12),
    E1(0xF500, TEXT_BV,   M_PM12),
    E1(0xF600, TEXT_BIOZ, M_PM12),
    E1(0xF800, TEXT_CALL, M_PM12),
    E1(0xF900, TEXT_B,    M_PM12),
    E1(0xFA00, TEXT_BLZ,  M_PM12),
    E1(0xFB00, TEXT_BLEZ, M_PM12),
    E1(0xFC00, TEXT_BGZ,  M_PM12),
    E1(0xFD00, TEXT_BGEZ, M_PM12),
    E1(0xFE00, TEXT_BNZ,  M_PM12),
    E1(0xFF00, TEXT_BZ,   M_PM12),
};

constexpr uint8_t INDEX_TMS32010[] PROGMEM = {
     37,  // TEXT_ABS
      0,  // TEXT_ADD
      9,  // TEXT_ADDH
     10,  // TEXT_ADDS
     28,  // TEXT_AND
     44,  // TEXT_APAC
     53,  // TEXT_B
     49,  // TEXT_BANZ
     57,  // TEXT_BGEZ
     56,  // TEXT_BGZ
     51,  // TEXT_BIOZ
     55,  // TEXT_BLEZ
     54,  // TEXT_BLZ
     58,  // TEXT_BNZ
     50,  // TEXT_BV
     59,  // TEXT_BZ
     41,  // TEXT_CALA
     52,  // TEXT_CALL
     35,  // TEXT_DINT
     19,  // TEXT_DMOV
     36,  // TEXT_EINT
      5,  // TEXT_IN
      2,  // TEXT_LAC
     33,  // TEXT_LACK
      4,  // TEXT_LAR
     26,  // TEXT_LARK
     17,  // TEXT_LARP
     25,  // TEXT_LDP
     24,  // TEXT_LDPK
     30,  // TEXT_LST
     20,  // TEXT_LT
     22,  // TEXT_LTA
     21,  // TEXT_LTD
     18,  // TEXT_MAR
     23,  // TEXT_MPY
     48,  // TEXT_MPYK
     34,  // TEXT_NOP
     29,  // TEXT_OR
      6,  // TEXT_OUT
     43,  // TEXT_PAC
     47,  // TEXT_POP
     46,  // TEXT_PUSH
     42,  // TEXT_RET
     39,  // TEXT_ROVM
      8,  // TEXT_SACH
      7,  // TEXT_SACL
      3,  // TEXT_SAR
     40,  // TEXT_SOVM
     45,  // TEXT_SPAC
     31,  // TEXT_SST
      1,  // TEXT_SUB
     13,  // TEXT_SUBC
     11,  // TEXT_SUBH
     12,  // TEXT_SUBS
     16,  // TEXT_TBLR
     32,  // TEXT_TBLW
     27,  // TEXT_XOR
     38,  // TEXT_ZAC
     14,  // TEXT_ZALH
     15,  // TEXT_ZALS
};

const Entry TABLE_TMS3202X[] PROGMEM = {
    E3(0x0000, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x1000, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, TEXT_LAC,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x3800, TEXT_MPY,  M_MAM,  M_NARP),
    E2(0x3900, TEXT_SQRA, M_MAM,  M_NARP),
    E2(0x3C00, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x3D00, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x3E00, TEXT_LTP,  M_MAM,  M_NARP),
    E2(0x3F00, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x4000, TEXT_ZALH, M_MAM,  M_NARP),
    E2(0x4100, TEXT_ZALS, M_MAM,  M_NARP),
    E2(0x4200, TEXT_LACT, M_MAM,  M_NARP),
    E2(0x4400, TEXT_SUBH, M_MAM,  M_NARP),
    E2(0x4500, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x4600, TEXT_SUBT, M_MAM,  M_NARP),
    E2(0x4700, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x4800, TEXT_ADDH, M_MAM,  M_NARP),
    E2(0x4900, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x4A00, TEXT_ADDT, M_MAM,  M_NARP),
    E2(0x4B00, TEXT_RPT,  M_MAM,  M_NARP),
    E2(0x4C00, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x4D00, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x4E00, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x5000, TEXT_LST,  M_MAM,  M_NARP),
    E2(0x5100, TEXT_LST1, M_MAM,  M_NARP),
    E2(0x5200, TEXT_LDP,  M_MAM,  M_NARP),
    E2(0x5300, TEXT_LPH,  M_MAM,  M_NARP),
    E2(0x5400, TEXT_PSHD, M_MAM,  M_NARP),
    E0(0x5500, TEXT_NOP),
    E1(0x5588, TEXT_LARP, M_ARK),
    E2(0x5500, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x5600, TEXT_DMOV, M_MAM,  M_NARP),
    E2(0x5700, TEXT_BITT, M_MAM,  M_NARP),
    E2(0x5800, TEXT_TBLR, M_MAM,  M_NARP),
    E2(0x5900, TEXT_TBLW, M_MAM,  M_NARP),
    E2(0x5A00, TEXT_SQRS, M_MAM,  M_NARP),
    E2(0x5B00, TEXT_LTS,  M_MAM,  M_NARP),
    E3(0x5C00, TEXT_MACD, M_PM16, M_MAM,  M_NARP),
    E3(0x5D00, TEXT_MAC,  M_PM16, M_MAM,  M_NARP),
    E3(0x6000, TEXT_SACL, M_MAM,  M_LS3,  M_NARP),
    E3(0x6800, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E3(0x7000, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x7800, TEXT_SST,  M_MAM,  M_NARP),
    E2(0x7900, TEXT_SST1, M_MAM,  M_NARP),
    E2(0x7A00, TEXT_POPD, M_MAM,  M_NARP),
    E3(0x8000, TEXT_IN,   M_MAM,  M_PA,   M_NARP),
    E3(0x9000, TEXT_BIT,  M_MAM,  M_BIT,  M_NARP),
    E1(0xA000, TEXT_MPYK, M_IM13),
    E2(0xC000, TEXT_LARK, M_AR,   M_IM8),
    E1(0xC800, TEXT_LDPK, M_IM9),
    E0(0xCA00, TEXT_ZAC),
    E1(0xCA00, TEXT_LACK, M_IM8),
    E1(0xCB00, TEXT_RPTK, M_IM8),
    E0(0xCE00, TEXT_EINT),
    E0(0xCE01, TEXT_DINT),
    E0(0xCE02, TEXT_ROVM),
    E0(0xCE03, TEXT_SOVM),
    E0(0xCE06, TEXT_RSXM),
    E0(0xCE07, TEXT_SSXM),
    E1(0xCE08, TEXT_SPM,  M_IM2),
    E0(0xCE0C, TEXT_RXF),
    E0(0xCE0D, TEXT_SXF),
    E1(0xCE0E, TEXT_FORT, M_IM1),
    E0(0xCE14, TEXT_PAC),
    E0(0xCE15, TEXT_APAC),
    E0(0xCE16, TEXT_SPAC),
    E0(0xCE18, TEXT_SFL),
    E0(0xCE19, TEXT_SFR),
    E0(0xCE1B, TEXT_ABS),
    E0(0xCE1C, TEXT_PUSH),
    E0(0xCE1D, TEXT_POP),
    E0(0xCE1E, TEXT_TRAP),
    E0(0xCE1F, TEXT_IDLE),
    E0(0xCE20, TEXT_RTXM),
    E0(0xCE21, TEXT_STXM),
    E0(0xCE23, TEXT_NEG),
    E0(0xCE24, TEXT_CALA),
    E0(0xCE25, TEXT_BACC),
    E0(0xCE26, TEXT_RET),
    E0(0xCE27, TEXT_CMPL),
    E1(0xCE50, TEXT_CMPR, M_IM2),
    E2(0xD000, TEXT_LRLK, M_AR,   M_PM16),
    E2(0xD001, TEXT_LALK, M_IM16, M_LS4),
    E2(0xD002, TEXT_ADLK, M_IM16, M_LS4),
    E2(0xD003, TEXT_SBLK, M_IM16, M_LS4),
    E2(0xD004, TEXT_ANDK, M_IM16, M_LS4),
    E2(0xD005, TEXT_ORK,  M_IM16, M_LS4),
    E2(0xD006, TEXT_XORK, M_IM16, M_LS4),
    E3(0xE000, TEXT_OUT,  M_MAM,  M_PA,   M_NARP),
    E3(0xF080, TEXT_BV,   M_PM16, M_MAR,  M_NARP),
    E3(0xF180, TEXT_BGZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF280, TEXT_BLEZ, M_PM16, M_MAR,  M_NARP),
    E3(0xF380, TEXT_BLZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF480, TEXT_BGEZ, M_PM16, M_MAR,  M_NARP),
    E3(0xF580, TEXT_BNZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF680, TEXT_BZ,   M_PM16, M_MAR,  M_NARP),
    E3(0xF780, TEXT_BNV,  M_PM16, M_MAR,  M_NARP),
    E3(0xF880, TEXT_BBZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF980, TEXT_BBNZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFA80, TEXT_BIOZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFB80, TEXT_BANZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFC00, TEXT_BLKP, M_PM16, M_MAM,  M_NARP),
    E3(0xFD00, TEXT_BLKD, M_PM16, M_MAM,  M_NARP),
    E3(0xFE80, TEXT_CALL, M_PM16, M_MAR,  M_NARP),
    E3(0xFF80, TEXT_B,    M_PM16, M_MAR,  M_NARP),
};

constexpr uint8_t INDEX_TMS3202X[] PROGMEM = {
     69,  // TEXT_ABS
      0,  // TEXT_ADD
     17,  // TEXT_ADDH
     18,  // TEXT_ADDS
     19,  // TEXT_ADDT
     84,  // TEXT_ADLK
     23,  // TEXT_AND
     86,  // TEXT_ANDK
     65,  // TEXT_APAC
    105,  // TEXT_B
     78,  // TEXT_BACC
    101,  // TEXT_BANZ
     99,  // TEXT_BBNZ
     98,  // TEXT_BBZ
     94,  // TEXT_BGEZ
     91,  // TEXT_BGZ
    100,  // TEXT_BIOZ
     47,  // TEXT_BIT
     33,  // TEXT_BITT
     92,  // TEXT_BLEZ
    103,  // TEXT_BLKD
    102,  // TEXT_BLKP
     93,  // TEXT_BLZ
     97,  // TEXT_BNV
     95,  // TEXT_BNZ
     90,  // TEXT_BV
     96,  // TEXT_BZ
     77,  // TEXT_CALA
    104,  // TEXT_CALL
     80,  // TEXT_CMPL
     81,  // TEXT_CMPR
     55,  // TEXT_DINT
     32,  // TEXT_DMOV
     54,  // TEXT_EINT
     63,  // TEXT_FORT
     73,  // TEXT_IDLE
     46,  // TEXT_IN
      2,  // TEXT_LAC
     52,  // TEXT_LACK
     12,  // TEXT_LACT
     83,  // TEXT_LALK
      3,  // TEXT_LAR
     49,  // TEXT_LARK
     30,  // TEXT_LARP
     26,  // TEXT_LDP
     50,  // TEXT_LDPK
     27,  // TEXT_LPH
     82,  // TEXT_LRLK
     24,  // TEXT_LST
     25,  // TEXT_LST1
      6,  // TEXT_LT
      7,  // TEXT_LTA
      9,  // TEXT_LTD
      8,  // TEXT_LTP
     37,  // TEXT_LTS
     39,  // TEXT_MAC
     38,  // TEXT_MACD
     31,  // TEXT_MAR
      4,  // TEXT_MPY
     48,  // TEXT_MPYK
     76,  // TEXT_NEG
     29,  // TEXT_NOP
     22,  // TEXT_OR
     87,  // TEXT_ORK
     89,  // TEXT_OUT
     64,  // TEXT_PAC
     71,  // TEXT_POP
     45,  // TEXT_POPD
     28,  // TEXT_PSHD
     70,  // TEXT_PUSH
     79,  // TEXT_RET
     56,  // TEXT_ROVM
     20,  // TEXT_RPT
     53,  // TEXT_RPTK
     58,  // TEXT_RSXM
     74,  // TEXT_RTXM
     61,  // TEXT_RXF
     41,  // TEXT_SACH
     40,  // TEXT_SACL
     42,  // TEXT_SAR
     85,  // TEXT_SBLK
     67,  // TEXT_SFL
     68,  // TEXT_SFR
     57,  // TEXT_SOVM
     66,  // TEXT_SPAC
     60,  // TEXT_SPM
      5,  // TEXT_SQRA
     36,  // TEXT_SQRS
     43,  // TEXT_SST
     44,  // TEXT_SST1
     59,  // TEXT_SSXM
     75,  // TEXT_STXM
      1,  // TEXT_SUB
     16,  // TEXT_SUBC
     13,  // TEXT_SUBH
     14,  // TEXT_SUBS
     15,  // TEXT_SUBT
     62,  // TEXT_SXF
     34,  // TEXT_TBLR
     35,  // TEXT_TBLW
     72,  // TEXT_TRAP
     21,  // TEXT_XOR
     88,  // TEXT_XORK
     51,  // TEXT_ZAC
     10,  // TEXT_ZALH
     11,  // TEXT_ZALS
};

constexpr Entry TABLE_TMS320C2X[] PROGMEM = {
    E2(0x3A00, TEXT_MPYA, M_MAM,  M_NARP),
    E2(0x3B00, TEXT_MPYS, M_MAM,  M_NARP),
    E2(0x4300, TEXT_ADDC, M_MAM,  M_NARP),
    E2(0x4F00, TEXT_SUBB, M_MAM,  M_NARP),
    E3(0x5E80, TEXT_BC,   M_PM16, M_MAR, M_NARP),
    E3(0x5F80, TEXT_BNC,  M_PM16, M_MAR, M_NARP),
    E2(0x7B00, TEXT_ZALR, M_MAM,  M_NARP),
    E2(0x7C00, TEXT_SPL,  M_MAM,  M_NARP),
    E2(0x7D00, TEXT_SPH,  M_MAM,  M_NARP),
    E1(0x7E00, TEXT_ADRK, M_IM8),
    E1(0x7F00, TEXT_SBRK, M_IM8),
    E1(0xCC00, TEXT_ADDK, M_IM8),
    E1(0xCD00, TEXT_SUBK, M_IM8),
    E0(0xCE30, TEXT_RC),
    E0(0xCE31, TEXT_SC),
    E0(0xCE32, TEXT_RTC),
    E0(0xCE33, TEXT_STC),
    E0(0xCE34, TEXT_ROL),
    E0(0xCE35, TEXT_ROR),
    E0(0xCE36, TEXT_RFSM),
    E0(0xCE37, TEXT_SFSM),
    E0(0xCE38, TEXT_RHM),
    E0(0xCE39, TEXT_SHM),
    E1(0xCE82, TEXT_NORM, M_IND),
    E2(0xCF00, TEXT_MPYU, M_MAM,  M_NARP),
};

constexpr uint8_t INDEX_TMS320C2X[] = {
      2,  // TEXT_ADDC
     11,  // TEXT_ADDK
      9,  // TEXT_ADRK
      4,  // TEXT_BC
      5,  // TEXT_BNC
      0,  // TEXT_MPYA
      1,  // TEXT_MPYS
     24,  // TEXT_MPYU
     23,  // TEXT_NORM
     13,  // TEXT_RC
     19,  // TEXT_RFSM
     21,  // TEXT_RHM
     17,  // TEXT_ROL
     18,  // TEXT_ROR
     15,  // TEXT_RTC
     10,  // TEXT_SBRK
     14,  // TEXT_SC
     20,  // TEXT_SFSM
     22,  // TEXT_SHM
      8,  // TEXT_SPH
      7,  // TEXT_SPL
     16,  // TEXT_STC
      3,  // TEXT_SUBB
     12,  // TEXT_SUBK
      6,  // TEXT_ZALR
};

constexpr Entry TABLE_TMS32020[] = {
    E0(0xCE82, TEXT_NORM),
};

constexpr uint8_t INDEX_TMS32020[] = {
      0,  // TEXT_NORM
};

constexpr Entry TABLE_TMS320CNFx[] PROGMEM = {
    E0(0xCE04, TEXT_CNFD),
    E0(0xCE05, TEXT_CNFP),
};

constexpr uint8_t INDEX_TMS320CNFx[] = {
      0,  // TEXT_CNFD
      1,  // TEXT_CNFP
};

constexpr Entry TABLE_TMS320CONF[] PROGMEM = {
    E1(0xCE3C, TEXT_CONF, M_IM2),
};

constexpr uint8_t INDEX_TMS320CONF[] = {
      0,  // TEXT_CONF
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage TMS32010_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS32010), ARRAY_RANGE(INDEX_TMS32010)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr EntryPage TMS32020_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS32020), ARRAY_RANGE(INDEX_TMS32020)},
        {ARRAY_RANGE(TABLE_TMS320CNFx), ARRAY_RANGE(INDEX_TMS320CNFx)},
};

constexpr EntryPage TMS320C25_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C2X), ARRAY_RANGE(INDEX_TMS320C2X)},
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS320CNFx), ARRAY_RANGE(INDEX_TMS320CNFx)},
};

constexpr EntryPage TMS320C26_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C2X), ARRAY_RANGE(INDEX_TMS320C2X)},
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS320CONF), ARRAY_RANGE(INDEX_TMS320CONF)},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS32010, TEXT_CPU_32010, ARRAY_RANGE(TMS32010_PAGES)},
        {TMS32015, TEXT_CPU_32015, ARRAY_RANGE(TMS32010_PAGES)},
        {TMS32020, TEXT_CPU_32020, ARRAY_RANGE(TMS32020_PAGES)},
        {TMS320C25, TEXT_CPU_320C25, ARRAY_RANGE(TMS320C25_PAGES)},
        {TMS320C26, TEXT_CPU_320C26, ARRAY_RANGE(TMS320C26_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table) {
    if (opr == table)
        return true;
    if (opr == M_NONE)  // These can be ommitted.
        return table == M_LS0 || table == M_LS3 || table == M_LS4 || table == M_MAR ||
               table == M_NARP;
    if (opr == M_AR)
        return table == M_ARK || table == M_NARP;
    if (opr == M_ARP || opr == M_INC || opr == M_DEC || opr == M_INC0 || opr == M_DEC0 ||
            opr == M_IBR0 || opr == M_DBR0)
        return table == M_MAM || table == M_IND || table == M_MAR;
    if (opr == M_CNST)
        return table == M_MAM || (table >= M_IM1 && table < M_CNST);
    return false;
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

bool matchOpCode(DisInsn &insn, const Entry *entry, CpuType cpuType) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    const auto mode3 = flags.mode3();
    if ((opc & 0x80) != 0 && (mode2 == M_NARP || mode3 == M_NARP)) {
        const auto narp = (opc & 8);
        const auto ar = (opc & 7);
        if (cpuType == TMS32010) {
            if (opc & 0x46)
                return false;  // check reserved bits
            if (narp && ar)
                return false;  // check no next ARP
        } else {
            if (narp == 0 && ar)
                return false;  // check no next ARP
        }
        opc &= ~0xF;
    }

    if ((opc & 0x80) != 0 && (mode1 == M_MAM || mode2 == M_MAM || mode1 == M_MAR ||
                                     mode2 == M_MAR || mode1 == M_IND)) {
        const auto modify = (opc >> 4) & 7;
        if (modify == 3)
            return false;  // Don't allow both increment and decrement.
        opc &= ~0x70;
    }
    if (mode1 == M_MAM || mode2 == M_MAM)
        opc &= ~0xFF;

    if (mode1 == M_IM8 || mode2 == M_IM8) {
        opc &= ~0xFF;
    } else if (mode1 == M_IM1) {
        opc &= ~1;
    } else if (mode1 == M_IM2) {
        opc &= ~3;
    } else if (mode1 == M_IM9) {
        opc &= ~0x1FF;
    } else if (mode1 == M_IM13) {
        opc &= ~0x1FFF;
    }

    const auto maxAR = (cpuType == TMS32010) ? 1 : 7;
    if (mode1 == M_AR)
        opc &= ~(maxAR << 8);
    if (mode1 == M_ARK)
        opc &= ~(maxAR << 0);
    if (mode2 == M_PA) {
        const auto maxPA = (cpuType == TMS32010) ? 7 : 15;
        opc &= ~(maxPA << 8);
    }
    if (mode2 == M_LS4 || mode2 == M_BIT)
        opc &= ~(0xF << 8);
    if (mode2 == M_LS3) {
        const auto shift = (opc >> 8) & 7;
        if (cpuType == TMS320C25 || shift == 0 || shift == 1 || shift == 4) {
            opc &= ~(7 << 8);
        } else {
            return false;
        }
    }
    return opc == entry->readOpCode();
}

bool matchOpCode1x(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS32010);
}

bool matchOpCode20(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS32020);
}

bool matchOpCode2x(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS320C25);
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    const auto c = cpu(cpuType);
    if (cpuType == TMS32010 || cpuType == TMS32015) {
        c->searchOpCode(insn, out, matchOpCode1x);
    } else if (cpuType == TMS32020) {
        c->searchOpCode(insn, out, matchOpCode20);
    } else {
        c->searchOpCode(insn, out, matchOpCode2x);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms32010::listCpu_P() const {
    return TEXT_TMS32010_LIST;
}

const /*PROGMEM*/ char *TableTms32010::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms32010::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_TMS32010_LIST, 3);  // Ignore "TMS" if exist
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableTms32010 TABLE;

}  // namespace tms32010
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
