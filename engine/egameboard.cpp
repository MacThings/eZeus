#include "egameboard.h"

#include <random>

#include "characters/echaracter.h"
#include "buildings/ebuilding.h"
#include "spawners/espawner.h"

#include "buildings/estoragebuilding.h"

#include "characters/gods/eathena.h"
#include "characters/gods/edemeter.h"
#include "characters/gods/edionysus.h"
#include "characters/gods/ehermes.h"
#include "characters/actions/egodvisitaction.h"

#include "characters/esoldier.h"
#include "characters/actions/esoldieraction.h"
#include "characters/erockthrower.h"
#include "characters/ehoplite.h"
#include "characters/ehorseman.h"
#include "characters/esoldierbanner.h"

#include "buildings/sanctuaries/esanctbuilding.h"
#include "buildings/etradepost.h"

#include "buildings/esmallhouse.h"
#include "buildings/eelitehousing.h"

#include "buildings/epalace.h"
#include "buildings/epalacetile.h"

#include "engine/boardData/eheatmaptask.h"

#include "missiles/emissile.h"

#include "etilehelper.h"

#include "emessages.h"

#include "buildings/eheatgetters.h"

#include "fileIO/ebuildingreader.h"
#include "fileIO/ebuildingwriter.h"

#include "characters/actions/egodworshippedaction.h"
#include "characters/actions/egodattackaction.h"

#include "gameEvents/egameeventcycle.h"
#include "gameEvents/egodvisitevent.h"
#include "gameEvents/egodattackevent.h"
#include "gameEvents/emonsterattackevent.h"

#include "evectorhelpers.h"

eGameBoard::eGameBoard() :
    mEmplData(mPopData, *this) {
    const int min = static_cast<int>(eBuildingMode::road);
    const int max = static_cast<int>(eBuildingMode::theseusHall);
    for(int i = min; i <= max; i++) {
        const auto bm = static_cast<eBuildingMode>(i);
        mSupportedBuildings.push_back(bm);
    }
    mSupportedResources = eResourceType::all;
}

eGameBoard::~eGameBoard() {
    clear();
}

void eGameBoard::initialize(const int w, const int h) {
    mThreadPool.initialize(w, h);

    clear();
    mTiles.reserve(w);
    for(int x = 0; x < w; x++) {
        std::vector<eTile*> yArr;
        yArr.reserve(h);
        for(int y = 0; y < h; y++) {
            int tx;
            int ty;
            eTileHelper::dtileIdToTileId(x, y, tx, ty);
            const auto tile = new eTile(tx, ty, x, y);
            yArr.push_back(tile);
        }
        mTiles.push_back(yArr);
    }
    mWidth = w;
    mHeight = h;

    mAppealMap.initialize(w, h);

    updateNeighbours();
}

void eGameBoard::clear() {
    for(const auto& x : mTiles) {
        for(const auto y : x) {
            delete y;
        }
    }
    mTiles.clear();
    mWidth = 0;
    mHeight = 0;
}

void eGameBoard::iterateOverAllTiles(const eTileAction& a) {
    for(int y = 0; y < mHeight; y++) {
        for(int x = 0; x < mWidth; x++) {
            const auto t = dtile(x, y);
            a(t);
        }
    }
}

void eGameBoard::scheduleAppealMapUpdate() {
    mUpdateAppeal = true;
}

void eGameBoard::updateAppealMapIfNeeded() {
    if(!mUpdateAppeal) return;
    mUpdateAppeal = false;
    const auto finish = [this](eHeatMap& map) {
        std::swap(appealMap(), map);
    };
    const auto task = new eHeatMapTask(eHeatGetters::appeal, finish);
    mThreadPool.queueTask(task);
}

void eGameBoard::clearSoldierSelection() {
    for(const auto s : mSelectedBanners) {
        s->setSelected(false);
    }
    mSelectedBanners.clear();
}

void eGameBoard::deselectSoldier(eSoldierBanner* const c) {
    eVectorHelpers::remove(mSelectedBanners, c);
    c->setSelected(false);
}

void eGameBoard::selectSoldier(eSoldierBanner* const c) {
    mSelectedBanners.push_back(c);
    c->setSelected(true);
}

void eGameBoard::bannersGoHome() {
    for(const auto b : mSelectedBanners) {
        b->goHome();
    }
}

