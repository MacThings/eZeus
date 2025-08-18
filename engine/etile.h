#ifndef ETILE_H
#define ETILE_H

#include <vector>
#include <functional>

#include "etilebase.h"
#include "eterrain.h"
#include "eorientation.h"

#include "pointers/estdpointer.h"

#include "characters/esoldierbanner.h"

#include "etileterrainpainter.h"

enum class eTileSize : int {
    s15, s30, s45, s60
};

class eMissile;
class eCharacter;
class eBuilding;
class eBuildingRenderer;
class eBanner;
class eSoldierBanner;
enum class eWorldDirection;

struct eTerritoryBorder {
    bool fTR = false;
    bool fR = false;
    bool fBR = false;
    bool fB = false;
    bool fBL = false;
    bool fL = false;
    bool fTL = false;
    bool fT = false;
};

class eTile : public eTileBase {
public:
    eTile(const int x, const int y,
          const int dx, const int dy);

    int scrubId(const int nLevels) const;
    double scrub() const { return mScrub; }
    void setScrub(const double s);
    void incScrub(const double s);

    bool rainforest() const { return mRainforest; }
    void setRainforest(const bool r) { mRainforest = r; }

    bool isHalfSlope() const { return mHalfSlope; }

    void setAltitude(const int a, const bool update = true);
    void setDoubleAltitude(const int da, const bool update = true);
    int doubleAltitude() const { return mDoubleAltitude; }
    int altitude() const { return mDoubleAltitude/2; }

    void addCharacter(const stdsptr<eCharacter>& c,
                      const bool prepend = false);
    bool removeCharacter(const stdsptr<eCharacter>& c);
    bool hasCharacter(const eHasChar& func) const override;

    const std::vector<stdsptr<eCharacter>>& characters() const
    { return mCharacters; }

    void addMissile(const stdsptr<eMissile>& m);
    bool removeMissile(const stdsptr<eMissile>& m);

    const std::vector<stdsptr<eMissile>>& missiles() const
    { return mMissiles; }

    bool hasRoad() const override;
    bool hasAvenue() const;
    void setUnderBuilding(const stdsptr<eBuilding>& b);
    eBuilding* underBuilding() const { return mUnderBuilding.get(); }
    eBuildingType underBuildingType() const override;

    void setBanner(const stdsptr<eBanner>& b);
    eBanner* banner() const { return mBanner.get(); }
    eBannerTypeS bannerType() const;

    void setSoldierBanner(eSoldierBanner* const b);
    eSoldierBanner* soldierBanner() const { return mSoldierBanner; }

    // used for stones rendering
    void setDrawDim(const int drawDim);
    int drawDim() const { return mTerrainPainter.fDrawDim; }

    std::vector<eTile*> surroundingRoads() const;
    eTile* nearestRoad() const;

    void surroundingTerrain(eTerrain& tlTerr,
                            eTerrain& trTerr,
                            eTerrain& brTerr,
                            eTerrain& blTerr,
                            eTerrain& tTerr,
                            eTerrain& rTerr,
                            eTerrain& bTerr,
                            eTerrain& lTerr,
                            const eWorldDirection dir) const;
    void neighboursWithTerrain(const eTerrain terr,
                               bool& tl, bool& tr,
                               bool& br, bool& bl,
                               bool& t, bool& r,
                               bool& b, bool& l,
                               const eWorldDirection dir) const;
    using eTilePtr = eTile*;
    void rotatedNeighbours(eTilePtr& tr,
                           eTilePtr& r,
                           eTilePtr& br,
                           eTilePtr& b,
                           eTilePtr& bl,
                           eTilePtr& l,
                           eTilePtr& tl,
                           eTilePtr& t,
                           const eWorldDirection dir) const;

    static const int sMaxDistanceToBorder = 8;
    int distanceToBorder() const { return mDistanceToBorder; }

    const eTerritoryBorder& territoryBorder() const { return mBorder; }
    void updateTerritoryBorder();

    eTileTerrainPainter& terrainPainter() { return mTerrainPainter; }
    bool updateTerrain() const { return mUpdateTerrain; }
    void scheduleTerrainUpdate() { mUpdateTerrain = true; }
    void scheduleNeighboursTerrainUpdate(const int range = 1);
    void terrainUpdated() { mUpdateTerrain = false; }

    void setUnderTile(eTile* const tile,
                      const int dx = 0,
                      const int dy = 0);
    eTile* underTile() const { return mUnderTile; }
    int underTileDX() const { return mUnderTileDX; }
    int underTileDY() const { return mUnderTileDY; }

    bool hasPrey() const;

    bool tidalWaveZone() const { return mTidalWaveZone; }
    void setTidalWaveZone(const bool z) { mTidalWaveZone = z; }

    bool lavaZone() const { return mLavaZone; }
    void setLavaZone(const bool z) { mLavaZone = z; }

    bool onFire() const override;
    void setOnFire(const bool f) override;

    void setTerrain(const eTerrain terr) override;
    void setMarbleLevel(const int l) override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    void updateIsElevationTile();

    eTileTerrainPainter mTerrainPainter;
    eTerritoryBorder mBorder;
    int mDistanceToBorder = 0;
    bool mUpdateTerrain = false;
    eTile* mUnderTile = nullptr;
    int mUnderTileDX = 0;
    int mUnderTileDY = 0;

    int mDoubleAltitude = 0;
    bool mHalfSlope = false;
    bool mRainforest = false;
    double mScrub = 0;
    bool mTidalWaveZone = false;
    bool mLavaZone = false;

    std::vector<stdsptr<eMissile>> mMissiles;
    std::vector<stdsptr<eCharacter>> mCharacters;
    stdsptr<eBuilding> mUnderBuilding;
    stdsptr<eBanner> mBanner;
    stdptr<eSoldierBanner> mSoldierBanner;
};

#endif // ETILE_H
