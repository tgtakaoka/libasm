/*
 * Copyright 2026 Tadashi G. Takaoka
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

#include "table_c166.h"
#include "entry_c166.h"
#include "entry_table.h"
#include "reg_c166.h"
#include "text_c166.h"

using namespace libasm::text::c166;
using namespace libasm::c166::reg;

namespace libasm {
namespace c166 {

#define E2(_opc, _post, _cf, _name, _dst, _src) \
    {_opc, _post, Entry::Flags::create(_cf, _dst, _src), _name}
#define E1(_opc, _post, _cf, _name, _dst) E2(_opc, _post, _cf, _name, _dst, M_NONE)
#define E0(_opc, _post, _cf, _name) E1(_opc, _post, _cf, _name, M_NONE)
// A CoXXX instruction, whose 3rd byte is a sub-opcode.
#define X3(_opc, _post, _cf, _sub, _name, _dst, _src, _ext) \
    {_opc, _post, Entry::Flags::create(_cf, _dst, _src, _ext, true, true, _sub), _name}
// A CoXXX instruction without a sub-opcode.
#define S3(_opc, _post, _cf, _name, _dst, _src, _ext) \
    {_opc, _post, Entry::Flags::create(_cf, _dst, _src, _ext, true), _name}

// clang-format off
constexpr Entry TABLE_C166[] PROGMEM = {
    E2(0x00, 0x00, CF_FF, TEXT_ADD,    M_RWH,  M_RWL),
    E2(0x08, 0x08, CF_F3, TEXT_ADD,    M_RWH,  M_IR2),
    E2(0x08, 0x0C, CF_F3, TEXT_ADD,    M_RWH,  M_PI2),
    E2(0x08, 0x00, CF_F7, TEXT_ADD,    M_RWH,  M_D3),
    E2(0x06, 0x00, CF_FF, TEXT_ADD,    M_REGW, M_IM16),
    E2(0x02, 0x00, CF_FF, TEXT_ADD,    M_REGW, M_MEM),
    E2(0x04, 0x00, CF_FF, TEXT_ADD,    M_MEM,  M_REGW),
    E2(0x01, 0x00, CF_FF, TEXT_ADDB,   M_RBH,  M_RBL),
    E2(0x09, 0x08, CF_F3, TEXT_ADDB,   M_RBH,  M_IR2),
    E2(0x09, 0x0C, CF_F3, TEXT_ADDB,   M_RBH,  M_PI2),
    E2(0x09, 0x00, CF_F7, TEXT_ADDB,   M_RBH,  M_D3),
    E2(0x07, 0x00, CF_FF, TEXT_ADDB,   M_REGB, M_IM8),
    E2(0x03, 0x00, CF_FF, TEXT_ADDB,   M_REGB, M_MEM),
    E2(0x05, 0x00, CF_FF, TEXT_ADDB,   M_MEM,  M_REGB),
    E2(0x10, 0x00, CF_FF, TEXT_ADDC,   M_RWH,  M_RWL),
    E2(0x18, 0x08, CF_F3, TEXT_ADDC,   M_RWH,  M_IR2),
    E2(0x18, 0x0C, CF_F3, TEXT_ADDC,   M_RWH,  M_PI2),
    E2(0x18, 0x00, CF_F7, TEXT_ADDC,   M_RWH,  M_D3),
    E2(0x16, 0x00, CF_FF, TEXT_ADDC,   M_REGW, M_IM16),
    E2(0x12, 0x00, CF_FF, TEXT_ADDC,   M_REGW, M_MEM),
    E2(0x14, 0x00, CF_FF, TEXT_ADDC,   M_MEM,  M_REGW),
    E2(0x11, 0x00, CF_FF, TEXT_ADDCB,  M_RBH,  M_RBL),
    E2(0x19, 0x08, CF_F3, TEXT_ADDCB,  M_RBH,  M_IR2),
    E2(0x19, 0x0C, CF_F3, TEXT_ADDCB,  M_RBH,  M_PI2),
    E2(0x19, 0x00, CF_F7, TEXT_ADDCB,  M_RBH,  M_D3),
    E2(0x17, 0x00, CF_FF, TEXT_ADDCB,  M_REGB, M_IM8),
    E2(0x13, 0x00, CF_FF, TEXT_ADDCB,  M_REGB, M_MEM),
    E2(0x15, 0x00, CF_FF, TEXT_ADDCB,  M_MEM,  M_REGB),
    E2(0x20, 0x00, CF_FF, TEXT_SUB,    M_RWH,  M_RWL),
    E2(0x28, 0x08, CF_F3, TEXT_SUB,    M_RWH,  M_IR2),
    E2(0x28, 0x0C, CF_F3, TEXT_SUB,    M_RWH,  M_PI2),
    E2(0x28, 0x00, CF_F7, TEXT_SUB,    M_RWH,  M_D3),
    E2(0x26, 0x00, CF_FF, TEXT_SUB,    M_REGW, M_IM16),
    E2(0x22, 0x00, CF_FF, TEXT_SUB,    M_REGW, M_MEM),
    E2(0x24, 0x00, CF_FF, TEXT_SUB,    M_MEM,  M_REGW),
    E2(0x21, 0x00, CF_FF, TEXT_SUBB,   M_RBH,  M_RBL),
    E2(0x29, 0x08, CF_F3, TEXT_SUBB,   M_RBH,  M_IR2),
    E2(0x29, 0x0C, CF_F3, TEXT_SUBB,   M_RBH,  M_PI2),
    E2(0x29, 0x00, CF_F7, TEXT_SUBB,   M_RBH,  M_D3),
    E2(0x27, 0x00, CF_FF, TEXT_SUBB,   M_REGB, M_IM8),
    E2(0x23, 0x00, CF_FF, TEXT_SUBB,   M_REGB, M_MEM),
    E2(0x25, 0x00, CF_FF, TEXT_SUBB,   M_MEM,  M_REGB),
    E2(0x30, 0x00, CF_FF, TEXT_SUBC,   M_RWH,  M_RWL),
    E2(0x38, 0x08, CF_F3, TEXT_SUBC,   M_RWH,  M_IR2),
    E2(0x38, 0x0C, CF_F3, TEXT_SUBC,   M_RWH,  M_PI2),
    E2(0x38, 0x00, CF_F7, TEXT_SUBC,   M_RWH,  M_D3),
    E2(0x36, 0x00, CF_FF, TEXT_SUBC,   M_REGW, M_IM16),
    E2(0x32, 0x00, CF_FF, TEXT_SUBC,   M_REGW, M_MEM),
    E2(0x34, 0x00, CF_FF, TEXT_SUBC,   M_MEM,  M_REGW),
    E2(0x31, 0x00, CF_FF, TEXT_SUBCB,  M_RBH,  M_RBL),
    E2(0x39, 0x08, CF_F3, TEXT_SUBCB,  M_RBH,  M_IR2),
    E2(0x39, 0x0C, CF_F3, TEXT_SUBCB,  M_RBH,  M_PI2),
    E2(0x39, 0x00, CF_F7, TEXT_SUBCB,  M_RBH,  M_D3),
    E2(0x37, 0x00, CF_FF, TEXT_SUBCB,  M_REGB, M_IM8),
    E2(0x33, 0x00, CF_FF, TEXT_SUBCB,  M_REGB, M_MEM),
    E2(0x35, 0x00, CF_FF, TEXT_SUBCB,  M_MEM,  M_REGB),
    E2(0x40, 0x00, CF_FF, TEXT_CMP,    M_RWH,  M_RWL),
    E2(0x48, 0x08, CF_F3, TEXT_CMP,    M_RWH,  M_IR2),
    E2(0x48, 0x0C, CF_F3, TEXT_CMP,    M_RWH,  M_PI2),
    E2(0x48, 0x00, CF_F7, TEXT_CMP,    M_RWH,  M_D3),
    E2(0x46, 0x00, CF_FF, TEXT_CMP,    M_REGW, M_IM16),
    E2(0x42, 0x00, CF_FF, TEXT_CMP,    M_REGW, M_MEM),
    E2(0x41, 0x00, CF_FF, TEXT_CMPB,   M_RBH,  M_RBL),
    E2(0x49, 0x08, CF_F3, TEXT_CMPB,   M_RBH,  M_IR2),
    E2(0x49, 0x0C, CF_F3, TEXT_CMPB,   M_RBH,  M_PI2),
    E2(0x49, 0x00, CF_F7, TEXT_CMPB,   M_RBH,  M_D3),
    E2(0x47, 0x00, CF_FF, TEXT_CMPB,   M_REGB, M_IM8),
    E2(0x43, 0x00, CF_FF, TEXT_CMPB,   M_REGB, M_MEM),
    E2(0x50, 0x00, CF_FF, TEXT_XOR,    M_RWH,  M_RWL),
    E2(0x58, 0x08, CF_F3, TEXT_XOR,    M_RWH,  M_IR2),
    E2(0x58, 0x0C, CF_F3, TEXT_XOR,    M_RWH,  M_PI2),
    E2(0x58, 0x00, CF_F7, TEXT_XOR,    M_RWH,  M_D3),
    E2(0x56, 0x00, CF_FF, TEXT_XOR,    M_REGW, M_IM16),
    E2(0x52, 0x00, CF_FF, TEXT_XOR,    M_REGW, M_MEM),
    E2(0x54, 0x00, CF_FF, TEXT_XOR,    M_MEM,  M_REGW),
    E2(0x51, 0x00, CF_FF, TEXT_XORB,   M_RBH,  M_RBL),
    E2(0x59, 0x08, CF_F3, TEXT_XORB,   M_RBH,  M_IR2),
    E2(0x59, 0x0C, CF_F3, TEXT_XORB,   M_RBH,  M_PI2),
    E2(0x59, 0x00, CF_F7, TEXT_XORB,   M_RBH,  M_D3),
    E2(0x57, 0x00, CF_FF, TEXT_XORB,   M_REGB, M_IM8),
    E2(0x53, 0x00, CF_FF, TEXT_XORB,   M_REGB, M_MEM),
    E2(0x55, 0x00, CF_FF, TEXT_XORB,   M_MEM,  M_REGB),
    E2(0x60, 0x00, CF_FF, TEXT_AND,    M_RWH,  M_RWL),
    E2(0x68, 0x08, CF_F3, TEXT_AND,    M_RWH,  M_IR2),
    E2(0x68, 0x0C, CF_F3, TEXT_AND,    M_RWH,  M_PI2),
    E2(0x68, 0x00, CF_F7, TEXT_AND,    M_RWH,  M_D3),
    E2(0x66, 0x00, CF_FF, TEXT_AND,    M_REGW, M_IM16),
    E2(0x62, 0x00, CF_FF, TEXT_AND,    M_REGW, M_MEM),
    E2(0x64, 0x00, CF_FF, TEXT_AND,    M_MEM,  M_REGW),
    E2(0x61, 0x00, CF_FF, TEXT_ANDB,   M_RBH,  M_RBL),
    E2(0x69, 0x08, CF_F3, TEXT_ANDB,   M_RBH,  M_IR2),
    E2(0x69, 0x0C, CF_F3, TEXT_ANDB,   M_RBH,  M_PI2),
    E2(0x69, 0x00, CF_F7, TEXT_ANDB,   M_RBH,  M_D3),
    E2(0x67, 0x00, CF_FF, TEXT_ANDB,   M_REGB, M_IM8),
    E2(0x63, 0x00, CF_FF, TEXT_ANDB,   M_REGB, M_MEM),
    E2(0x65, 0x00, CF_FF, TEXT_ANDB,   M_MEM,  M_REGB),
    E2(0x70, 0x00, CF_FF, TEXT_OR,     M_RWH,  M_RWL),
    E2(0x78, 0x08, CF_F3, TEXT_OR,     M_RWH,  M_IR2),
    E2(0x78, 0x0C, CF_F3, TEXT_OR,     M_RWH,  M_PI2),
    E2(0x78, 0x00, CF_F7, TEXT_OR,     M_RWH,  M_D3),
    E2(0x76, 0x00, CF_FF, TEXT_OR,     M_REGW, M_IM16),
    E2(0x72, 0x00, CF_FF, TEXT_OR,     M_REGW, M_MEM),
    E2(0x74, 0x00, CF_FF, TEXT_OR,     M_MEM,  M_REGW),
    E2(0x71, 0x00, CF_FF, TEXT_ORB,    M_RBH,  M_RBL),
    E2(0x79, 0x08, CF_F3, TEXT_ORB,    M_RBH,  M_IR2),
    E2(0x79, 0x0C, CF_F3, TEXT_ORB,    M_RBH,  M_PI2),
    E2(0x79, 0x00, CF_F7, TEXT_ORB,    M_RBH,  M_D3),
    E2(0x77, 0x00, CF_FF, TEXT_ORB,    M_REGB, M_IM8),
    E2(0x73, 0x00, CF_FF, TEXT_ORB,    M_REGB, M_MEM),
    E2(0x75, 0x00, CF_FF, TEXT_ORB,    M_MEM,  M_REGB),
    E2(0x0A, 0x00, CF_FF, TEXT_BFLDL,  M_BOFF, M_MDL),
    E2(0x1A, 0x00, CF_FF, TEXT_BFLDH,  M_BOFF, M_MDH),
    E2(0x2A, 0x00, CF_FF, TEXT_BCMP,   M_BITZ, M_BITQ),
    E2(0x3A, 0x00, CF_FF, TEXT_BMOVN,  M_BITZ, M_BITQ),
    E2(0x4A, 0x00, CF_FF, TEXT_BMOV,   M_BITZ, M_BITQ),
    E2(0x5A, 0x00, CF_FF, TEXT_BOR,    M_BITZ, M_BITQ),
    E2(0x6A, 0x00, CF_FF, TEXT_BAND,   M_BITZ, M_BITQ),
    E2(0x7A, 0x00, CF_FF, TEXT_BXOR,   M_BITZ, M_BITQ),
    E2(0x0B, 0x00, CF_FF, TEXT_MUL,    M_RWH,  M_RWL),
    E2(0x1B, 0x00, CF_FF, TEXT_MULU,   M_RWH,  M_RWL),
    E2(0x2B, 0x00, CF_FF, TEXT_PRIOR,  M_RWH,  M_RWL),
    E1(0x4B, 0x00, CF_NN, TEXT_DIV,    M_RWH),
    E1(0x5B, 0x00, CF_NN, TEXT_DIVU,   M_RWH),
    E1(0x6B, 0x00, CF_NN, TEXT_DIVL,   M_RWH),
    E1(0x7B, 0x00, CF_NN, TEXT_DIVLU,  M_RWH),
    E2(0x0C, 0x00, CF_FF, TEXT_ROL,    M_RWH,  M_RWL),
    E2(0x1C, 0x00, CF_FF, TEXT_ROL,    M_RWL,  M_D4),
    E2(0x2C, 0x00, CF_FF, TEXT_ROR,    M_RWH,  M_RWL),
    E2(0x3C, 0x00, CF_FF, TEXT_ROR,    M_RWL,  M_D4),
    E2(0x4C, 0x00, CF_FF, TEXT_SHL,    M_RWH,  M_RWL),
    E2(0x5C, 0x00, CF_FF, TEXT_SHL,    M_RWL,  M_D4),
    E2(0x6C, 0x00, CF_FF, TEXT_SHR,    M_RWH,  M_RWL),
    E2(0x7C, 0x00, CF_FF, TEXT_SHR,    M_RWL,  M_D4),
    E2(0xAC, 0x00, CF_FF, TEXT_ASHR,   M_RWH,  M_RWL),
    E2(0xBC, 0x00, CF_FF, TEXT_ASHR,   M_RWL,  M_D4),
    E2(0x0D, 0x00, CF_OP, TEXT_JMPR,   M_CCO,  M_REL),
    E1(0x0E, 0x00, CF_OP, TEXT_BCLR,   M_BITO),
    E1(0x0F, 0x00, CF_OP, TEXT_BSET,   M_BITO),
    E2(0x80, 0x00, CF_FF, TEXT_CMPI1,  M_RWL,  M_D4),
    E2(0x86, 0xF0, CF_0F, TEXT_CMPI1,  M_RWL,  M_IM16),
    E2(0x82, 0xF0, CF_0F, TEXT_CMPI1,  M_RWL,  M_MEM),
    E2(0x90, 0x00, CF_FF, TEXT_CMPI2,  M_RWL,  M_D4),
    E2(0x96, 0xF0, CF_0F, TEXT_CMPI2,  M_RWL,  M_IM16),
    E2(0x92, 0xF0, CF_0F, TEXT_CMPI2,  M_RWL,  M_MEM),
    E2(0xA0, 0x00, CF_FF, TEXT_CMPD1,  M_RWL,  M_D4),
    E2(0xA6, 0xF0, CF_0F, TEXT_CMPD1,  M_RWL,  M_IM16),
    E2(0xA2, 0xF0, CF_0F, TEXT_CMPD1,  M_RWL,  M_MEM),
    E2(0xB0, 0x00, CF_FF, TEXT_CMPD2,  M_RWL,  M_D4),
    E2(0xB6, 0xF0, CF_0F, TEXT_CMPD2,  M_RWL,  M_IM16),
    E2(0xB2, 0xF0, CF_0F, TEXT_CMPD2,  M_RWL,  M_MEM),
    E1(0x81, 0x00, CF_F0, TEXT_NEG,    M_RWH),
    E1(0x91, 0x00, CF_F0, TEXT_CPL,    M_RWH),
    E1(0xA1, 0x00, CF_F0, TEXT_NEGB,   M_RBH),
    E1(0xB1, 0x00, CF_F0, TEXT_CPLB,   M_RBH),
    E2(0x84, 0x00, CF_0F, TEXT_MOV,    M_IRL,  M_MEM),
    E2(0x94, 0x00, CF_0F, TEXT_MOV,    M_MEM,  M_IRL),
    E2(0xA4, 0x00, CF_0F, TEXT_MOVB,   M_IRL,  M_MEM),
    E2(0xB4, 0x00, CF_0F, TEXT_MOVB,   M_MEM,  M_IRL),
    E0(0x87, 0x78, CF_PD, TEXT_IDLE),
    E0(0x97, 0x68, CF_PD, TEXT_PWRDN),
    E0(0xA7, 0x58, CF_PD, TEXT_SRVWDT),
    E0(0xB7, 0x48, CF_PD, TEXT_SRST),
    E0(0xA5, 0x5A, CF_PD, TEXT_DISWDT),
    E0(0xB5, 0x4A, CF_PD, TEXT_EINIT),
    E2(0xF0, 0x00, CF_FF, TEXT_MOV,    M_RWH,  M_RWL),
    E2(0xE0, 0x00, CF_FF, TEXT_MOV,    M_RWL,  M_D4),
    E2(0xA8, 0x00, CF_FF, TEXT_MOV,    M_RWH,  M_IRL),
    E2(0x98, 0x00, CF_FF, TEXT_MOV,    M_RWH,  M_PIL),
    E2(0xB8, 0x00, CF_FF, TEXT_MOV,    M_IRL,  M_RWH),
    E2(0x88, 0x00, CF_FF, TEXT_MOV,    M_PDL,  M_RWH),
    E2(0xC8, 0x00, CF_FF, TEXT_MOV,    M_IRH,  M_IRL),
    E2(0xD8, 0x00, CF_FF, TEXT_MOV,    M_PIH,  M_IRL),
    E2(0xE8, 0x00, CF_FF, TEXT_MOV,    M_IRH,  M_PIL),
    E2(0xF1, 0x00, CF_FF, TEXT_MOVB,   M_RBH,  M_RBL),
    E2(0xE1, 0x00, CF_FF, TEXT_MOVB,   M_RBL,  M_D4),
    E2(0xA9, 0x00, CF_FF, TEXT_MOVB,   M_RBH,  M_IRL),
    E2(0x99, 0x00, CF_FF, TEXT_MOVB,   M_RBH,  M_PIL),
    E2(0xB9, 0x00, CF_FF, TEXT_MOVB,   M_IRL,  M_RBH),
    E2(0x89, 0x00, CF_FF, TEXT_MOVB,   M_PDL,  M_RBH),
    E2(0xC9, 0x00, CF_FF, TEXT_MOVB,   M_IRH,  M_IRL),
    E2(0xD9, 0x00, CF_FF, TEXT_MOVB,   M_PIH,  M_IRL),
    E2(0xE9, 0x00, CF_FF, TEXT_MOVB,   M_IRH,  M_PIL),
    E2(0xE6, 0x00, CF_FF, TEXT_MOV,    M_REGW, M_IM16),
    E2(0xE7, 0x00, CF_FF, TEXT_MOVB,   M_REGB, M_IM8),
    E2(0xD4, 0x00, CF_FF, TEXT_MOV,    M_RWH,  M_IXL),
    E2(0xC4, 0x00, CF_FF, TEXT_MOV,    M_IXL,  M_RWH),
    E2(0xF4, 0x00, CF_FF, TEXT_MOVB,   M_RBH,  M_IXL),
    E2(0xE4, 0x00, CF_FF, TEXT_MOVB,   M_IXL,  M_RBH),
    E2(0xF2, 0x00, CF_FF, TEXT_MOV,    M_REGW, M_MEM),
    E2(0xF6, 0x00, CF_FF, TEXT_MOV,    M_MEM,  M_REGW),
    E2(0xF3, 0x00, CF_FF, TEXT_MOVB,   M_REGB, M_MEM),
    E2(0xF7, 0x00, CF_FF, TEXT_MOVB,   M_MEM,  M_REGB),
    E2(0xC0, 0x00, CF_FF, TEXT_MOVBZ,  M_RWL,  M_RBH),
    E2(0xC2, 0x00, CF_FF, TEXT_MOVBZ,  M_REGW, M_MEM),
    E2(0xC5, 0x00, CF_FF, TEXT_MOVBZ,  M_MEM,  M_REGB),
    E2(0xD0, 0x00, CF_FF, TEXT_MOVBS,  M_RWL,  M_RBH),
    E2(0xD2, 0x00, CF_FF, TEXT_MOVBS,  M_REGW, M_MEM),
    E2(0xD5, 0x00, CF_FF, TEXT_MOVBS,  M_MEM,  M_REGB),
    E2(0x8A, 0x00, CF_FF, TEXT_JB,     M_BITQ, M_RELW),
    E2(0x9A, 0x00, CF_FF, TEXT_JNB,    M_BITQ, M_RELW),
    E2(0xAA, 0x00, CF_FF, TEXT_JBC,    M_BITQ, M_RELW),
    E2(0xBA, 0x00, CF_FF, TEXT_JNBS,   M_BITQ, M_RELW),
    E1(0x9B, 0x00, CF_FE, TEXT_TRAP,   M_TRAP),
    E2(0xAB, 0x00, CF_FF, TEXT_CALLI,  M_CC,   M_IRL),
    E2(0x9C, 0x00, CF_FF, TEXT_JMPI,   M_CC,   M_IRL),
    E1(0xBB, 0x00, CF_FF, TEXT_CALLR,  M_REL),
    E2(0xC6, 0x00, CF_FF, TEXT_SCXT,   M_REGW, M_IM16),
    E2(0xD6, 0x00, CF_FF, TEXT_SCXT,   M_REGW, M_MEM),
    E2(0xCA, 0x00, CF_F0, TEXT_CALLA,  M_CC,   M_CADR),
    E2(0xEA, 0x00, CF_F0, TEXT_JMPA,   M_CC,   M_CADR),
    E2(0xDA, 0x00, CF_FF, TEXT_CALLS,  M_SEG,  M_CADR),
    E2(0xFA, 0x00, CF_FF, TEXT_JMPS,   M_SEG,  M_CADR),
    E2(0xE2, 0x00, CF_FF, TEXT_PCALL,  M_REGW, M_CADR),
    E0(0xCB, 0x00, CF_00, TEXT_RET),
    E0(0xDB, 0x00, CF_00, TEXT_RETS),
    E0(0xFB, 0x88, CF_00, TEXT_RETI),
    E1(0xEB, 0x00, CF_FF, TEXT_RETP,   M_REGW),
    E0(0xCC, 0x00, CF_00, TEXT_NOP),
    E1(0xEC, 0x00, CF_FF, TEXT_PUSH,   M_REGW),
    E1(0xFC, 0x00, CF_FF, TEXT_POP,    M_REGW),
};

constexpr uint8_t INDEX_C166[] PROGMEM = {
      0,  // TEXT_ADD
      1,  // TEXT_ADD
      2,  // TEXT_ADD
      3,  // TEXT_ADD
      4,  // TEXT_ADD
      5,  // TEXT_ADD
      6,  // TEXT_ADD
      7,  // TEXT_ADDB
      8,  // TEXT_ADDB
      9,  // TEXT_ADDB
     10,  // TEXT_ADDB
     11,  // TEXT_ADDB
     12,  // TEXT_ADDB
     13,  // TEXT_ADDB
     14,  // TEXT_ADDC
     15,  // TEXT_ADDC
     16,  // TEXT_ADDC
     17,  // TEXT_ADDC
     18,  // TEXT_ADDC
     19,  // TEXT_ADDC
     20,  // TEXT_ADDC
     21,  // TEXT_ADDCB
     22,  // TEXT_ADDCB
     23,  // TEXT_ADDCB
     24,  // TEXT_ADDCB
     25,  // TEXT_ADDCB
     26,  // TEXT_ADDCB
     27,  // TEXT_ADDCB
     82,  // TEXT_AND
     83,  // TEXT_AND
     84,  // TEXT_AND
     85,  // TEXT_AND
     86,  // TEXT_AND
     87,  // TEXT_AND
     88,  // TEXT_AND
     89,  // TEXT_ANDB
     90,  // TEXT_ANDB
     91,  // TEXT_ANDB
     92,  // TEXT_ANDB
     93,  // TEXT_ANDB
     94,  // TEXT_ANDB
     95,  // TEXT_ANDB
    133,  // TEXT_ASHR
    134,  // TEXT_ASHR
    116,  // TEXT_BAND
    136,  // TEXT_BCLR
    112,  // TEXT_BCMP
    111,  // TEXT_BFLDH
    110,  // TEXT_BFLDL
    114,  // TEXT_BMOV
    113,  // TEXT_BMOVN
    115,  // TEXT_BOR
    137,  // TEXT_BSET
    117,  // TEXT_BXOR
    208,  // TEXT_CALLA
    203,  // TEXT_CALLI
    205,  // TEXT_CALLR
    210,  // TEXT_CALLS
     56,  // TEXT_CMP
     57,  // TEXT_CMP
     58,  // TEXT_CMP
     59,  // TEXT_CMP
     60,  // TEXT_CMP
     61,  // TEXT_CMP
     62,  // TEXT_CMPB
     63,  // TEXT_CMPB
     64,  // TEXT_CMPB
     65,  // TEXT_CMPB
     66,  // TEXT_CMPB
     67,  // TEXT_CMPB
    144,  // TEXT_CMPD1
    145,  // TEXT_CMPD1
    146,  // TEXT_CMPD1
    147,  // TEXT_CMPD2
    148,  // TEXT_CMPD2
    149,  // TEXT_CMPD2
    138,  // TEXT_CMPI1
    139,  // TEXT_CMPI1
    140,  // TEXT_CMPI1
    141,  // TEXT_CMPI2
    142,  // TEXT_CMPI2
    143,  // TEXT_CMPI2
    151,  // TEXT_CPL
    153,  // TEXT_CPLB
    162,  // TEXT_DISWDT
    121,  // TEXT_DIV
    123,  // TEXT_DIVL
    124,  // TEXT_DIVLU
    122,  // TEXT_DIVU
    163,  // TEXT_EINIT
    158,  // TEXT_IDLE
    198,  // TEXT_JB
    200,  // TEXT_JBC
    209,  // TEXT_JMPA
    204,  // TEXT_JMPI
    135,  // TEXT_JMPR
    211,  // TEXT_JMPS
    199,  // TEXT_JNB
    201,  // TEXT_JNBS
    154,  // TEXT_MOV
    155,  // TEXT_MOV
    164,  // TEXT_MOV
    165,  // TEXT_MOV
    166,  // TEXT_MOV
    167,  // TEXT_MOV
    168,  // TEXT_MOV
    169,  // TEXT_MOV
    170,  // TEXT_MOV
    171,  // TEXT_MOV
    172,  // TEXT_MOV
    182,  // TEXT_MOV
    184,  // TEXT_MOV
    185,  // TEXT_MOV
    188,  // TEXT_MOV
    189,  // TEXT_MOV
    156,  // TEXT_MOVB
    157,  // TEXT_MOVB
    173,  // TEXT_MOVB
    174,  // TEXT_MOVB
    175,  // TEXT_MOVB
    176,  // TEXT_MOVB
    177,  // TEXT_MOVB
    178,  // TEXT_MOVB
    179,  // TEXT_MOVB
    180,  // TEXT_MOVB
    181,  // TEXT_MOVB
    183,  // TEXT_MOVB
    186,  // TEXT_MOVB
    187,  // TEXT_MOVB
    190,  // TEXT_MOVB
    191,  // TEXT_MOVB
    195,  // TEXT_MOVBS
    196,  // TEXT_MOVBS
    197,  // TEXT_MOVBS
    192,  // TEXT_MOVBZ
    193,  // TEXT_MOVBZ
    194,  // TEXT_MOVBZ
    118,  // TEXT_MUL
    119,  // TEXT_MULU
    150,  // TEXT_NEG
    152,  // TEXT_NEGB
    217,  // TEXT_NOP
     96,  // TEXT_OR
     97,  // TEXT_OR
     98,  // TEXT_OR
     99,  // TEXT_OR
    100,  // TEXT_OR
    101,  // TEXT_OR
    102,  // TEXT_OR
    103,  // TEXT_ORB
    104,  // TEXT_ORB
    105,  // TEXT_ORB
    106,  // TEXT_ORB
    107,  // TEXT_ORB
    108,  // TEXT_ORB
    109,  // TEXT_ORB
    212,  // TEXT_PCALL
    219,  // TEXT_POP
    120,  // TEXT_PRIOR
    218,  // TEXT_PUSH
    159,  // TEXT_PWRDN
    213,  // TEXT_RET
    215,  // TEXT_RETI
    216,  // TEXT_RETP
    214,  // TEXT_RETS
    125,  // TEXT_ROL
    126,  // TEXT_ROL
    127,  // TEXT_ROR
    128,  // TEXT_ROR
    206,  // TEXT_SCXT
    207,  // TEXT_SCXT
    129,  // TEXT_SHL
    130,  // TEXT_SHL
    131,  // TEXT_SHR
    132,  // TEXT_SHR
    161,  // TEXT_SRST
    160,  // TEXT_SRVWDT
     28,  // TEXT_SUB
     29,  // TEXT_SUB
     30,  // TEXT_SUB
     31,  // TEXT_SUB
     32,  // TEXT_SUB
     33,  // TEXT_SUB
     34,  // TEXT_SUB
     35,  // TEXT_SUBB
     36,  // TEXT_SUBB
     37,  // TEXT_SUBB
     38,  // TEXT_SUBB
     39,  // TEXT_SUBB
     40,  // TEXT_SUBB
     41,  // TEXT_SUBB
     42,  // TEXT_SUBC
     43,  // TEXT_SUBC
     44,  // TEXT_SUBC
     45,  // TEXT_SUBC
     46,  // TEXT_SUBC
     47,  // TEXT_SUBC
     48,  // TEXT_SUBC
     49,  // TEXT_SUBCB
     50,  // TEXT_SUBCB
     51,  // TEXT_SUBCB
     52,  // TEXT_SUBCB
     53,  // TEXT_SUBCB
     54,  // TEXT_SUBCB
     55,  // TEXT_SUBCB
    202,  // TEXT_TRAP
     68,  // TEXT_XOR
     69,  // TEXT_XOR
     70,  // TEXT_XOR
     71,  // TEXT_XOR
     72,  // TEXT_XOR
     73,  // TEXT_XOR
     74,  // TEXT_XOR
     75,  // TEXT_XORB
     76,  // TEXT_XORB
     77,  // TEXT_XORB
     78,  // TEXT_XORB
     79,  // TEXT_XORB
     80,  // TEXT_XORB
     81,  // TEXT_XORB
};

// Not available on the 80C166.
constexpr Entry TABLE_C167[] PROGMEM = {
    E1(0xD1, 0x00, CF_30, TEXT_ATOMIC, M_RNG),
    E1(0xD1, 0x80, CF_30, TEXT_EXTR,   M_RNGR),
    E2(0xDC, 0x00, CF_3F, TEXT_EXTS,   M_RWL,  M_RNG),
    E2(0xDC, 0x40, CF_3F, TEXT_EXTP,   M_RWL,  M_RNG),
    E2(0xDC, 0x80, CF_3F, TEXT_EXTSR,  M_RWL,  M_RNGR),
    E2(0xDC, 0xC0, CF_3F, TEXT_EXTPR,  M_RWL,  M_RNGR),
    E2(0xD7, 0x00, CF_30, TEXT_EXTS,   M_SEG8, M_RNG),
    E2(0xD7, 0x40, CF_30, TEXT_EXTP,   M_PAG,  M_RNG),
    E2(0xD7, 0x80, CF_30, TEXT_EXTSR,  M_SEG8, M_RNGR),
    E2(0xD7, 0xC0, CF_30, TEXT_EXTPR,  M_PAG,  M_RNGR),
};

constexpr uint8_t INDEX_C167[] PROGMEM = {
      0,  // TEXT_ATOMIC
      3,  // TEXT_EXTP
      7,  // TEXT_EXTP
      5,  // TEXT_EXTPR
      9,  // TEXT_EXTPR
      1,  // TEXT_EXTR
      2,  // TEXT_EXTS
      6,  // TEXT_EXTS
      4,  // TEXT_EXTSR
      8,  // TEXT_EXTSR
};
// Not available on the 80C166 and the 80C167.
constexpr Entry TABLE_XC16X[] PROGMEM = {
    E2(0xEA, 0x00, CF_FA, TEXT_JMPA,    M_XCC,  M_CADR),
    E2(0xEA, 0x00, CF_FA, TEXT_JMPA_P,  M_XCC,  M_CADR),
    E2(0xEA, 0x01, CF_FA, TEXT_JMPA_M,  M_XCC,  M_CADR),
    E2(0xCA, 0x00, CF_F8, TEXT_CALLA,   M_XCC,  M_CADR),
    E2(0xCA, 0x00, CF_F8, TEXT_CALLA_P, M_XCC,  M_CADR),
    E2(0xCA, 0x01, CF_F8, TEXT_CALLA_M, M_XCC,  M_CADR),
    E0(0x85, 0x7A, CF_PD, TEXT_ENWDT),
    E0(0x8C, 0x00, CF_00, TEXT_SBRK),
};

constexpr uint8_t INDEX_XC16X[] PROGMEM = {
      3,  // TEXT_CALLA
      4,  // TEXT_CALLA_P
      5,  // TEXT_CALLA_M
      6,  // TEXT_ENWDT
      0,  // TEXT_JMPA
      1,  // TEXT_JMPA_P
      2,  // TEXT_JMPA_M
      7,  // TEXT_SBRK
};

// The MAC unit.
constexpr Entry TABLE_MAC[] PROGMEM = {
    X3(0x83, 0x00, CF_FF, 0x00, TEXT_COMULU,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x01, TEXT_COMULU,     M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x02, TEXT_COADD,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x08, TEXT_COMULU_M,   M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x0A, TEXT_COSUB,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x10, TEXT_COMACU,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x11, TEXT_COMACU,     M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x12, TEXT_COSUBR,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x20, TEXT_COMACU_M,   M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x22, TEXT_COLOAD,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x2A, TEXT_COLOAD_M,   M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x30, TEXT_COMACRU,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x31, TEXT_COMACRU,    M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x3A, TEXT_COMAX,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x40, TEXT_COMULSU,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x41, TEXT_COMULSU,    M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x42, TEXT_COADD2,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x48, TEXT_COMULSU_M,  M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x4A, TEXT_COSUB2,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x50, TEXT_COMACSU,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x51, TEXT_COMACSU,    M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x52, TEXT_COSUB2R,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x60, TEXT_COMACSU_M,  M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x62, TEXT_COLOAD2,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x6A, TEXT_COLOAD2_M,  M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x70, TEXT_COMACRSU,   M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x71, TEXT_COMACRSU,   M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x7A, TEXT_COMIN,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x80, TEXT_COMULUS,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x81, TEXT_COMULUS,    M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0x88, TEXT_COMULUS_M,  M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_NN, 0x8A, TEXT_COSHL,      M_QIRL,  M_NONE,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x90, TEXT_COMACUS,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0x91, TEXT_COMACUS,    M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_NN, 0x9A, TEXT_COSHR,      M_QIRL,  M_NONE,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xA0, TEXT_COMACUS_M,  M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_NN, 0xAA, TEXT_COASHR,     M_QIRL,  M_NONE,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xB0, TEXT_COMACRUS,   M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xB1, TEXT_COMACRUS,   M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_NN, 0xBA, TEXT_COASHR,     M_QIRL,  M_RND,   M_NONE),
    X3(0x83, 0x00, CF_FF, 0xC0, TEXT_COMUL,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xC1, TEXT_COMUL,      M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0xC2, TEXT_COCMP,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xC8, TEXT_COMUL_M,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xCA, TEXT_COABS,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xD0, TEXT_COMAC,      M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xD1, TEXT_COMAC,      M_RWH,   M_QIRL,  M_RND),
    X3(0x83, 0x00, CF_FF, 0xE0, TEXT_COMAC_M,    M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xF0, TEXT_COMACR,     M_RWH,   M_QIRL,  M_NONE),
    X3(0x83, 0x00, CF_FF, 0xF1, TEXT_COMACR,     M_RWH,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x00, TEXT_COMULU,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x01, TEXT_COMULU,     M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x02, TEXT_COADD,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x08, TEXT_COMULU_M,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x0A, TEXT_COSUB,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x10, TEXT_COMACU,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x11, TEXT_COMACU,     M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x12, TEXT_COSUBR,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x18, TEXT_COMACMU,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x19, TEXT_COMACMU,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x20, TEXT_COMACU_M,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x22, TEXT_COLOAD,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x28, TEXT_COMACMU_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x2A, TEXT_COLOAD_M,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x30, TEXT_COMACRU,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x31, TEXT_COMACRU,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x38, TEXT_COMACMRU,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x39, TEXT_COMACMRU,   M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x3A, TEXT_COMAX,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x40, TEXT_COMULSU,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x41, TEXT_COMULSU,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x42, TEXT_COADD2,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x48, TEXT_COMULSU_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x4A, TEXT_COSUB2,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x50, TEXT_COMACSU,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x51, TEXT_COMACSU,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x52, TEXT_COSUB2R,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x58, TEXT_COMACMSU,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x59, TEXT_COMACMSU,   M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_F0, 0x5A, TEXT_CONOP,      M_QIX,   M_QNOP,  M_NONE),
    X3(0x93, 0x10, CF_0F, 0x5A, TEXT_CONOP,      M_QIRL,  M_NONE,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x5A, TEXT_CONOP,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x60, TEXT_COMACSU_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x62, TEXT_COLOAD2,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x68, TEXT_COMACMSU_M, M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x6A, TEXT_COLOAD2_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x70, TEXT_COMACRSU,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x71, TEXT_COMACRSU,   M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x78, TEXT_COMACMRSU,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x79, TEXT_COMACMRSU,  M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x7A, TEXT_COMIN,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x80, TEXT_COMULUS,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x81, TEXT_COMULUS,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x88, TEXT_COMULUS_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x90, TEXT_COMACUS,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x91, TEXT_COMACUS,    M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0x98, TEXT_COMACMUS,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0x99, TEXT_COMACMUS,   M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xA0, TEXT_COMACUS_M,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xA8, TEXT_COMACMUS_M, M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xB0, TEXT_COMACRUS,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xB1, TEXT_COMACRUS,   M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xB8, TEXT_COMACMRUS,  M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xB9, TEXT_COMACMRUS,  M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xC0, TEXT_COMUL,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xC1, TEXT_COMUL,      M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xC2, TEXT_COCMP,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xC8, TEXT_COMUL_M,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xCA, TEXT_COABS,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xD0, TEXT_COMAC,      M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xD1, TEXT_COMAC,      M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xD8, TEXT_COMACM,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xD9, TEXT_COMACM,     M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xE0, TEXT_COMAC_M,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xE8, TEXT_COMACM_M,   M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xF0, TEXT_COMACR,     M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xF1, TEXT_COMACR,     M_QIX,   M_QIRL,  M_RND),
    X3(0x93, 0x00, CF_FF, 0xF8, TEXT_COMACMR,    M_QIX,   M_QIRL,  M_NONE),
    X3(0x93, 0x00, CF_FF, 0xF9, TEXT_COMACMR,    M_QIX,   M_QIRL,  M_RND),
    X3(0xA3, 0x00, CF_FF, 0x00, TEXT_COMULU,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x01, TEXT_COMULU,     M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0x02, TEXT_COADD,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x08, TEXT_COMULU_M,   M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x0A, TEXT_COSUB,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x10, TEXT_COMACU,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x11, TEXT_COMACU,     M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0x12, TEXT_COSUBR,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_00, 0x1A, TEXT_COABS,      M_NONE,  M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x20, TEXT_COMACU_M,   M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x22, TEXT_COLOAD,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x2A, TEXT_COLOAD_M,   M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x30, TEXT_COMACRU,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x31, TEXT_COMACRU,    M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_00, 0x32, TEXT_CONEG,      M_NONE,  M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x3A, TEXT_COMAX,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x40, TEXT_COMULSU,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x41, TEXT_COMULSU,    M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0x42, TEXT_COADD2,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x48, TEXT_COMULSU_M,  M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x4A, TEXT_COSUB2,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x50, TEXT_COMACSU,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x51, TEXT_COMACSU,    M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0x52, TEXT_COSUB2R,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x60, TEXT_COMACSU_M,  M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x62, TEXT_COLOAD2,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x6A, TEXT_COLOAD2_M,  M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x70, TEXT_COMACRSU,   M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x71, TEXT_COMACRSU,   M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_00, 0x72, TEXT_CONEG,      M_RND,   M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x7A, TEXT_COMIN,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x80, TEXT_COMULUS,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x81, TEXT_COMULUS,    M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_00, 0x82, TEXT_COSHL,      M_D5,    M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x88, TEXT_COMULUS_M,  M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_NN, 0x8A, TEXT_COSHL,      M_RWH,   M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x90, TEXT_COMACUS,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0x91, TEXT_COMACUS,    M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_00, 0x92, TEXT_COSHR,      M_D5,    M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_NN, 0x9A, TEXT_COSHR,      M_RWH,   M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xA0, TEXT_COMACUS_M,  M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_00, 0xA2, TEXT_COASHR,     M_D5,    M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_NN, 0xAA, TEXT_COASHR,     M_RWH,   M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xB0, TEXT_COMACRUS,   M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xB1, TEXT_COMACRUS,   M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_00, 0xB2, TEXT_COASHR,     M_D5,    M_RND,   M_NONE),
    X3(0xA3, 0x00, CF_00, 0xB2, TEXT_CORND,      M_NONE,  M_NONE,  M_NONE),
    X3(0xA3, 0x00, CF_NN, 0xBA, TEXT_COASHR,     M_RWH,   M_RND,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xC0, TEXT_COMUL,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xC1, TEXT_COMUL,      M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0xC2, TEXT_COCMP,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xC8, TEXT_COMUL_M,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xCA, TEXT_COABS,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xD0, TEXT_COMAC,      M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xD1, TEXT_COMAC,      M_RWH,   M_RWL,   M_RND),
    X3(0xA3, 0x00, CF_FF, 0xE0, TEXT_COMAC_M,    M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xF0, TEXT_COMACR,     M_RWH,   M_RWL,   M_NONE),
    X3(0xA3, 0x00, CF_FF, 0xF1, TEXT_COMACR,     M_RWH,   M_RWL,   M_RND),
    S3(0xB3, 0x00, CF_NN,       TEXT_COSTORE,    M_QIRL,  M_COREG, M_NONE),
    S3(0xC3, 0x00, CF_NN,       TEXT_COSTORE,    M_RWH,   M_COREG, M_NONE),
    X3(0xD3, 0x00, CF_FF, 0x00, TEXT_COMOV,      M_QIX,   M_QIRL,  M_NONE),
};

constexpr uint8_t INDEX_MAC[] PROGMEM = {
     44,  // TEXT_COABS
    108,  // TEXT_COABS
    127,  // TEXT_COABS
    171,  // TEXT_COABS
      2,  // TEXT_COADD
     52,  // TEXT_COADD
    121,  // TEXT_COADD
     16,  // TEXT_COADD2
     71,  // TEXT_COADD2
    137,  // TEXT_COADD2
     36,  // TEXT_COASHR
     39,  // TEXT_COASHR
    160,  // TEXT_COASHR
    161,  // TEXT_COASHR
    164,  // TEXT_COASHR
    166,  // TEXT_COASHR
     42,  // TEXT_COCMP
    106,  // TEXT_COCMP
    169,  // TEXT_COCMP
      9,  // TEXT_COLOAD
     61,  // TEXT_COLOAD
    129,  // TEXT_COLOAD
     10,  // TEXT_COLOAD_M
     63,  // TEXT_COLOAD_M
    130,  // TEXT_COLOAD_M
     23,  // TEXT_COLOAD2
     83,  // TEXT_COLOAD2
    144,  // TEXT_COLOAD2
     24,  // TEXT_COLOAD2_M
     85,  // TEXT_COLOAD2_M
    145,  // TEXT_COLOAD2_M
     45,  // TEXT_COMAC
     46,  // TEXT_COMAC
    109,  // TEXT_COMAC
    110,  // TEXT_COMAC
    172,  // TEXT_COMAC
    173,  // TEXT_COMAC
     47,  // TEXT_COMAC_M
    113,  // TEXT_COMAC_M
    174,  // TEXT_COMAC_M
    111,  // TEXT_COMACM
    112,  // TEXT_COMACM
    114,  // TEXT_COMACM_M
    117,  // TEXT_COMACMR
    118,  // TEXT_COMACMR
     88,  // TEXT_COMACMRSU
     89,  // TEXT_COMACMRSU
     66,  // TEXT_COMACMRU
     67,  // TEXT_COMACMRU
    102,  // TEXT_COMACMRUS
    103,  // TEXT_COMACMRUS
     77,  // TEXT_COMACMSU
     78,  // TEXT_COMACMSU
     84,  // TEXT_COMACMSU_M
     58,  // TEXT_COMACMU
     59,  // TEXT_COMACMU
     62,  // TEXT_COMACMU_M
     96,  // TEXT_COMACMUS
     97,  // TEXT_COMACMUS
     99,  // TEXT_COMACMUS_M
     48,  // TEXT_COMACR
     49,  // TEXT_COMACR
    115,  // TEXT_COMACR
    116,  // TEXT_COMACR
    175,  // TEXT_COMACR
    176,  // TEXT_COMACR
     25,  // TEXT_COMACRSU
     26,  // TEXT_COMACRSU
     86,  // TEXT_COMACRSU
     87,  // TEXT_COMACRSU
    146,  // TEXT_COMACRSU
    147,  // TEXT_COMACRSU
     11,  // TEXT_COMACRU
     12,  // TEXT_COMACRU
     64,  // TEXT_COMACRU
     65,  // TEXT_COMACRU
    131,  // TEXT_COMACRU
    132,  // TEXT_COMACRU
     37,  // TEXT_COMACRUS
     38,  // TEXT_COMACRUS
    100,  // TEXT_COMACRUS
    101,  // TEXT_COMACRUS
    162,  // TEXT_COMACRUS
    163,  // TEXT_COMACRUS
     19,  // TEXT_COMACSU
     20,  // TEXT_COMACSU
     74,  // TEXT_COMACSU
     75,  // TEXT_COMACSU
    140,  // TEXT_COMACSU
    141,  // TEXT_COMACSU
     22,  // TEXT_COMACSU_M
     82,  // TEXT_COMACSU_M
    143,  // TEXT_COMACSU_M
      5,  // TEXT_COMACU
      6,  // TEXT_COMACU
     55,  // TEXT_COMACU
     56,  // TEXT_COMACU
    124,  // TEXT_COMACU
    125,  // TEXT_COMACU
      8,  // TEXT_COMACU_M
     60,  // TEXT_COMACU_M
    128,  // TEXT_COMACU_M
     32,  // TEXT_COMACUS
     33,  // TEXT_COMACUS
     94,  // TEXT_COMACUS
     95,  // TEXT_COMACUS
    155,  // TEXT_COMACUS
    156,  // TEXT_COMACUS
     35,  // TEXT_COMACUS_M
     98,  // TEXT_COMACUS_M
    159,  // TEXT_COMACUS_M
     13,  // TEXT_COMAX
     68,  // TEXT_COMAX
    134,  // TEXT_COMAX
     27,  // TEXT_COMIN
     90,  // TEXT_COMIN
    149,  // TEXT_COMIN
    179,  // TEXT_COMOV
     40,  // TEXT_COMUL
     41,  // TEXT_COMUL
    104,  // TEXT_COMUL
    105,  // TEXT_COMUL
    167,  // TEXT_COMUL
    168,  // TEXT_COMUL
     43,  // TEXT_COMUL_M
    107,  // TEXT_COMUL_M
    170,  // TEXT_COMUL_M
     14,  // TEXT_COMULSU
     15,  // TEXT_COMULSU
     69,  // TEXT_COMULSU
     70,  // TEXT_COMULSU
    135,  // TEXT_COMULSU
    136,  // TEXT_COMULSU
     17,  // TEXT_COMULSU_M
     72,  // TEXT_COMULSU_M
    138,  // TEXT_COMULSU_M
      0,  // TEXT_COMULU
      1,  // TEXT_COMULU
     50,  // TEXT_COMULU
     51,  // TEXT_COMULU
    119,  // TEXT_COMULU
    120,  // TEXT_COMULU
      3,  // TEXT_COMULU_M
     53,  // TEXT_COMULU_M
    122,  // TEXT_COMULU_M
     28,  // TEXT_COMULUS
     29,  // TEXT_COMULUS
     91,  // TEXT_COMULUS
     92,  // TEXT_COMULUS
    150,  // TEXT_COMULUS
    151,  // TEXT_COMULUS
     30,  // TEXT_COMULUS_M
     93,  // TEXT_COMULUS_M
    153,  // TEXT_COMULUS_M
    133,  // TEXT_CONEG
    148,  // TEXT_CONEG
     79,  // TEXT_CONOP
     80,  // TEXT_CONOP
     81,  // TEXT_CONOP
    165,  // TEXT_CORND
     31,  // TEXT_COSHL
    152,  // TEXT_COSHL
    154,  // TEXT_COSHL
     34,  // TEXT_COSHR
    157,  // TEXT_COSHR
    158,  // TEXT_COSHR
    177,  // TEXT_COSTORE
    178,  // TEXT_COSTORE
      4,  // TEXT_COSUB
     54,  // TEXT_COSUB
    123,  // TEXT_COSUB
     18,  // TEXT_COSUB2
     73,  // TEXT_COSUB2
    139,  // TEXT_COSUB2
     21,  // TEXT_COSUB2R
     76,  // TEXT_COSUB2R
    142,  // TEXT_COSUB2R
      7,  // TEXT_COSUBR
     57,  // TEXT_COSUBR
    126,  // TEXT_COSUBR
};
// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage C166_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_C166), ARRAY_RANGE(INDEX_C166)},
};

constexpr EntryPage C167_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_C166), ARRAY_RANGE(INDEX_C166)},
        {ARRAY_RANGE(TABLE_C167), ARRAY_RANGE(INDEX_C167)},
};

constexpr EntryPage XC16X_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_XC16X), ARRAY_RANGE(INDEX_XC16X)},
        {ARRAY_RANGE(TABLE_MAC), ARRAY_RANGE(INDEX_MAC)},
        {ARRAY_RANGE(TABLE_C166), ARRAY_RANGE(INDEX_C166)},
        {ARRAY_RANGE(TABLE_C167), ARRAY_RANGE(INDEX_C167)},
};

// The XC16X without the MAC unit, which GNU as does not know.
constexpr EntryPage XC16X_NOMAC_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_XC16X), ARRAY_RANGE(INDEX_XC16X)},
        {ARRAY_RANGE(TABLE_C166), ARRAY_RANGE(INDEX_C166)},
        {ARRAY_RANGE(TABLE_C167), ARRAY_RANGE(INDEX_C167)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {C166, TEXT_CPU_80C166, ARRAY_RANGE(C166_PAGES)},
        {C167, TEXT_CPU_80C167, ARRAY_RANGE(C167_PAGES)},
        {XC16X, TEXT_CPU_XC16X, ARRAY_RANGE(XC16X_PAGES)},
};

constexpr Cpu XC16X_NOMAC PROGMEM = {XC16X, TEXT_CPU_XC16X, ARRAY_RANGE(XC16X_NOMAC_PAGES)};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

namespace {

bool fitsImm(const Operand &op, uint32_t max, int32_t min = 0) {
    return !op.val.isUndefined() && !op.val.overflow(max, min);
}

bool acceptImm(const Operand &op, AddrMode table) {
    switch (table) {
    case M_D3:
        return fitsImm(op, 7);
    case M_D4:
        return fitsImm(op, 15);
    case M_D5:
        return fitsImm(op, 16);
    case M_TRAP:
    case M_IM16:
    case M_IM8:
    case M_RNG:
    case M_RNGR:
    case M_PAG:
    case M_SEG8:
        return true;
    default:
        return false;
    }
}

bool acceptAddr(const AsmInsn &insn, const Operand &op, AddrMode table) {
    switch (table) {
    case M_REGW:
    case M_REGB:
        return op.val.isUndefined() || shortReg(insn, op.val.getUnsigned()) >= 0;
    case M_MEM:
    case M_CADR:
    case M_REL:
    case M_RELW:
    case M_BOFF:
    case M_SEG:
        return true;
    default:
        return false;
    }
}

bool acceptMode(const AsmInsn &insn, const Operand &op, AddrMode table) {
    if (op.mode == table)
        return true;
    switch (op.mode) {
    case M_RW:
        return table == M_RWH || table == M_RWL || table == M_REGW || table == M_BOFF;
    case M_RB:
        return table == M_RBH || table == M_RBL || table == M_REGB;
    case M_IR:
        return table == M_IRH || table == M_IRL || table == M_QIRL ||
               (table == M_IR2 && encodeRegNum(op.reg) < 4);
    case M_PI:
        return table == M_PIH || table == M_PIL || table == M_QIRL ||
               (table == M_PI2 && encodeRegNum(op.reg) < 4);
    case M_IMM:
        return acceptImm(op, table);
    case M_CC:
        return table == M_CCO || table == M_XCC;
    case M_NONE:
        return table == M_QNOP;
    case M_BIT:
        return table == M_BITO || table == M_BITQ || table == M_BITZ;
    case M_ADDR:
        return acceptAddr(insn, op, table);
    default:
        return false;
    }
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    if (!acceptMode(insn, insn.dstOp, table.dst()))
        return false;
    const auto src = table.src();
    if (src == M_MDL || src == M_MDH)
        return insn.srcOp.mode == M_IMM && insn.extOp.mode == M_IMM;
    return acceptMode(insn, insn.srcOp, src) && acceptMode(insn, insn.extOp, table.ext());
}

void readCode(AsmInsn &insn, const Entry *entry, const EntryPage *) {
    insn.setOpCode(entry->readOpCode());
    insn.setFlags(entry->readFlags());
    insn.setPost(entry->readPost());
}

}  // namespace

int16_t shortReg(const AsmInsn &insn, uint32_t addr) {
    const uint32_t base = insn.esfr() ? 0xF000 : 0xFE00;
    if (addr < base || addr > base + 2 * 0xEF || (addr & 1))
        return -1;
    return (addr - base) / 2;
}

int16_t bitOff(const AsmInsn &insn, uint32_t addr) {
    if (addr >= 0xFD00 && addr <= 0xFDFE && (addr & 1) == 0)
        return (addr - 0xFD00) / 2;
    const uint32_t base = insn.esfr() ? 0xF100 : 0xFF00;
    if (addr >= base && addr <= base + 2 * (0xEF - 0x80) && (addr & 1) == 0)
        return 0x80 + (addr - base) / 2;
    return -1;
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    cpu(cpuType)->searchName(insn, acceptModes, Cpu::defaultPageSetup, readCode);
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    if ((insn.opCode() & ~flags.opCodeMask()) != entry->readOpCode())
        return false;
    const auto post = insn.post();
    if (flags.cf() == CF_NN && (post >> 4) != (post & 0xF))
        return false;
    if ((post & ~flags.postMask()) != entry->readPost())
        return false;
    // No CoXXX instruction has an operand a prefix changes, so as a follower
    // any of them will do, without asking for its second word.
    if (flags.rrr() && insn.follower())
        return true;
    if (flags.hasSub()) {
        insn.readByte3();
        insn.readByte4();
        if (insn.byte3() != flags.sub())
            return false;
    }
    if (flags.src() == M_QNOP)
        return (insn.byte4() & 7) == 1;
    return true;
}

Error searchOpCode(CpuType cpuType, bool mac, DisInsn &insn, StrBuffer &out) {
    const auto *c = (cpuType == XC16X && !mac) ? &XC16X_NOMAC : cpu(cpuType);
    c->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableC166::listCpu_P() const {
    return TEXT_C166_LIST;
}

const /*PROGMEM*/ char *TableC166::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableC166::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    if (name.iequals_P(TEXT_CPU_80C167CS)) {
        cpuType = C167;
        return OK;
    }
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
        return OK;
    }
    return UNSUPPORTED_CPU;
}

const TableC166 TABLE;

}  // namespace c166
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