void eGameBoard::bannersBackFromHome() {
    for(const auto b : mSelectedBanners) {
        b->backFromHome();
    }
}

void eGameBoard::setRegisterBuildingsEnabled(const bool e) {
    mRegisterBuildingsEnabled = e;
}

void eGameBoard::setButtonsVisUpdater(const eAction& u) {
    mButtonVisUpdater = u;
}

void eGameBoard::addSupportedBuilding(const eBuildingMode t) {
    if(supportsBuilding(t)) return;
    mSupportedBuildings.push_back(t);
    mButtonVisUpdater();
}

void eGameBoard::removeSupportedBuilding(const eBuildingMode t) {
    const bool r = eVectorHelpers::remove(mSupportedBuildings, t);
    if(r) mButtonVisUpdater();
}

bool eGameBoard::supportsBuilding(const eBuildingMode t) const {
    return eVectorHelpers::contains(mSupportedBuildings, t);
}

bool eGameBoard::supportsResource(const eResourceType rt) const {
    const auto s = mSupportedResources & rt;
    return static_cast<bool>(s);
}

eResourceType eGameBoard::supportedResources() const {
    return mSupportedResources;
}

eBuilding* eGameBoard::randomBuilding(const eBuildingValidator& v) const {
    auto blds = mTimedBuildings;
    std::random_shuffle(blds.begin(), blds.end());
    for(const auto b : blds) {
        const bool r = v(b);
        if(r) return b;
    }
    return nullptr;
}

void eGameBoard::planAction(ePlannedAction* const a) {
    mPlannedActions.emplace_back(a);
}

void eGameBoard::restockMarbleTiles() {
    int maxLevel = 0;
    for(const auto t : mMarbleTiles) {
        if(t->resource() > 0) return;
        const int l = t->marbleLevel();
        if(l > maxLevel) {
            maxLevel = l;
        }
    }
    for(const auto t : mMarbleTiles) {
        const int l = t->marbleLevel();
        if(l == maxLevel) {
            t->setResource(10000);
        }
    }
}

void eGameBoard::updateMarbleTiles() {
    mMarbleTiles.clear();
    iterateOverAllTiles([&](eTile* const t) {
        if(t->terrain() != eTerrain::marble) return;
        mMarbleTiles.push_back(t);
    });
}
void eGameBoard::setFriendlyGods(const std::vector<eGodType>& gods) {
    mFriendlyGods = gods;

    const auto e = e::make_shared<eGodVisitEvent>(*this);
    e->setTypes(gods);
    eDate date = mDate;
    const int period = 450;
    date += period;
    const auto ec = e::make_shared<eGameEventCycle>(
                        e, date, period, 10000, *this);
    addGameEvent(ec);
}

void eGameBoard::setHostileGods(const std::vector<eGodType>& gods) {
    mHostileGods = gods;

    const auto e = e::make_shared<eGodAttackEvent>(*this);
    e->setTypes(gods);
    eDate date = mDate;
    const int period = 900;
    date += period;
    const auto ec = e::make_shared<eGameEventCycle>(
                        e, date, period, 10000, *this);
    addGameEvent(ec);
}

void eGameBoard::setMonsters(const std::vector<eMonsterType>& monsters) {
    mMonsters = monsters;

    const auto e = e::make_shared<eMonsterAttackEvent>(*this);
    e->setTypes(monsters);
    const int period = 150;
    const auto date = mDate + period;
    const auto ec = e::make_shared<eGameEventCycle>(
                        e, date, period, 2, *this);
    addGameEvent(ec);
}

void eGameBoard::read(eReadStream& src) {
    int w;
    src >> w;
    int h;
    src >> h;
    initialize(w, h);

    for(const auto& ts : mTiles) {
        for(const auto& t : ts) {
            t->read(src);
        }
    }

    int nbs;
    src >> nbs;
    for(int i = 0; i < nbs; i++) {
        eBuildingType type;
        src >> type;
        eBuildingReader::sRead(*this, type, src);
    }

    int nfg;
    src >> nfg;
    for(int i = 0; i < nfg; i++) {
        eGodType type;
        src >> type;
        mFriendlyGods.push_back(type);
    }

    int nrg;
    src >> nrg;
    for(int i = 0; i < nrg; i++) {
        eGodType type;
        src >> type;
        mHostileGods.push_back(type);
    }

    src.handlePostFuncs();
}

