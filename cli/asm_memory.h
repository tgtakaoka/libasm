/* -*- mode: c++; -*- */
#ifndef __ASM_MEMORY_H__
#define __ASM_MEMORY_H__

#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>

#include "bin_formatter.h"

template<typename Addr>
class AsmMemory {
public:
    AsmMemory() {
        invalidateCache();
    }

    void writeBytes(Addr addr, const uint8_t *p, unsigned size) {
        for (const uint8_t *end = p + size; p < end; p++)
            writeByte(addr++, *p);
    }

    bool readBytes(Addr addr, uint8_t *p, unsigned size) {
        for (uint8_t *end = p + size; p < end; p++) {
            if (!readByte(addr, p))
                return false;
        }
        return true;
    }

    void writeByte(Addr addr, uint8_t val) {
        // Shortcut for most possible case, appending byte to cached segment.
        if (atEndOf(_cache, addr)) {
            appendTo(_cache, addr, val);
            return;
        }
        if (insideOf(_cache, addr)) {
            replaceAt(_cache, addr, val);
            return;
        }

        invalidateCache();
        for (auto segment = _segments.begin(); segment != _segments.end(); segment++) {
            if (atEndOf(segment, addr)) {
                appendTo(segment, addr, val);
                return;
            }
            if (insideOf(segment, addr)) {
                replaceAt(segment, addr, val);
                return;
            }
        }

        // No appropriate segment found, create a segment.
        createSegment(addr, val);
    }

    bool readByte(Addr addr, uint8_t &val) {
        if (insideOf(_cache, addr)) {
            val = readFrom(_cache, addr);
            return true;
        }
        for (auto segment = _segments.begin(); segment != _segments.end(); segment++) {
            if (insideOf(segment, addr)) {
                val = readFrom(segment, addr);
                _cache = segment;
                return true;
            }
        }
        return false;
    }

    bool equals(const AsmMemory<Addr> &other) const {
        auto a = _segments.cbegin();
        auto b = other._segments.cbegin();
        while (a != _segments.cend() && b != other._segments.cend()) {
            if (a->first != b->first)
                return false;
            if (a->second.size() != b->second.size())
                return false;
            for (std::size_t i = 0; i < a->second.size(); i++) {
                if (a->second[i] != b->second[i])
                    return false;
            }
            a++;
            b++;
        }
        return a == _segments.cend() && b == other._segments.cend();
    }

    void swap(AsmMemory& other) {
        _segments.swap(other._segments);
        invalidateCache();
        other.invalidateCache();
    }

    // Dumper should accept (const char *).
    template<typename Dumper>
    void dump(
        BinFormatter<Addr> *formatter, Dumper dumper) const {
        const char *header = formatter->start();
        if (header) dumper(header);
        for (auto segment = _segments.cbegin();
             segment != _segments.cend(); segment++) {
            const auto &mem = segment->second;
            for (std::size_t i = 0; i < mem.size(); i += 16) {
                auto size = (i + 16 < mem.size()) ? 16 : mem.size() - i;
                const char *line = formatter->dump(
                    segment->first + i, mem.data() + i, size);
                dumper(line);
            }
        }
        const char *trailer = formatter->end();
        if (trailer) dumper(trailer);
    }

private:
    // Memory segment list in ascending order of start address.
    typedef std::map<Addr, std::vector<uint8_t>> Segments;
    typedef typename Segments::iterator Segment;
    Segments _segments;
    Segment _cache;

    void invalidateCache() {
        _cache = _segments.end();
    }

    bool insideOf(Segment &segment, Addr addr) const {
        return segment != _segments.end()
            && addr >= segment->first
            && addr < segment->first + segment->second.size();
    }

    bool atEndOf(Segment &segment, Addr addr) const {
        return segment != _segments.end()
            && addr == segment->first + segment->second.size();
    }

    uint8_t readFrom(Segment &segment, Addr addr) const {
        return segment->second.at(addr - segment->first);
    }

    void appendTo(Segment segment, Addr addr, uint8_t val) {
        segment->second.push_back(val);
        _cache = segment;
        aggregate(segment);
    }

    void replaceAt(Segment segment, Addr addr, uint8_t val) {
        segment->second.at(addr - segment->first) = val;
        _cache = segment;
        aggregate(segment);
    }

    void createSegment(Addr addr, uint8_t val) {
        _cache = _segments.insert(std::make_pair(addr, std::vector<uint8_t>())).first;
        _cache->second.push_back(val);
        aggregate(_cache);
    }

    void aggregate(Segment hint) {
        Segment prev = hint;
        // Check following segment whether it is adjacent to.
        for (Segment next = ++hint;
             next != _segments.end() && atEndOf(prev, next->first);
             next = _segments.erase(next)) {
            // Append next segment.
            prev->second.reserve(prev->second.size() + next->second.size());
            prev->second.insert(
                prev->second.end(),
                std::make_move_iterator(next->second.begin()),
                std::make_move_iterator(next->second.end()));
            invalidateCache();
        }
    }
};

#endif
