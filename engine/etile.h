﻿#ifndef ETILE_H
#define ETILE_H

#include <vector>
#include <functional>

#include "etilebase.h"
#include "eterrain.h"
#include "eorientation.h"

#include "pointers/estdselfref.h"

enum class eTileSize : int {
    s15, s30, s45, s60
};

class eMissile;
class eCharacter;
class eBuilding;
class eBuildingRenderer;
class eSpawner;
class eSoldierBanner;

class eTile : public eTileBase {
public:
    eTile(const int x, const int y);

    int scrubId(const int nLevels) const;

    void addCharacter(const stdsptr<eCharacter>& c);
    bool removeCharacter(const stdsptr<eCharacter>& c);
    bool hasCharacter(const eHasChar& func) const;

    const std::vector<stdsptr<eCharacter>>& characters() const
    { return mCharacters; }

    void addMissile(const stdsptr<eMissile>& m);
    bool removeMissile(const stdsptr<eMissile>& m);

    const std::vector<stdsptr<eMissile>>& missiles() const
    { return mMissiles; }

    void setBuilding(const stdsptr<eBuildingRenderer>& b);
    eBuildingRenderer* building() const { return mBuilding.get(); }
    bool hasRoad() const;
    void setUnderBuilding(const stdsptr<eBuilding>& b);
    eBuilding* underBuilding() const { return mUnderBuilding.get(); }
    eBuildingType underBuildingType() const;

    void setSpawner(eSpawner* const s);
    eSpawner* spawner() const { return mSpawner; }

    void setBanner(eSoldierBanner* const b);
    eSoldierBanner* banner() const { return mBanner; }

    // used for stones rendering
    void setFutureDimension(const int futureDim);
    int futureDim() const { return mFutureDim; }

    std::vector<eTile*> surroundingRoads() const;
    eTile* nearestRoad() const;

    void surroundingTerrain(eTerrain& tlTerr,
                            eTerrain& trTerr,
                            eTerrain& brTerr,
                            eTerrain& blTerr,
                            eTerrain& tTerr,
                            eTerrain& rTerr,
                            eTerrain& bTerr,
                            eTerrain& lTerr) const;
    void neighboursWithTerrain(const eTerrain terr,
                               bool& tl, bool& tr,
                               bool& br, bool& bl,
                               bool& t, bool& r,
                               bool& b, bool& l) const;

    void addTerrainTile(eTile* const tile) { mTerrainTiles.push_back(tile); }
    std::vector<eTile*>& terrainTiles() { return mTerrainTiles; }
private:
    int mFutureDim = 0;
private:
    std::vector<eTile*> mTerrainTiles;

    std::vector<stdsptr<eMissile>> mMissiles;
    std::vector<stdsptr<eCharacter>> mCharacters;
    stdsptr<eBuilding> mUnderBuilding;
    stdsptr<eBuildingRenderer> mBuilding;
    eSpawner* mSpawner = nullptr;
    eSoldierBanner* mBanner = nullptr;
};

#endif // ETILE_H