void eGameBoard::write(eWriteStream& dst) const {
    dst << mWidth;
    dst << mHeight;

    for(const auto& ts : mTiles) {
        for(const auto& t : ts) {
            t->write(dst);
        }
    }

    int id = 0;
    for(const auto b : mAllBuildings) {
        b->setIOID(id++);
    }
    const int nbs = mAllBuildings.size();
    dst << nbs;
    for(const auto b : mAllBuildings) {
        dst << b->type();
        eBuildingWriter::sWrite(b, dst);
    }

    const int nfg = mFriendlyGods.size();
    dst << nfg;
    for(const auto g : mFriendlyGods) {
        dst << g;
    }

    const int nrg = mHostileGods.size();
    dst << nrg;
    for(const auto g : mHostileGods) {
        dst << g;
    }
}

eBuilding* eGameBoard::buildingWithIOID(const int id) const {
    for(const auto b : mAllBuildings) {
        const int bio = b->ioID();
        if(bio == id) return b;
    }
    return nullptr;
}

void eGameBoard::updateTileRenderingOrder() {
    std::vector<std::vector<std::pair<int, int>>> map;
    map.reserve(width());
    for(int x = 0; x < width(); x++) {
        auto& m = map.emplace_back();
        m.reserve(height());
        for(int y = 0; y < height(); y++) {
            m.push_back({0, 0});
        }
    }

    const auto updateRenderingOrder = [&](eTile* const tile) {
        tile->terrainTiles().clear();
        bool elev = false;
        const int a = tile->altitude();
        for(int x = 0; x < 2 && !elev; x++) {
            for(int y = 0; y < 2 && !elev; y++) {
                if(x == 0 && y == 0) continue;
                const auto t = tile->tileRel(x, y);
                if(!t) continue;
                const int ta = t->altitude();
                if(ta > a) elev = true;
            }
        }
        if(elev) {
            tile->addTerrainTile(tile);
            const int dx = tile->dx();
            const int dy = tile->dy();
            map[dx][dy] = {dx, dy};
            return;
        }
        const auto ubt = tile->underBuildingType();
        const auto ub = tile->underBuilding();
        const auto sc = dynamic_cast<eSanctBuilding*>(ub);
        const bool r = sc && sc->progress() == 0;
        if(r || eBuilding::sFlatBuilding(ubt)) {
            const int tx = tile->dx();
            const int ty = tile->dy();
            bool found = false;
            eTile* lastT = tile;
            int ttx;
            int tty;

            int mmx = 0;
            int mmy = 0;

            for(int i = 1; i < 3 && !found; i++) {
                tty = ty - i;
                const int w = i + 1;
                const int ddx = ty % 2 == 0 ? 1 : 0;
                const int dx = -(i + ddx)/2;
                ttx = tx + i + dx;
                for(int j = 0; j < w && !found; j++, ttx--) {
                    const auto t = eGameBoard::dtile(ttx, tty);
                    if(!t) continue;
                    {
                        const auto& coords = map[ttx][tty];
                        const int cx = coords.first;
                        const int cy = coords.second;
                        if(cy > mmy) {
                            mmy = cy;
                            mmx = cx;
                        } else if(cy == mmy) {
                            if(cx > mmx) {
                                mmx = cx;
                            }
                        }
                        if(tty < mmy || (tty == mmy && ttx < mmx)) {
                            found = true;
                            break;
                        }
                    }
                    lastT = t;
//                    if(t->altitude() < tile->altitude()) {
//                        found = true;
//                        break;
//                    }
                    const auto tubt = t->underBuildingType();
                    const auto ub = t->underBuilding();
                    const auto sc = dynamic_cast<eSanctBuilding*>(ub);
                    const bool r = sc && sc->progress() == 0;
                    if(!r && !eBuilding::sFlatBuilding(tubt)) {
//                        const auto b = t->underBuilding();
//                        const auto ct = b->centerTile();
                        lastT = t;
                        found = true;
                        break;
                    }
                }
            }

            if(lastT) {
                lastT->addTerrainTile(tile);
                const int dx = tile->dx();
                const int dy = tile->dy();
                const int ddx = lastT->dx();
                const int ddy = lastT->dy();
                map[dx][dy] = {ddx, ddy};
            }
        }
    };

    iterateOverAllTiles([&](eTile* const tile) {
        updateRenderingOrder(tile);
    });
}

