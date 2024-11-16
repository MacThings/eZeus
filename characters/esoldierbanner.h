#ifndef ESOLDIERBANNER_H
#define ESOLDIERBANNER_H

#include <vector>
#include <map>

#include "pointers/eobject.h"
#include "fileIO/estreams.h"
#include "engine/ecityid.h"

class eSoldier;
class eGameBoard;
class eTile;
enum class eCharacterType;

enum class eBannerType {
    hoplite,
    horseman,
    rockThrower,
    amazon,
    aresWarrior,
    enemy
};

class eSoldierBanner : public eObject {
public:
    eSoldierBanner(const eBannerType type,
                   eGameBoard& board);
    ~eSoldierBanner();

    eBannerType type() const { return mType; }
    eCharacterType characterType() const;
    stdsptr<eSoldier> createSoldier(eTile* const t);

    int id() const { return mId; }

    void setIOID(const int id) { mIOID = id; }
    int ioID() const { return mIOID; }

    bool militaryAid() const { return mMilitaryAid; }
    void setMilitaryAid(const bool a) { mMilitaryAid = a; }

    void moveTo(const int x, const int y);
    void moveToDefault();
    void goHome();
    void backFromHome();
    void goAbroad();
    void backFromAbroad(int& wait);
    void callSoldiers();

    bool isHome() const { return mHome; }
    bool isGoingHome() const;
    bool isAbroad() const { return mAbroad; }

    void addSoldier(eSoldier* const s);
    void removeSoldier(eSoldier* const s);

    eTile* tile() const { return mTile; }
    eTile* place(eSoldier* const s) const;

    void killAll();
    void killAllWithCorpse();

    void setSelected(const bool s) { mSelected = s; }
    bool selected() const { return mSelected; }

    void setCityId(const eCityId pid) { mCityId = pid; }
    eCityId cityId() const { return mCityId; }

    ePlayerId playerId() const;
    eTeamId teamId() const;

    int count() const { return mCount; }
    void incCount();
    void decCount();

    bool stationary() const;
    bool fighting() const;

    const std::string& name() const { return mName; }
    void setName(const std::string& n) { mName = n; }

    void read(eReadStream& src);
    void write(eWriteStream& dst) const;

    void setRegistered(const bool r) { mRegistered = r; }
    bool registered() const { return mRegistered; }


    bool nearestSoldier(const int fromX, const int fromY,
                        int& toX, int& toY) const;

    static void sPlace(std::vector<eSoldierBanner*> bs,
                       const int ctx, const int cty,
                       eGameBoard& board, const int dist,
                       const int minDistFromEdge);
    static std::string sName(const eBannerType type,
                             const bool atlantean);
private:
    void updatePlaces();
    void updateCount();
    void callSoldier(eSoldier* const s);
    void purgeDead();

    const eBannerType mType;
    const int mId;
    std::string mName;

    bool mMilitaryAid = false;

    int mIOID = -1;

    bool mRegistered = false;

    bool mHome = true;
    bool mAbroad = false;

    bool mSelected = false;

    eGameBoard& mBoard;

    eTile* mTile = nullptr;

    int mCount = 0;

    eCityId mCityId = eCityId::neutralFriendly;

    std::map<eSoldier*, eTile*> mPlaces;
    std::vector<eSoldier*> mSoldiers;
};

#endif // ESOLDIERBANNER_H
