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
#include "text_c166.h"

using namespace libasm::text::c166;

namespace libasm {
namespace c166 {

#define E2(_opc, _post, _cf, _name, _dst, _src) \
    {_opc, _post, Entry::Flags::create(_cf, _dst, _src), _name}
#define E1(_opc, _post, _cf, _name, _dst) E2(_opc, _post, _cf, _name, _dst, M_NONE)
#define E0(_opc, _post, _cf, _name) E1(_opc, _post, _cf, _name, M_NONE)

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

constexpr uint8_t INDEX_C166[] PROGMEM = {0};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage C166_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_C166), ARRAY_RANGE(INDEX_C166)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {C166, TEXT_CPU_80C166, ARRAY_RANGE(C166_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool matchOpCode(DisInsn &insn, const Entry *entry, const EntryPage *) {
    const auto flags = entry->readFlags();
    if ((insn.opCode() & ~flags.opCodeMask()) != entry->readOpCode())
        return false;
    const auto post = insn.post();
    if (flags.cf() == CF_NN && (post >> 4) != (post & 0xF))
        return false;
    return (post & ~flags.postMask()) == entry->readPost();
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    cpu(cpuType)->searchOpCode(insn, out, matchOpCode);
    return insn.getError();
}

const /*PROGMEM*/ char *TableC166::listCpu_P() const {
    return TEXT_C166_LIST;
}

const /*PROGMEM*/ char *TableC166::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableC166::searchCpuName(StrScanner &name, CpuType &cpuType) const {
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