void eGameBoard::updateMaxSoldiers() {
    mMaxRockThrowers = 0;
    mMaxHoplites = 0;
    mMaxHorsemen = 0;
    for(const auto b : mTimedBuildings) {
        const auto bt = b->type();
        if(bt == eBuildingType::commonHouse) {
            const auto ch = static_cast<eSmallHouse*>(b);
            const int l = ch->level();
            if(l < 2) continue;
            if(l == 2) mMaxRockThrowers += 5;
            else if(l == 3) mMaxRockThrowers += 6;
            else if(l == 4) mMaxRockThrowers += 10;
            else if(l == 5) mMaxRockThrowers += 12;
            else if(l == 6) mMaxRockThrowers += 15;
        } else if(bt == eBuildingType::eliteHousing) {
            const auto eh = static_cast<eEliteHousing*>(b);
            const int l = eh->level();
            if(l < 2) continue;
            const int a = eh->arms();
            if(l == 2) {
                mMaxHoplites += std::min(2, a);
            } else if(l == 3) {
                mMaxHoplites += std::min(4, a);
            } else if(l == 4) {
                const int h = eh->horses();
                const int hh = std::min(std::min(a, 4), h);
                mMaxHorsemen += hh;
                mMaxHoplites += std::min(4 - hh, a - hh);
            }
        }
    }
    mMaxRockThrowers /= 6;
}

void eGameBoard::addSoldier(const eCharacterType st) {
    bool found = false;
    for(const auto& b : mBanners) {
        const auto bt = b->type();
        const int c = b->count();
        if(c >= 8) continue;
        if(bt == eBannerType::rockThrower &&
           st == eCharacterType::rockThrower) {
            found = true;
        } else if(bt == eBannerType::hoplite &&
                  st == eCharacterType::hoplite) {
            found = true;
        } else if(bt == eBannerType::horseman &&
                  st == eCharacterType::horseman){
            found = true;
        }
        if(found) {
            b->incCount();
            break;
        }
    }
    if(found) return;
    eBannerType bt;
    if(st == eCharacterType::rockThrower) {
        bt = eBannerType::rockThrower;
    } else if(st == eCharacterType::hoplite) {
        bt = eBannerType::hoplite;
    } else { //horseman
        bt = eBannerType::horseman;
    }
    const auto b = e::make_shared<eSoldierBanner>(bt, *this);
    registerBanner(b);
    b->incCount();
    const auto ts = mPalace->tiles();
    for(const auto& t : ts) {
        const auto tt = t->centerTile();
        if(!tt) continue;
        const auto bb = tt->banner();
        if(bb) continue;
        b->moveTo(tt->x(), tt->y());
        break;
    }
}

void eGameBoard::removeSoldier(const eCharacterType st) {
    for(const auto& b : mBanners) {
        const auto bt = b->type();
        const int c = b->count();
        if(c <= 0) continue;
        bool found = false;
        if(bt == eBannerType::rockThrower &&
           st == eCharacterType::rockThrower) {
            found = true;
        } else if(bt == eBannerType::hoplite &&
                  st == eCharacterType::hoplite) {
            found = true;
        } else if(bt == eBannerType::horseman &&
                  st == eCharacterType::horseman){
            found = true;
        }
        if(found) {
            b->decCount();
            if(b->count() <= 0) {
                unregisterBanner(b);
            }
            break;
        }
    }
}

void eGameBoard::distributeSoldiers() {
    if(!hasPalace()) return;
    int cRockThrowers = 0;
    int cHoplites = 0;
    int cHorsemen = 0;
    for(const auto& b : mBanners) {
        const auto bt = b->type();
        const int c = b->count();
        if(bt == eBannerType::rockThrower) {
            cRockThrowers += c;
        } else if(bt == eBannerType::hoplite) {
            cHoplites += c;
        } else { //horseman
            cHorsemen += c;
        }
    }
    const int remRockThrowers = mMaxRockThrowers - cRockThrowers;
    const int remHoplites = mMaxHoplites - cHoplites;
    const int remHorsemen = mMaxHorsemen - cHorsemen;

    for(int i = 0; i < remRockThrowers; i++) {
        addSoldier(eCharacterType::rockThrower);
    }
    for(int i = 0; i < -remRockThrowers; i++) {
        removeSoldier(eCharacterType::rockThrower);
    }
    for(int i = 0; i < remHoplites; i++) {
        addSoldier(eCharacterType::hoplite);
    }
    for(int i = 0; i < -remHoplites; i++) {
        removeSoldier(eCharacterType::hoplite);
    }
    for(int i = 0; i < remHorsemen; i++) {
        addSoldier(eCharacterType::horseman);
    }
    for(int i = 0; i < -remHorsemen; i++) {
        removeSoldier(eCharacterType::horseman);
    }
}

