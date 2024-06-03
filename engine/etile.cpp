#include "etile.h"

#include <random>
#include <algorithm>

#include "buildings/ebuilding.h"
#include "buildings/ebuildingrenderer.h"
#include "characters/echaracter.h"

#include "evectorhelpers.h"

eTile::eTile(const int x, const int y,
             const int dx, const int dy) {
    setSeed(rand());
    setX(x);
    setY(y);
    setDX(dx);
    setDY(dy);
}

int eTile::scrubId(const int nLevels) const {
    const double div = 1./nLevels;
    const int i = std::round(scrub()/div);
    return std::clamp(i, 0, nLevels);
}

std::vector<eTile*> eTile::surroundingRoads() const {
    std::vector<eTile*> tiles;
    for(int i = 0; i < 8; i += 2) {
        const auto o = static_cast<eOrientation>(i);
        const auto n = neighbour(o);
        const auto nt = static_cast<eTile*>(n);
        if(n && nt->hasRoad()) tiles.push_back(nt);
    }
    return tiles;
}

eTile* eTile::nearestRoad() const {
    const auto tiles = surroundingRoads();
    if(tiles.empty()) return nullptr;
    return tiles[rand() % tiles.size()];
}

void eTile::surroundingTerrain(eTerrain& tlTerr,
                               eTerrain& trTerr,
                               eTerrain& brTerr,
                               eTerrain& blTerr,
                               eTerrain& tTerr,
                               eTerrain& rTerr,
                               eTerrain& bTerr,
                               eTerrain& lTerr) const {
    tlTerr = terrain();
    trTerr = tlTerr;
    brTerr = tlTerr;
    blTerr = tlTerr;

    tTerr = tlTerr;
    rTerr = tlTerr;
    bTerr = tlTerr;
    lTerr = tlTerr;

    {
        const auto tl = topLeft();
        if(tl) {
            tlTerr = tl->terrain();
        }
    }
    {
        const auto tr = topRight();
        if(tr) {
            trTerr = tr->terrain();
        }
    }
    {
        const auto br = bottomRight();
        if(br) {
            brTerr = br->terrain();
        }
    }
    {
        const auto bl = bottomLeft();
        if(bl) {
            blTerr = bl->terrain();
        }
    }

    {
        const auto t = top();
        if(t) {
            tTerr = t->terrain();
        }
    }

    {
        const auto r = right();
        if(r) {
            rTerr = r->terrain();
        }
    }

    {
        const auto b = bottom();
        if(b) {
            bTerr = b->terrain();
        }
    }

    {
        const auto l = left();
        if(l) {
            lTerr = l->terrain();
        }
    }
}

void eTile::neighboursWithTerrain(const eTerrain terr,
                                  bool& tl, bool& tr,
                                  bool& br, bool& bl,
                                  bool& t, bool& r,
                                  bool& b, bool& l) const {
    eTerrain tlTerr;
    eTerrain trTerr;
    eTerrain brTerr;
    eTerrain blTerr;

    eTerrain tTerr;
    eTerrain rTerr;
    eTerrain bTerr;
    eTerrain lTerr;

    surroundingTerrain(tlTerr, trTerr, brTerr, blTerr,
                       tTerr, rTerr, bTerr, lTerr);

    tl = tlTerr != terr;
    tr = trTerr != terr;
    br = brTerr != terr;
    bl = blTerr != terr;

    t = tTerr != terr;
    r = rTerr != terr;
    b = bTerr != terr;
    l = lTerr != terr;
}

bool eTile::onFire() const {
    const auto ub = underBuilding();
    if(!ub) return false;
    return ub->isOnFire();
}

void eTile::setOnFire(const bool f) {
    const auto ub = underBuilding();
    if(!ub) return;
    ub->setOnFire(f);
}

void eTile::setTerrain(const eTerrain terr) {
    eTileBase::setTerrain(terr);
    scheduleTerrainUpdate();
}

void eTile::setMarbleLevel(const int l) {
    eTileBase::setMarbleLevel(l);
    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
            const auto t = tileRel<eTile>(dx, dy);
            if(!t) continue;
            scheduleTerrainUpdate();
        }
    }
}

void eTile::read(eReadStream& src) {
    eTileBase::read(src);
    src >> mUpdateTerrain;
}

void eTile::write(eWriteStream& dst) const {
    eTileBase::write(dst);
    dst << mUpdateTerrain;
}

void eTile::addCharacter(const stdsptr<eCharacter>& c,
                         const bool prepend) {
    if(prepend) {
        mCharacters.insert(mCharacters.begin(), c);
    } else {
        mCharacters.push_back(c);
    }
}

bool eTile::removeCharacter(const stdsptr<eCharacter>& c) {
    return eVectorHelpers::remove(mCharacters, c);
}

void eTile::addMissile(const stdsptr<eMissile>& m) {
    mMissiles.push_back(m);
}

bool eTile::removeMissile(const stdsptr<eMissile>& m) {
    return eVectorHelpers::remove(mMissiles, m);
}

bool eTile::hasCharacter(const eHasChar& func) const {
    for(const auto& c : mCharacters) {
        if(func(*c)) return true;
    }
    return false;
}

bool eTile::hasRoad() const {
    if(!mUnderBuilding) return false;
    const auto t = mUnderBuilding->type();
    return t == eBuildingType::road;
}

void eTile::setUnderBuilding(const stdsptr<eBuilding>& b) {
    mUnderBuilding = b;
    scheduleTerrainUpdate();
}

eBuildingType eTile::underBuildingType() const {
    if(!mUnderBuilding) return eBuildingType::none;
    else return mUnderBuilding->type();
}

void eTile::setBanner(eBanner* const s) {
    mBanner = s;
}

void eTile::setSoldierBanner(eSoldierBanner* const b) {
    mSoldierBanner = b;
}

void eTile::setFutureDimension(const int futureDim) {
    mTerrainPainter.fFutureDim = futureDim;
}
