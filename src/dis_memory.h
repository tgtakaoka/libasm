/*
 * Copyright 2020 Tadashi G. Takaoka
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

#ifndef __DIS_MEMORY_H__
#define __DIS_MEMORY_H__

namespace libasm {

template<typename Addr>
class DisMemory {
public:
    virtual bool hasNext() const = 0;
    Addr address() const { return _address; }
    uint8_t readByte() {
        const uint8_t val = nextByte();
        _address++;
        return val;
    }

protected:
    Addr _address;

    DisMemory(Addr address) : _address(address) {}
    virtual uint8_t nextByte() = 0;
};

} // namespace libasm

#endif

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