void eGameBoard::registerBanner(const stdsptr<eSoldierBanner>& b) {
    mBanners.push_back(b);
}

bool eGameBoard::unregisterBanner(const stdsptr<eSoldierBanner>& b) {
    return eVectorHelpers::remove(mBanners, b);
}

void eGameBoard::addGameEvent(const stdsptr<eGameEventCycle>& e) {
    mGameEvents.push_back(e);
}

void eGameBoard::updateCoverage() {
    int people = 0;
    int sport = 0;
    int phil = 0;
    int drama = 0;
    for(const auto b : mTimedBuildings) {
        if(const auto h = dynamic_cast<eHouseBase*>(b)) {
            const int p = h->people();
            if(h->athletes() > 0) {
                sport += p;
            } else if(h->philosophers() > 0) {
                phil += p;
            } else if(h->actors() > 0) {
                drama += p;
            }
            people += p;
        }
    }
    if(people <= 0) {
        mAthleticsCoverage = 0;
        mPhilosophyCoverage = 0;
        mDramaCoverage = 0;
    } else {
        mAthleticsCoverage = 100*sport/people;
        mPhilosophyCoverage = 100*phil/people;
        mDramaCoverage = 100*drama/people;
    }
    mAllDiscCoverage = (mAthleticsCoverage + mPhilosophyCoverage + mDramaCoverage)/3;
}

double coverageMultiplier(const int pop) {
    if(pop < 250) return 0.125;
    else if(pop < 500) return 0.25;
    else if(pop < 1000) return 0.375;
    else return 0.5;
}

void eGameBoard::handleGamesBegin(const eGames game) {
    int coverage = 0;
    eGameMessages* msgs = nullptr;
    switch(game) {
    case eGames::isthmian:
        msgs = &eMessages::instance.fIsthmianGames;
        coverage = mPhilosophyCoverage;
        break;
    case eGames::nemean:
        msgs = &eMessages::instance.fNemeanGames;
        coverage = mAthleticsCoverage;
        break;
    case eGames::pythian:
        msgs = &eMessages::instance.fPythianGames;
        coverage = mDramaCoverage;
        break;
    case eGames::olympian:
        msgs = &eMessages::instance.fOlympianGames;
        coverage = mAllDiscCoverage;
        break;
    }
    const int pop = mPopData.population();
    const double mult = coverageMultiplier(pop);
    if(mult*coverage > 15) {
        showMessage(nullptr, msgs->fBegin);
    } else {
        showMessage(nullptr, msgs->fNoPart);
    }
}

void eGameBoard::handleGamesEnd(const eGames game) {
    int coverage = 0;
    eGameMessages* msgs = nullptr;
    switch(game) {
    case eGames::isthmian:
        msgs = &eMessages::instance.fIsthmianGames;
        coverage = mPhilosophyCoverage;
        break;
    case eGames::nemean:
        msgs = &eMessages::instance.fNemeanGames;
        coverage = mAthleticsCoverage;
        break;
    case eGames::pythian:
        msgs = &eMessages::instance.fPythianGames;
        coverage = mDramaCoverage;
        break;
    case eGames::olympian:
        msgs = &eMessages::instance.fOlympianGames;
        coverage = mAllDiscCoverage;
        break;
    }

    const int pop = mPopData.population();
    const double mult = coverageMultiplier(pop);

    if(mult*coverage < 15) {
        return;
    } else {
        const double coveragef = coverage/100.;
        const double chance = mult*coveragef*coveragef;
        const bool won = rand() % 101 < 100*chance;
        if(won) {
            showMessage(nullptr, msgs->fWon);
        } else {
            const bool second = rand() % 101 < 200*chance;
            if(second) {
                showMessage(nullptr, msgs->fSecond);
            } else {
                showMessage(nullptr, msgs->fLost);
            }
        }
    }
}

