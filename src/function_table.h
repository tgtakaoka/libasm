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

#ifndef __LIBASM_FUNCTION_TABLE_H__
#define __LIBASM_FUNCTION_TABLE_H__

#include "operators.h"
#include "str_scanner.h"

namespace libasm {

/**
 * Pre-defined function table.
 */
struct FunctionTable {
    /**
     * Look up |name| and returns a Functor pointer for a function, otherwise return nullptr.
     */
    virtual const Functor *lookupFunction(const StrScanner &) const { return nullptr; }
};

struct RcaFunctionTable final : FunctionTable, Singleton<RcaFunctionTable> {
    /** A(), A.0(), A.1() */
    const Functor *lookupFunction(const StrScanner &name) const override;
};

struct Ins8060FunctionTable final : FunctionTable, Singleton<Ins8060FunctionTable> {
    /** H(), L(), ADDR() */
    const Functor *lookupFunction(const StrScanner &) const override;
};

struct Ins8070FunctionTable final : FunctionTable, Singleton<Ins8070FunctionTable> {
    /** H(), L(), ADDR() */
    const Functor *lookupFunction(const StrScanner &) const override;
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
