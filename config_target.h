#ifndef __CONFIG_TARGET_H__
#define __CONFIG_TARGET_H__

#include <cstdint>

namespace target
{
    typedef std::uint8_t  byte_t;
    typedef std::uint16_t word_t;
    typedef std::uint32_t dword_t;
    typedef std::int16_t  sword_t;
    typedef std::int32_t  sdword_t;
    typedef std::uint16_t uintptr_t;
    typedef std::int16_t  intptr_t;
    typedef std::int16_t  ptrdiff_t;
} // namespace target

#endif /* __CONFIG_TARGET_H__ */
