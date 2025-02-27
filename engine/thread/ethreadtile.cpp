#include "ethreadtile.h"

#include "characters/echaracter.h"

void eThreadTile::load(eTile* const src) {
    setSeed(src->seed());
    setX(src->x());
    setY(src->y());
    setDX(src->dx());
    setDY(src->dy());
    setWalkableElev(src->walkableElev());
    setElevation(src->isElevationTile());
    setBusy(src->busy());
    setResource(src->resource());
    setTerrain(src->terrain());
    setScrub(src->scrub());
    setAltitude(src->altitude(), false);
    setOnFire(src->onFire());
    setHasFish(src->hasFish());
    setHasUrchin(src->hasUrchin());
    mBannerType = src->bannerType();
    setCityId(src->cityId());

    mCharacters.clear();
    const auto& chars = src->characters();
    for(const auto& ch : chars) {
        mCharacters.emplace_back().load(ch.get());
    }
    mUnderBuilding.load(src->underBuilding());
}

bool eThreadTile::hasRoad() const {
    return mUnderBuilding.type() == eBuildingType::road;
}

bool eThreadTile::hasCharacter(const eHasChar& func) const {
    for(const auto& c : mCharacters) {
        if(func(c)) return true;
    }
    return false;
}

eBuildingType eThreadTile::underBuildingType() const {
    return mUnderBuilding.type();
}

bool eThreadTile::isUnderBuilding() const {
    return mUnderBuilding.type() != eBuildingType::none;
}

int eThreadTile::houseVacancies() const {
    return mUnderBuilding.houseVacancies();
}
