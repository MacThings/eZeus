#ifndef ESANCTUARY_H
#define ESANCTUARY_H

#include "../eemployingbuilding.h"

#include "esanctuaryblueprint.h"
#include "esanctbuilding.h"
#include "characters/gods/egod.h"
#include "engine/edate.h"

class eBuildingTextures;

enum class eHelpDenialReason {
    tooSoon,
    noTarget,
    error
};

class eSanctuary : public eEmployingBuilding {
public:
    eSanctuary(eGameBoard& board,
               const eBuildingType type,
               const int sw, const int sh,
               const int maxEmployees,
               const eCityId cid);
    ~eSanctuary();

    void erase() override;

    std::shared_ptr<eTexture>
    getTexture(const eTileSize) const override
    { return nullptr; }

    void timeChanged(const int by) override;

    int spaceLeft(const eResourceType type) const override;
    int add(const eResourceType type, const int count) override;

    std::vector<eCartTask> cartTasks() const override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;

    std::vector<eTile*> warriorTiles() const;
    void addWarriorTile(eTile* const t);
    void addSpecialTile(eTile* const t);

    bool askForHelp(eHelpDenialReason& reason);

    eGod* god() const { return mGod; }

    void registerElement(const stdsptr<eSanctBuilding>& e);

    int progress() const; // 0-100
    bool finished() const;

    eSanctCost cost() const;
    const eSanctCost& stored() const { return mStored; }
    const eSanctCost& used() const { return mUsed; }

    void useResources(const eSanctCost& r);

    int altitude() const { return mAltitude; }
    void setAltitude(const int a) { mAltitude = a; }

    eGodType godType() const;

    eGod* spawnGod();
    void spawnPatrolingGod();

    void buildingProgressed();

    void godComeback();
    bool godAbroad() const { return mGodAbroad; }

    void setConstructionHalted(const bool h);
    bool constructionHalted() const { return mHaltConstruction; }


    static stdsptr<eSanctuary> sCreate(
            const eBuildingType type,
            const int sw, const int sh,
            eGameBoard& board);

    void setRotated(const bool r) { mRotated = r; }
    bool rotated() const { return mRotated; }

    void setSpawnWait(const int w);

    bool sacrificing() const;
private:
    bool mHaltConstruction = false;
    eSanctCost mStored{0, 0, 0};
    eSanctCost mUsed{0, 0, 0};
    int mAltitude = 0;
    bool mRotated = false;

    stdptr<eCartTransporter> mCart;

    std::vector<stdsptr<eSanctBuilding>> mElements;

    stdptr<eGod> mGod;
    bool mGodAbroad = false;
    int mSpawnWait = 0;

    bool mAskedForHelp = false;
    int mCheckHelpNeeded = 0;
    int mHelpTimer = 1000000;

    std::vector<eTile*> mWarriorTiles;
    std::vector<eTile*> mSpecialTiles;
};

#endif // ESANCTUARY_H
