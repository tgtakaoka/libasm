/*
 * Copyright 2023 Tadashi G. Takaoka
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

#ifndef __STACK_H__
#define __STACK_H__

#include <stdint.h>

namespace libasm {

template <typename ELEM, const uint8_t CAPACITY>
struct Stack {
    Stack() : _size(0) {}

    uint8_t size() const { return _size; }
    bool empty() const { return _size == 0; }
    bool full() const { return _size >= CAPACITY; }

    void push(const ELEM elem) {
        if (_size < CAPACITY)
            _contents[_size++] = elem;
    }
    ELEM pop() { return _size ? _contents[--_size] : ELEM(); }

protected:
    uint8_t _size;
    ELEM _contents[CAPACITY];
};

}  // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