void eGameBoard::updateTileRenderingOrderIfNeeded() {
    if(mTileRenderingOrderUpdateNeeded) {
        updateTileRenderingOrder();
        mTileRenderingOrderUpdateNeeded = false;
    }
}

eTile* eGameBoard::tile(const int x, const int y) const {
    int dtx;
    int dty;
    eTileHelper::tileIdToDTileId(x, y, dtx, dty);
    return dtile(dtx, dty);
}

eTile* eGameBoard::dtile(const int x, const int y) const {
    if(x < 0 || x >= mWidth) return nullptr;
    if(y < 0 || y >= mHeight) return nullptr;
    return mTiles[x][y];
}

void eGameBoard::registerCharacter(eCharacter* const c) {
    mCharacters.push_back(c);
}

bool eGameBoard::unregisterCharacter(eCharacter* const c) {
    return eVectorHelpers::remove(mCharacters, c);
}

void eGameBoard::registerSoldier(eSoldier* const c) {
    mSoldiers.push_back(c);
}

bool eGameBoard::unregisterSoldier(eSoldier* const c) {
    return eVectorHelpers::remove(mSoldiers, c);
}

void eGameBoard::registerBuilding(eBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mAllBuildings.push_back(b);
    if(eBuilding::sTimedBuilding(b->type())) {
        mTimedBuildings.push_back(b);
    }
    mTileRenderingOrderUpdateNeeded = true;
    scheduleAppealMapUpdate();
}

bool eGameBoard::unregisterBuilding(eBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    eVectorHelpers::remove(mAllBuildings, b);
    eVectorHelpers::remove(mTimedBuildings, b);
    mTileRenderingOrderUpdateNeeded = true;
    scheduleAppealMapUpdate();
    return true;
}

void eGameBoard::registerTradePost(eTradePost* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mTradePosts.push_back(b);
    mButtonVisUpdater();
}

bool eGameBoard::unregisterTradePost(eTradePost* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    const bool r = eVectorHelpers::remove(mTradePosts, b);
    if(r) mButtonVisUpdater();
    return r;
}

bool eGameBoard::hasTradePost(const eWorldCity& city) {
    for(const auto t : mTradePosts) {
        const bool r = &t->city() == &city;
        if(r) return true;
    }
    return false;
}

void eGameBoard::registerSpawner(eSpawner* const s) {
    mSpawners.push_back(s);
}

bool eGameBoard::unregisterSpawner(eSpawner* const s) {
    return eVectorHelpers::remove(mSpawners, s);;
}

