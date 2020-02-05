/* -*- mode: c++; -*- */
#ifndef __LEVENSHTEIN_H__
#define __LEVENSHTEIN_H__

#include <string.h>

template<typename Tcost = int>
class Levenshtein {
public:
    Levenshtein()
        : _deleteCost(1),
          _insertCost(1),
          _replaceCost(1)
    {}

    Tcost distance(const char *source, const char *target) {
        const size_t sourceSize = strlen(source);
        const size_t targetSize = strlen(target);
        if (targetSize < sourceSize)
            return distance(target, source);

        Tcost *distance = new Tcost[sourceSize + 1];
        distance[0] = 0;
        for (size_t s = 1; s <= sourceSize; s++) {
            distance[s] = distance[s-1] + _deleteCost;
        }

        for (size_t t = 1; t <= targetSize; t++) {
            Tcost prev = distance[0];
            distance[0] += _insertCost;
            for (size_t s = 1; s <= sourceSize; s++) {
                const Tcost saved = distance[s];
                if (source[s-1] == target[t-1]) {
                    distance[s] = prev;
                } else {
                    const Tcost del = distance[s-1] + _deleteCost;
                    const Tcost ins = distance[s] + _insertCost;
                    const Tcost repl = prev + _replaceCost;
                    Tcost min = (del < ins) ? del : ins;
                    if (repl < min) min = repl;
                    distance[s] = min;
                }
                prev = saved;
            }
        }

        const Tcost editDistance = distance[sourceSize];
        delete distance;
        return editDistance;
    }

    void setDeleteCost(Tcost deleteCost) { _deleteCost = deleteCost; }
    void setInsertCost(Tcost insertCost) { _insertCost = insertCost; }
    void setReplaceCost(Tcost replaceCost) { _replaceCost = replaceCost; }
private:
    Tcost _deleteCost;
    Tcost _insertCost;
    Tcost _replaceCost;
};

#endif
