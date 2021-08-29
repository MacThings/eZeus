#ifndef ETERRAIN_H
#define ETERRAIN_H


enum class eTerrain {
    dry = 1 << 0,
    beach = 1 << 1,
    water = 1 << 2,
    fertile = 1 << 3,
    forest = 1 << 4,
    choppedForest = 1 << 5,

    flatStones = 1 << 6,
    copper = 1 << 7,
    silver = 1 << 8,
    tallStones = 1 << 9,
    tinyStones = 1 << 10,

    dryBased = dry |
               fertile |
               forest |
               choppedForest |

               flatStones |
               copper |
               silver |
               tallStones |
               tinyStones,

    buildable = dry | beach | fertile,
    walkable = dry | beach | fertile |
               forest | choppedForest |
               tinyStones
};

inline eTerrain operator|(const eTerrain a, const eTerrain b) {
    return static_cast<eTerrain>(static_cast<int>(a) | static_cast<int>(b));
}

inline eTerrain operator&(const eTerrain a, const eTerrain b) {
    return static_cast<eTerrain>(static_cast<int>(a) & static_cast<int>(b));
}

namespace eTerrainTools {
    double speed(const eTerrain t);
}

#endif // ETERRAIN_H