void eGameBoard::registerStadium(eBuilding* const s) {
    if(!mRegisterBuildingsEnabled) return;
    mStadium = s;
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::unregisterStadium() {
    if(!mRegisterBuildingsEnabled) return;
    mStadium = nullptr;
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::registerStorBuilding(eStorageBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mStorBuildings.push_back(b);
}

bool eGameBoard::unregisterStorBuilding(eStorageBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    return eVectorHelpers::remove(mStorBuildings, b);
    return true;
}

void eGameBoard::registerMissile(eMissile* const m) {
    mMissiles.push_back(m);
}

bool eGameBoard::unregisterMissile(eMissile* const m) {
    return eVectorHelpers::remove(mMissiles, m);
}

void eGameBoard::registerPalace(ePalace* const p) {
    if(!mRegisterBuildingsEnabled) return;
    mPalace = p;
    mButtonVisUpdater();
}

void eGameBoard::unregisterPalace() {
    if(!mRegisterBuildingsEnabled) return;
    mPalace = nullptr;
    mButtonVisUpdater();
}

void eGameBoard::incTime(const int by) {
    mThreadPool.handleFinished();
    mThreadPool.scheduleUpdate(*this);

    updateAppealMapIfNeeded();

    const int iMax = mPlannedActions.size() - 1;
    for(int i = iMax; i >= 0; i--) {
        const auto a = mPlannedActions[i];
        a->incTime(by);
        if(a->finished()) {
            mPlannedActions.erase(mPlannedActions.begin() + i);
            delete a;
        }
    }

    mSoldiersUpdate += by;
    const int sup = 2000;
    if(mSoldiersUpdate > sup) {
        mSoldiersUpdate -= sup;
        updateMaxSoldiers();
        distributeSoldiers();
    }

    mCoverageUpdate += by;
    const int cup = 2000;
    if(mCoverageUpdate > cup) {
        mCoverageUpdate -= cup;
        updateCoverage();
    }

    mTime += by;
    bool nextMonth = false;
    bool nextYear = false;
    const int dayLen = 350;
    const int nd = mTime/dayLen;
    mDate.nextDays(nd, nextMonth, nextYear);
    mTime -= nd*dayLen;

    for(const auto& e : mGameEvents) {
        e->handleNewDate(mDate);
    }

    if(nextYear) {
        const int d = mEmplData.pensions();
        incDrachmas(-d);
    }
    if(nextMonth) {
        mPopData.nextMonth();

        const auto m = mDate.month();
        const int ng = std::abs(mDate.year() % 4);
        const auto game = static_cast<eGames>(ng);
        if(m == eMonth::june) {
            handleGamesBegin(game);
        } else if(m == eMonth::august) {
            handleGamesEnd(game);
        }
    }
    mWorldBoard.incTime(by);
    if(nextYear) mWorldBoard.nextYear();
    const auto chars = mCharacters;
    for(const auto c : chars) {
        if(c->isSoldier()) continue;
        c->incTime(by);
    }
    const auto solds = mSoldiers;
    for(const auto c : solds) {
        c->incTime(by);
    }
    const auto build = mTimedBuildings;
    for(const auto b : build) {
        b->incTime(by);
        if(nextMonth) b->nextMonth();
    }
    for(const auto s : mSpawners) {
        s->incTime(by);
    }

    const auto missiles = mMissiles;
    for(const auto m : missiles) {
        m->incTime(by);
    }

    emptyRubbish();
}

void eGameBoard::incDrachmas(const int d) {
    mDrachmas += d;
}

double eGameBoard::appeal(const int tx, const int ty) const {
    return mAppealMap.heat(tx, ty);
}

double eGameBoard::taxRateF() const {
    return eTaxRateHelpers::getRate(mTaxRate);
}

void eGameBoard::setTaxRate(const eTaxRate tr) {
    mTaxRate = tr;
}

void eGameBoard::setWageRate(const eWageRate wr) {
    mWageRate = wr;
}

void eGameBoard::addRubbish(const stdsptr<eObject>& o) {
    mRubbish.push_back(o);
}

void eGameBoard::emptyRubbish() {
    while(!mRubbish.empty()) {
        std::vector<stdsptr<eObject>> r;
        std::swap(mRubbish, r);
    }
}

void eGameBoard::setEventHandler(const eEventHandler& eh) {
    mEventHandler = eh;
}

void eGameBoard::event(const eEvent e, eTile* const tile) {
    if(mEventHandler) mEventHandler(e, tile);
}

void eGameBoard::setVisibilityChecker(const eVisibilityChecker& vc) {
    mVisibilityChecker = vc;
}

void eGameBoard::setTipShower(const eTipShower& ts) {
    mTipShower = ts;
}

void eGameBoard::showTip(const std::string& tip) {
    if(mTipShower) mTipShower(tip);
}

bool eGameBoard::ifVisible(eTile* const tile, const eAction& func) const {
    if(!tile) return false;
    if(!mVisibilityChecker) return false;
    const bool r = mVisibilityChecker(tile);
    if(r) func();
    return r;
}

void eGameBoard::setMessageShower(const eMessageShower& msg) {
    mMsgShower = msg;
}

void eGameBoard::showMessage(eTile* const t, const eMessageType& msg) {
    mMsgShower(t, msg);
}

void eGameBoard::requestTileRenderingOrderUpdate() {
    mTileRenderingOrderUpdateNeeded = true;
}

void eGameBoard::updateNeighbours() {
    for(int x = 0; x < mWidth; x++) {
        for(int y = 0; y < mHeight; y++) {
            const auto t = mTiles[x][y];
            {
                const int dx = y % 2 == 0 ? -1 : 0;
                t->setTopLeft(dtile(x + dx, y - 1));
                t->setBottomLeft(dtile(x + dx, y + 1));
            }
            {
                const int dx = y % 2 == 0 ? 0 : 1;
                t->setTopRight(dtile(x + dx, y - 1));
                t->setBottomRight(dtile(x + dx, y + 1));
            }
        }
    }
}

void eGameBoard::updateResources() {
    for(auto& r : mResources) {
        int& count = r.second;
        count = 0;
        const auto type = r.first;
        for(const auto s : mStorBuildings) {
            count += s->count(type);
        }
    }
}
