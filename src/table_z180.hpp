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

#include "table_z80.hpp"

namespace libasm {
namespace z80 {
namespace common {

using namespace libasm::text::z80;

#define E2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::create(_mask, _dst, _src), _name}
#define E1(_opc, _mask, _name, _dst) E2(_opc, _mask, _name, _dst, M_NONE)
#define E0(_opc, _mask, _name) E1(_opc, _mask, _name, M_NONE)
#define I2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::imcapable(_mask, _dst, _src), _name}
#define I1(_opc, _mask, _name, _dst) I2(_opc, _mask, _name, _dst, M_NONE)
#define I0(_opc, _mask, _name) I1(_opc, _mask, _name, M_NONE)
#define L2(_opc, _mask, _name, _dst, _src) {_opc, Entry::Flags::lmcapable(_mask, _dst, _src), _name}
#define L1(_opc, _mask, _name, _dst) L2(_opc, _mask, _name, _dst, M_NONE)
#define L0(_opc, _mask, _name) L1(_opc, _mask, _name, M_NONE)
#define D2(_opc, _mask, _name, _dst, _src) \
    {_opc, Entry::Flags::ilmcapable(_mask, _dst, _src), _name}
#define D1(_opc, _mask, _name, _dst) D2(_opc, _mask, _name, _dst, M_NONE)
#define D0(_opc, _mask, _name) D1(_opc, _mask, _name, M_NONE)

// clang-format off
// Z180 ED:xx
constexpr Entry TABLE_Z180_EXT[] PROGMEM = {
    E1(0x4C, 0060, TEXT_MLT,    M_R16),
    E1(0x04, 0070, TEXT_TST,    M_DST),
    E1(0x64, 0000, TEXT_TST,    M_IM8),
    E1(0x74, 0000, TEXT_TSTIO,  M_IM8),
    E2(0x00, 0070, TEXT_IN0,    M_DR8, M_IOA),
    E2(0x01, 0070, TEXT_OUT0,   M_IOA, M_DR8),
    E0(0x83, 0000, TEXT_OTIM),
    E0(0x93, 0000, TEXT_OTIMR),
    E0(0x8B, 0000, TEXT_OTDM),
    E0(0x9B, 0000, TEXT_OTDMR),
    E0(0x76, 0000, TEXT_SLP),
};

constexpr uint8_t INDEX_Z180_EXT[] PROGMEM = {
      4,  // TEXT_IN0
      0,  // TEXT_MLT
      8,  // TEXT_OTDM
      9,  // TEXT_OTDMR
      6,  // TEXT_OTIM
      7,  // TEXT_OTIMR
      5,  // TEXT_OUT0
     10,  // TEXT_SLP
      1,  // TEXT_TST
      2,  // TEXT_TST
      3,  // TEXT_TSTIO
};
// clang-format on

constexpr EntryPage Z180_PAGES[] PROGMEM = {
        {0x0000, ARRAY_RANGE(TABLE_Z80), ARRAY_RANGE(INDEX_Z80)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z80_EXT), ARRAY_RANGE(INDEX_Z80_EXT)},
        {Entry::EXT, ARRAY_RANGE(TABLE_Z180_EXT), ARRAY_RANGE(INDEX_Z180_EXT)},
        {Entry::BIT, ARRAY_RANGE(TABLE_Z80_BIT), ARRAY_RANGE(INDEX_Z80_BIT)},
        {Entry::IX, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IY, ARRAY_RANGE(TABLE_Z80_IDX), ARRAY_RANGE(INDEX_Z80_IDX)},
        {Entry::IXBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
        {Entry::IYBIT, ARRAY_RANGE(TABLE_Z80_IDXBIT), ARRAY_RANGE(INDEX_Z80_IDXBIT)},
};

}  // namespace common
}  // namespace z80
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
