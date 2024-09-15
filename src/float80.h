/*
 * Copyright 2024 Tadashi G. Takaoka
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

#ifndef __LIBASM_FLOAT80_H__
#define __LIBASM_FLOAT80_H__

#include <float.h>
#include "float80_hard.h"
#include "float80_soft.h"

namespace libasm {
#if defined(LIBASM_HAS_LONG_DOUBLE) && !defined(LIBASM_SOFT_FLOAT80)
using float80_t = __float80_hard;
#else
using float80_t = __float80_soft;
#endif
}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
