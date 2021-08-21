#include "estoragebuilding.h"

int eStorageBuilding::add(const eResourceType type, const int count) {
    int rem = count;
    for(int i = 0; i < 8 && rem > 0; i++) {
        const auto t = mResource[i];
        int& c = mResourceCount[i];
        if(t == type) {
            const int dep = std::min(4 - c, rem);
            rem -= dep;
            c += dep;
        }
    }
    for(int i = 0; i < 8 && rem > 0; i++) {
        auto& t = mResource[i];
        int& c = mResourceCount[i];
        if(t == eResourceType::none) {
            t = type;
            const int dep = std::min(4, rem);
            rem -= dep;
            c += dep;
        }
    }
    return count - rem;
}

int eStorageBuilding::take(const eResourceType type, const int count) {
    int rem = count;
    for(int i = 0; i < 8 && rem > 0; i++) {
        auto& t = mResource[i];
        int& c = mResourceCount[i];
        if(t == type) {
            const int dep = std::min(c, rem);
            rem -= dep;
            c -= dep;
            if(c == 0) {
                t = eResourceType::none;
            }
        }
    }
    return count - rem;
}

int eStorageBuilding::count(const eResourceType type) const {
    return sCount(type, mResourceCount, mResource);
}

int eStorageBuilding::spaceLeft(const eResourceType type) const {
    return sSpaceLeft(type, mResourceCount, mResource);
}

int eStorageBuilding::sCount(const eResourceType type,
                             const int resourceCount[8],
                             const eResourceType resourceType[8]) {
    int result = 0;
    for(int i = 0; i < 8; i++) {
        const auto t = resourceType[i];
        if(t != type) continue;
        result += resourceCount[i];
    }
    return result;
}

int eStorageBuilding::sSpaceLeft(const eResourceType type,
                                 const int resourceCount[8],
                                 const eResourceType resourceType[8]) {

    int space = 0;
    for(int i = 0; i < 8; i++) {
        const int c = resourceCount[i];
        const auto t = resourceType[i];
        if(c == 0) {
            space += 8;
        } else if(t == type) {
            space += 8 - c;
        }
    }
    return space;
}