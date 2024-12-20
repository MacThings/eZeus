﻿
#include "egameboard.h"

#include <random>

#include "buildings/eagorabase.h"
#include "characters/echaracter.h"
#include "buildings/ebuilding.h"
#include "spawners/espawner.h"

#include "buildings/estoragebuilding.h"

#include "characters/esoldier.h"
#include "characters/esoldierbanner.h"

#include "buildings/sanctuaries/esanctbuilding.h"
#include "buildings/etradepost.h"

#include "buildings/esmallhouse.h"
#include "buildings/eelitehousing.h"

#include "buildings/epalace.h"
#include "buildings/epalacetile.h"

#include "buildings/ehorseranch.h"

#include "engine/boardData/eheatmaptask.h"

#include "missiles/emissile.h"

#include "etilehelper.h"

#include "emessages.h"

#include "spawners/emonsterpoint.h"
#include "spawners/elandinvasionpoint.h"

#include "buildings/eheatgetters.h"

#include "fileIO/ebuildingreader.h"
#include "fileIO/ebuildingwriter.h"

#include "eevent.h"
#include "emessageeventtype.h"

#include "gameEvents/egodvisitevent.h"
#include "gameEvents/egodattackevent.h"
#include "gameEvents/emonsterunleashedevent.h"
#include "gameEvents/einvasionevent.h"
#include "gameEvents/epaytributeevent.h"
#include "gameEvents/emakerequestevent.h"
#include "gameEvents/egifttoevent.h"
#include "gameEvents/egiftfromevent.h"
#include "gameEvents/ereceiverequestevent.h"
#include "gameEvents/erequestaidevent.h"

#include "eeventdata.h"

#include "einvasionhandler.h"
#include "characters/actions/emonsteraction.h"

#include "evectorhelpers.h"
#include "egifthelpers.h"
#include "estringhelpers.h"

#include "buildings/eheroshall.h"
#include "buildings/emuseum.h"
#include "buildings/estadium.h"
#include "buildings/eagoraspace.h"
#include "buildings/evendor.h"
#include "buildings/eroad.h"
#include "buildings/egatehouse.h"
#include "eplague.h"
#include "audio/emusic.h"

#include "ecampaign.h"
#include "eiteratesquare.h"
#include "ecolonymonumentaction.h"
#include "textures/emarbletile.h"
#include "elanguage.h"
#include "enumbers.h"

#include <algorithm>
#include <random>

eGameBoard::eGameBoard() :
    mThreadPool(*this),
    mHusbData(mPopData, *this),
    mEmplData(mPopData, *this),
    mEmplDistributor(mEmplData) {
    const auto types = eResourceTypeHelpers::extractResourceTypes(
                           eResourceType::allBasic);
    for(const auto type : types) {
        mPrices[type] = eResourceTypeHelpers::defaultPrice(type);
    }
    mSupportedResources = eResourceType::allBasic;
}

eGameBoard::~eGameBoard() {
    mRegisterBuildingsEnabled = false;
    clear();
}

void eGameBoard::initialize(const int w, const int h) {
    waitUntilFinished();
    mThreadPool.initialize(w, h);

    clear();
    mTiles.reserve(w);
    for(int x = 0; x < w; x++) {
        auto& yArr = mTiles.emplace_back();
        yArr.reserve(h);
        for(int y = 0; y < h; y++) {
            int tx;
            int ty;
            eTileHelper::dtileIdToTileId(x, y, tx, ty);
            const auto tile = new eTile(tx, ty, x, y);
            yArr.push_back(tile);
        }
    }
    mWidth = w;
    mHeight = h;

    mAppealMap.initialize(w, h);

    updateNeighbours();
    updateMarbleTiles();
    scheduleTerrainUpdate();
}

void eGameBoard::resize(const int w, const int h) {
    waitUntilFinished();
    mThreadPool.initialize(w, h);

    mTiles.reserve(w);

    for(int x = w; x < mWidth; x++) {
        mTiles.pop_back();
    }
    for(int x = 0; x < w; x++) {
        auto& yArr = mTiles[x];
        for(int y = h; y < mHeight; y++) {
            yArr.pop_back();
        }
    }

    for(int x = 0; x < w; x++) {
        auto& yArr = x < mWidth ? mTiles[x] : mTiles.emplace_back();
        yArr.reserve(h);
        for(int y = 0; y < h; y++) {
            if(x < mWidth && y < mHeight) continue;
            int tx;
            int ty;
            eTileHelper::dtileIdToTileId(x, y, tx, ty);
            const auto tile = new eTile(tx, ty, x, y);
            yArr.push_back(tile);
        }
    }
    mWidth = w;
    mHeight = h;

    mAppealMap.initialize(w, h);

    updateNeighbours();
    updateMarbleTiles();
    scheduleTerrainUpdate();
}

void eGameBoard::clear() {
    for(const auto c : mCharacters) {
        c->kill();
    }
    emptyRubbish();
    for(const auto& x : mTiles) {
        for(const auto y : x) {
            delete y;
        }
    }
    mTiles.clear();
    mWidth = 0;
    mHeight = 0;
    emptyRubbish();
}

void eGameBoard::setWorldDirection(const eWorldDirection dir) {
    mDirection = dir;
    scheduleTerrainUpdate();
}

eTile* eGameBoard::rotateddtile(const int x, const int y) const {
    int rx;
    int ry;
    eTileHelper::rotatedDTileIdToDTileId(
            x, y, rx, ry, mDirection, mWidth, mHeight);
    return dtile(rx, ry);
}

int eGameBoard::rotatedWidth() const {
    if(mDirection == eWorldDirection::N) {
        return mWidth;
    } else if(mDirection == eWorldDirection::E) {
        return mHeight/2;
    } else if(mDirection == eWorldDirection::S) {
        return mWidth;
    } else { // if(mDirection == eWorldDirection::W) {
        return mHeight/2;
    }}

int eGameBoard::rotatedHeight() const {
    if(mDirection == eWorldDirection::N) {
        return mHeight;
    } else if(mDirection == eWorldDirection::E) {
        return 2*mWidth;
    } else if(mDirection == eWorldDirection::S) {
        return mHeight;
    } else { // if(mDirection == eWorldDirection::W) {
        return 2*mWidth;
    }
}

void eGameBoard::iterateOverAllTiles(const eTileAction& a) {
    const int height = rotatedHeight();
    const int width = rotatedWidth();
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            const auto t = rotateddtile(x, y);
            if(!t) continue;
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

void eGameBoard::enlistForces(const eEnlistedForces& forces) {
    for(const auto& b : forces.fSoldiers) {
        b->goAbroad();
    }
    for(const auto h : forces.fHeroes) {
        const auto hh = heroHall(h);
        if(!hh) continue;
        hh->sendHeroOnQuest();
    }
    for(const auto& a : forces.fAllies) {
        a->setAbroad(true);
    }
}

void eGameBoard::clearBannerSelection() {
    for(const auto s : mSelectedBanners) {
        s->setSelected(false);
    }
    mSelectedBanners.clear();
}

void eGameBoard::deselectBanner(eSoldierBanner* const c) {
    eVectorHelpers::remove(mSelectedBanners, c);
    c->setSelected(false);
}

void eGameBoard::selectBanner(eSoldierBanner* const c) {
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

bool eGameBoard::supportsBuilding(const eBuildingMode mode) const {
    const auto type = eBuildingModeHelpers::toBuildingType(mode);
    int id = -1;
    switch(mode) {
    case eBuildingMode::populationMonument:
    case eBuildingMode::victoryMonument:
    case eBuildingMode::colonyMonument:
    case eBuildingMode::athleteMonument:
    case eBuildingMode::conquestMonument:
    case eBuildingMode::happinessMonument:
    case eBuildingMode::heroicFigureMonument:
    case eBuildingMode::diplomacyMonument:
    case eBuildingMode::scholarMonument:
        id = eBuildingModeHelpers::toCommemorativeId(mode);
        break;
    case eBuildingMode::aphroditeMonument:
    case eBuildingMode::apolloMonument:
    case eBuildingMode::aresMonument:
    case eBuildingMode::artemisMonument:
    case eBuildingMode::athenaMonument:
    case eBuildingMode::atlasMonument:
    case eBuildingMode::demeterMonument:
    case eBuildingMode::dionysusMonument:
    case eBuildingMode::hadesMonument:
    case eBuildingMode::hephaestusMonument:
    case eBuildingMode::heraMonument:
    case eBuildingMode::hermesMonument:
    case eBuildingMode::poseidonMonument:
    case eBuildingMode::zeusMonument: {
        const auto gt = eBuildingModeHelpers::toGodType(mode);
        id = static_cast<int>(gt);
    } break;
    default:
        break;
    }

    return availableBuilding(type, id);
}

bool eGameBoard::availableBuilding(const eBuildingType type,
                                   const int id) const {
    switch(type) {
    case eBuildingType::chariotVendor:
    case eBuildingType::chariotFactory: {
        if(!mPoseidonMode) return false;
    } break;
    case eBuildingType::horseTrainer: {
        if(mPoseidonMode) return false;
    } break;
    default:
        break;
    }

    return mAvailableBuildings.available(type, id);
}

void eGameBoard::built(const eBuildingType type,
                       const int id) {
    mAvailableBuildings.built(type, id);
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::destroyed(const eBuildingType type,
                           const int id) {
    if(!mRegisterBuildingsEnabled) return;
    mAvailableBuildings.destroyed(type, id);
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::allow(const eBuildingType type,
                       const int id) {
    mAvailableBuildings.allow(type, id);
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::disallow(const eBuildingType type,
                          const int id) {
    mAvailableBuildings.disallow(type, id);
    if(mButtonVisUpdater) mButtonVisUpdater();
}

bool eGameBoard::supportsResource(const eResourceType rt) const {
    const auto s = mSupportedResources & rt;
    return static_cast<bool>(s);
}

eResourceType eGameBoard::supportedResources() const {
    return mSupportedResources;
}

std::vector<eBuilding*> eGameBoard::buildings(const eBuildingValidator& v) const {
    std::vector<eBuilding*> result;
    for(const auto b : mTimedBuildings) {
        const bool r = v(b);
        if(r) result.push_back(b);
    }
    return result;
}

std::vector<eBuilding*> eGameBoard::buildings(const eBuildingType type) const {
    return buildings([type](eBuilding* const b) {
        const auto bt = b->type();
        return type == bt;
    });
}

int eGameBoard::countBuildings(const eBuildingValidator& v) const {
    const auto vv = buildings(v);
    return vv.size();
}

int eGameBoard::countBuildings(const eBuildingType t) const {
    return countBuildings([t](eBuilding* const b) {
        const auto bt = b->type();
        return bt == t;
    });
}

int eGameBoard::hasBuilding(const eBuildingType t) const {
    for(const auto b : mTimedBuildings) {
        const bool r = t == b->type();
        if(r) return true;
    }
    return false;
}

int eGameBoard::countAllowed(const eBuildingType t) const {
    eBuildingType parent;

    if(t == eBuildingType::sheep) {
        parent = eBuildingType::cardingShed;
    } else if(t == eBuildingType::goat) {
        parent = eBuildingType::dairy;
    } else if(t == eBuildingType::cattle) {
        parent = eBuildingType::corral;
    } else {
        return 0;
    }
    const int already = countBuildings(t);

    return 8*countBuildings(parent) - already;
}

eBuilding* eGameBoard::randomBuilding(const eBuildingValidator& v) const {
    auto blds = mTimedBuildings;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(blds.begin(), blds.end(), g);
    for (const auto b : blds) {
        const bool r = v(b);
        if (r) return b;
    }
    return nullptr;
}

int eGameBoard::horses() const {
    int h = 0;
    for(const auto b : mTimedBuildings) {
        if(b->type() != eBuildingType::horseRanch) continue;
        const auto r = static_cast<eHorseRanch*>(b);
        h += r->horseCount();
    }
    return h;
}

void eGameBoard::planAction(ePlannedAction* const a) {
    mPlannedActions.emplace_back(a);
}

void eGameBoard::restockMarbleTiles() {
    for(const auto& mt : mMarbleTiles) {
        mt.restock();
    }
}

bool eGameBoard::eMarbleTiles::contains(eTile* const tile) const {
    return eVectorHelpers::contains(fTiles, tile);
}

void eGameBoard::eMarbleTiles::add(eTile* const tile) {
    fTiles.push_back(tile);
}

void eGameBoard::eMarbleTiles::addWithNeighbours(eTile* const tile) {
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            const auto n = tile->tileRel<eTile>(x, y);
            const auto terr = n->terrain();
            if(terr != eTerrain::marble) return;
            const bool c = contains(n);
            if(c) continue;
            add(n);
            addWithNeighbours(n);
        }
    }
}

void eGameBoard::eMarbleTiles::restock() const {
    int maxLevel = 0;
    for(const auto t : fTiles) {
        if(t->resource() > 0) return;
        const int l = t->marbleLevel();
        if(l > maxLevel) maxLevel = l;
    }
    for(const auto t : fTiles) {
        const int l = t->marbleLevel();
        if(l == maxLevel) {
            if(maxLevel == 2) {
                t->setResource(99999);
            } else {
                const bool e = eMarbleTile::edge(t);
                if(!e) t->setResource(1);
            }
        }
    }
}

void eGameBoard::updateMarbleTiles() {
    mMarbleTiles.clear();
    iterateOverAllTiles([&](eTile* const t) {
        const auto terr = t->terrain();
        if(terr != eTerrain::marble) return;
        for(const auto& mt : mMarbleTiles) {
            const bool c = mt.contains(t);
            if(c) return;
        }
        auto& mt = mMarbleTiles.emplace_back();
        mt.addWithNeighbours(t);
    });
}

void eGameBoard::setFriendlyGods(const std::vector<eGodType>& gods) {
    mFriendlyGods = gods;

    for(const auto g : gods) {
        eBuildingType bt;
        switch(g) {
        case eGodType::aphrodite:
            bt = eBuildingType::templeAphrodite;
            break;
        case eGodType::apollo:
            bt = eBuildingType::templeApollo;
            break;
        case eGodType::ares:
            bt = eBuildingType::templeAres;
            break;
        case eGodType::artemis:
            bt = eBuildingType::templeArtemis;
            break;
        case eGodType::athena:
            bt = eBuildingType::templeAthena;
            break;
        case eGodType::atlas:
            bt = eBuildingType::templeAtlas;
            break;
        case eGodType::demeter:
            bt = eBuildingType::templeDemeter;
            break;
        case eGodType::dionysus:
            bt = eBuildingType::templeDionysus;
            break;
        case eGodType::hades:
            bt = eBuildingType::templeHades;
            break;
        case eGodType::hephaestus:
            bt = eBuildingType::templeHephaestus;
            break;
        case eGodType::hera:
            bt = eBuildingType::templeHera;
            break;
        case eGodType::hermes:
            bt = eBuildingType::templeHermes;
            break;
        case eGodType::poseidon:
            bt = eBuildingType::templePoseidon;
            break;
        case eGodType::zeus:
            bt = eBuildingType::templeZeus;
            break;
        default:
            bt = eBuildingType::none;
            break;
        }

        allow(bt);
    }

    const auto e = e::make_shared<eGodVisitEvent>(
                       eGameEventBranch::root);
    e->setIsEpisodeEvent(true);
    e->setGameBoard(this);
    eDate date = mDate;
    const int period = eNumbers::sFriendlyGodVisitPeriod;
    date += period;
    e->initializeDate(date, period, 10000);
    e->setTypes(gods);
    addRootGameEvent(e);
}

void eGameBoard::setHostileGods(const std::vector<eGodType>& gods) {
    mHostileGods = gods;

    const auto e = e::make_shared<eGodAttackEvent>(
                       eGameEventBranch::root);
    e->setIsEpisodeEvent(true);
    e->setGameBoard(this);
    eDate date = mDate;
    const int period = 900;
    date += period;
    e->initializeDate(date, period, 10000);
    e->setTypes(gods);
    addRootGameEvent(e);
}

void eGameBoard::setHostileMonsters(const std::vector<eMonsterType>& monsters) {
    mHostileMonsters = monsters;

//    const auto e = e::make_shared<eMonsterAttackEvent>(*this);
//    e->setTypes(monsters);
//    const int period = 150;
//    const auto date = mDate + period;
//    const auto ec = e::make_shared<eGameEventCycle>(
//                        e, date, period, 2, *this);
    //    addGameEvent(ec);
}

void eGameBoard::allowHero(const eHeroType heroType,
                           const std::string& reason) {
    const auto hallType = eHerosHall::sHeroTypeToHallType(heroType);
    allow(hallType);
    const auto& inst = eMessages::instance;
    const auto hm = inst.heroMessages(heroType);
    if(!hm) return;
    eEventData ed;
    auto msg = hm->fHallAvailable;
    eStringHelpers::replaceAll(msg.fFull.fText, "[reason_phrase]",
                               reason.empty() ? msg.fNoReason : reason);
    showMessage(ed, msg);
}

void eGameBoard::planInvasion(const eDate& date,
                              const int infantry,
                              const int cavalry,
                              const int archers) {
    const auto e = e::make_shared<eInvasionEvent>(
                       eGameEventBranch::root);
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    const auto city = mWorldBoard->cities().front();
    e->initialize(city, infantry, cavalry, archers);
    e->initializeDate(date, 0, 1);
    addRootGameEvent(e);
}

eBuilding* eGameBoard::buildingWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto b : mAllBuildings) {
        const int bio = b->ioID();
        if(bio == id) return b;
    }
    return nullptr;
}

eCharacter* eGameBoard::characterWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto c : mCharacters) {
        const int bio = c->ioID();
        if(bio == id) return c;
    }
    return nullptr;
}

eCharacterAction* eGameBoard::characterActionWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto ca : mCharacterActions) {
        const int bio = ca->ioID();
        if(bio == id) return ca;
    }
    return nullptr;
}

eBanner* eGameBoard::bannerWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto b : mBanners) {
        const int bio = b->ioID();
        if(bio == id) return b;
    }
    return nullptr;
}

eSoldierBanner* eGameBoard::soldierBannerWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto b : mAllSoldierBanners) {
        const int bio = b->ioID();
        if(bio == id) return b;
    }
    return nullptr;
}

eGameEvent* eGameBoard::eventWithIOID(const int id) const {
    if(id == -1) return nullptr;
    for(const auto e : mAllGameEvents) {
        const int eio = e->ioID();
        if(eio == id) return e;
    }
    return nullptr;
}

eTile* eGameBoard::monsterTile(const int id) const {
    const auto b = banner(eBannerTypeS::monsterPoint, id);
    return b ? b->tile() : nullptr;
}

eTile* eGameBoard::landInvasionTile(const int id) const {
    const auto b = banner(eBannerTypeS::landInvasion, id);
    return b ? b->tile() : nullptr;
}

eTile* eGameBoard::disasterTile(const int id) const {
    const auto b = banner(eBannerTypeS::disasterPoint, id);
    return b ? b->tile() : nullptr;
}

const eGameBoard::eIV& eGameBoard::invasionHandlers() const {
    return mInvasionHandlers;
}

void eGameBoard::addInvasionHandler(eInvasionHandler* const i) {
    mInvasionHandlers.push_back(i);
    updateMusic();
}

void eGameBoard::removeInvasionHandler(eInvasionHandler* const i) {
    eVectorHelpers::remove(mInvasionHandlers, i);
    updateMusic();
}

bool eGameBoard::hasActiveInvasions() const {
    return !mInvasionHandlers.empty();
}

int eGameBoard::addResource(const eResourceType type,
                            const int count) {
    if(type == eResourceType::drachmas) {
        incDrachmas(count);
        return count;
    }
    int rem = count;
    using eValidator = std::function<bool(eStorageBuilding*)>;
    const auto addFunc = [&](const eValidator& v) {
        if(rem <= 0) return;
        for(const auto s : mStorBuildings) {
            if(!v(s)) continue;
            const int c = s->add(type, rem);
            rem -= c;
            if(rem <= 0) break;
        }
    };
    addFunc([&](eStorageBuilding* const s) {
        return s->get(type);
    });
    addFunc([&](eStorageBuilding* const s) {
        (void)s;
        return true;
    });
    return count - rem;
}

int eGameBoard::spaceForResource(const eResourceType type) const {
    if(type == eResourceType::drachmas) {
        return __INT_MAX__/2;
    }
    int r = 0;
    for(const auto s : mStorBuildings) {
        r += s->spaceLeft(type);
    }
    return r;
}

int eGameBoard::maxSingleSpaceForResource(
        const eResourceType type,
        eStorageBuilding** b) const {
    *b = nullptr;
    int r = 0;
    for(const auto s : mStorBuildings) {
        const int ss = s->spaceLeft(type);
        if(ss > r) {
            *b = s;
            r = ss;
        }
    }
    return r;
}

int eGameBoard::maxSanctuarySpaceForResource(
        eSanctuary** b) const {
    *b = nullptr;
    int r = 0;
    for(const auto s : mSanctuaries) {
        int ss = s->spaceLeft(eResourceType::wood);
        ss += s->spaceLeft(eResourceType::marble);
        ss += s->spaceLeft(eResourceType::sculpture);
        if(ss > r) {
            *b = s;
            r = ss;
        }
    }
    return r;
}

void eGameBoard::planGiftFrom(const stdsptr<eWorldCity>& c,
                              const eResourceType type,
                              const int count,
                              const int delay) {
    const auto e = e::make_shared<eGiftFromEvent>(
                       eGameEventBranch::root);
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    e->initialize(true, type, count, c);
    const auto date = mDate + delay;
    e->initializeDate(date);
    addRootGameEvent(e);
}

void eGameBoard::request(const stdsptr<eWorldCity>& c,
                         const eResourceType type) {
    const auto e = e::make_shared<eMakeRequestEvent>(
                       eGameEventBranch::root);
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    e->initialize(true, type, c);
    const auto date = mDate + 90;
    e->initializeDate(date);
    addRootGameEvent(e);
}

void eGameBoard::requestAid(const stdsptr<eWorldCity>& c) {
    const auto e = e::make_shared<eRequestAidEvent>(
                       eGameEventBranch::root);
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    e->setCity(c);
    const auto date = mDate + 30;
    e->initializeDate(date);
    addRootGameEvent(e);
}

void eGameBoard::tributeFrom(const stdsptr<eWorldCity>& c,
                             const bool postpone) {
    const auto type = c->tributeType();
    const auto count = c->tributeCount();

    eEventData ed;
    ed.fType = eMessageEventType::requestTributeGranted;
    ed.fCity = c;
    ed.fSpaceCount = spaceForResource(type);
    ed.fResourceType = c->tributeType();
    ed.fResourceCount = c->tributeCount();
    ed.fA0 = [this, c, type, count]() { // accept
        const int a = addResource(type, count);
        if(a == count) return;
        eEventData ed;
        ed.fType = eMessageEventType::resourceGranted;
        ed.fCity = c;
        ed.fResourceType = type;
        ed.fResourceCount = a;
        event(eEvent::tributeAccepted, ed);
    };
    if(postpone) {
        ed.fA1 = [this, c, type, count]() { // postpone
            eEventData ed;
            ed.fType = eMessageEventType::resourceGranted;
            ed.fCity = c;
            ed.fResourceType = type;
            ed.fResourceCount = count;
            event(eEvent::tributePostponed, ed);

            const auto e = e::make_shared<ePayTributeEvent>(
                               eGameEventBranch::root);
            e->setGameBoard(this);
            e->setWorldBoard(mWorldBoard);
            e->initialize(c);
            const auto date = mDate + 31;
            e->initializeDate(date);
            addRootGameEvent(e);
        };
    }
    ed.fA2 = [this, c, type, count]() { // decline
        eEventData ed;
        ed.fType = eMessageEventType::resourceGranted;
        ed.fCity = c;
        ed.fResourceType = type;
        ed.fResourceCount = count;
        event(eEvent::tributeDeclined, ed);
    };
    event(eEvent::tributePaid, ed);
}

void eGameBoard::giftTo(const stdsptr<eWorldCity>& c,
                        const eResourceType type,
                        const int count) {
    takeResource(type, count);
    const auto e = e::make_shared<eGiftToEvent>(
                       eGameEventBranch::root);
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    e->initialize(c, type, count);
    const auto date = mDate + 90;
    e->initializeDate(date);
    addRootGameEvent(e);
}

void eGameBoard::giftToReceived(const stdsptr<eWorldCity>& c,
                                const eResourceType type,
                                const int count) {
    const bool a = c->acceptsGift(type, count);
    eEventData ed;
    ed.fType = eMessageEventType::resourceGranted;
    ed.fCity = c;
    ed.fResourceType = type;
    ed.fResourceCount = count;
    if(a) {
        const bool b = c->buys(type);
        const bool s = c->sells(type);
        if(type == eResourceType::drachmas) {
            event(eEvent::giftReceivedDrachmas, ed);
            c->incAttitude(10);
        } else if(b) {
            event(eEvent::giftReceivedNeeded, ed);
            c->incAttitude(10);
        } else if(s) {
            event(eEvent::giftReceivedSells, ed);
            c->incAttitude(5);
        } else {
            event(eEvent::giftReceivedNotNeeded, ed);
            c->incAttitude(5);
        }
        c->gifted(type, count);
    } else {
        event(eEvent::giftReceivedRefuse, ed);
    }
}

void eGameBoard::waitUntilFinished() {
    while(!mThreadPool.finished()) {
        mThreadPool.waitFinished();
        mThreadPool.handleFinished();
    }
}

void eGameBoard::consolidateSoldiers() {
    using eSoldierBanners = std::vector<stdsptr<eSoldierBanner>>;
    eSoldierBanners rabble;
    eSoldierBanners hoplites;
    eSoldierBanners horsemen;
    for(const auto& s : mPalaceSoldierBanners) {
        if(s->isAbroad()) continue;
        const auto tile = s->tile();
        if(!tile) s->moveToDefault();
        if(!s->isHome()) continue;
        switch(s->type()) {
        case eBannerType::rockThrower:
            rabble.push_back(s);
            break;
        case eBannerType::hoplite:
            hoplites.push_back(s);
            break;
        case eBannerType::horseman:
            horsemen.push_back(s);
            break;
        default:
            break;
        }
    }
    const auto consolidator = [](const eSoldierBanners& banners) {
        for(int i = 0; i < static_cast<int>(banners.size()); i++) {
            const auto s = banners[i];
            const int sc = s->count();
            int sSpace = 8 - sc;
            if(sSpace <= 0) continue;
            for(int j = banners.size() - 1; j > i; j--) {
                const auto ss = banners[j];
                const int ssc = ss->count();
                const int kMax = std::min(sSpace, ssc);
                for(int k = 0; k < kMax; k++) {
                    ss->decCount();
                    s->incCount();
                    sSpace--;
                    if(sSpace <= 0) break;
                }
                if(sSpace <= 0) break;
            }
        }
    };
    consolidator(rabble);
    consolidator(hoplites);
    consolidator(horsemen);
}

bool eGameBoard::wasHeroSummoned(const eHeroType hero) const {
    return eVectorHelpers::contains(mSummonedHeroes, hero);
}

void eGameBoard::heroSummoned(const eHeroType hero) {
    mSummonedHeroes.push_back(hero);
}

int eGameBoard::price(const eResourceType type) const {
    const auto it = mPrices.find(type);
    if(it == mPrices.end()) return 0;
    return it->second;
}

void eGameBoard::incPrice(const eResourceType type, const int by) {
    mPrices[type] += by;
}

void eGameBoard::changeWage(const int per) {
    const double mult = 1 + per/100.;
    mWageMultiplier *= mult;
}

void eGameBoard::updateMaxSoldiers() {
    mMaxRabble = 0;
    mMaxHoplites = 0;
    mMaxHorsemen = 0;
    if(!palace()) return;
    for(const auto b : mTimedBuildings) {
        const auto bt = b->type();
        if(bt == eBuildingType::commonHouse) {
            const auto ch = static_cast<eSmallHouse*>(b);
            const int l = ch->level();
            if(l < 2) continue;
            int lvlMax = 0;
            if(l == 2) lvlMax = 5;
            else if(l == 3) lvlMax = 6;
            else if(l == 4) lvlMax = 10;
            else if(l == 5) lvlMax = 12;
            else if(l == 6) lvlMax = 15;
            const int pop = ch->people();
            const int popMax = pop/4;
            mMaxRabble += std::min(lvlMax, popMax);
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
    mMaxRabble /= 6;

    const int nSpaces = 20;
    mMaxHorsemen = std::min(8*nSpaces, mMaxHorsemen);
    mMaxHorsemen = std::max(0, mMaxHorsemen);
    const int nHorsemenB = std::ceil(mMaxHorsemen/8.);
    mMaxHoplites = std::min(8*nSpaces - 8*nHorsemenB, mMaxHoplites);
    mMaxHoplites = std::max(0, mMaxHoplites);
    const int nHoplitesB = std::ceil(mMaxHoplites/8.);
    mMaxRabble = std::min(8*nSpaces - 8*nHorsemenB - 8*nHoplitesB, mMaxRabble);
    mMaxRabble = std::max(0, mMaxRabble);
}

void eGameBoard::addSoldier(const eCharacterType st) {
    bool found = false;
    for(const auto& b : mPalaceSoldierBanners) {
        if(b->isAbroad()) continue;
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
                  st == eCharacterType::horseman) {
            found = true;
        }
        if(found) {
            b->incCount();
            break;
        }
    }
    if(found) return;
    const int nSpaces = 20;
    if(mPalaceSoldierBanners.size() >= nSpaces) return;
    eBannerType bt;
    if(st == eCharacterType::rockThrower) {
        bt = eBannerType::rockThrower;
    } else if(st == eCharacterType::hoplite) {
        bt = eBannerType::hoplite;
    } else if(st == eCharacterType::horseman) {
        bt = eBannerType::horseman;
    } else {
        return;
    }
    const auto b = e::make_shared<eSoldierBanner>(bt, *this);
    b->setPlayerId(1);
    registerSoldierBanner(b);
    b->incCount();
    b->moveToDefault();
}

void eGameBoard::removeSoldier(const eCharacterType st,
                               const bool skipNotHome) {
    for(const auto& b : mPalaceSoldierBanners) {
        if(b->isAbroad()) continue;
        if(skipNotHome && !b->isHome()) continue;
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
            return;
        }
    }
    if(skipNotHome) removeSoldier(st, false);
}

void eGameBoard::distributeSoldiers() {
    int cRabble = 0;
    int cHoplites = 0;
    int cHorsemen = 0;
    for(const auto& b : mPalaceSoldierBanners) {
        const auto bt = b->type();
        const int c = b->count();
        if(bt == eBannerType::rockThrower) {
            cRabble += c;
        } else if(bt == eBannerType::hoplite) {
            cHoplites += c;
        } else if(bt == eBannerType::horseman) {
            cHorsemen += c;
        }
    }
    const int remRabble = mMaxRabble - cRabble;
    const int remHoplites = mMaxHoplites - cHoplites;
    const int remHorsemen = mMaxHorsemen - cHorsemen;

    for(int i = 0; i < -remRabble; i++) {
        removeSoldier(eCharacterType::rockThrower);
    }
    for(int i = 0; i < -remHoplites; i++) {
        removeSoldier(eCharacterType::hoplite);
    }
    for(int i = 0; i < -remHorsemen; i++) {
        removeSoldier(eCharacterType::horseman);
    }
    for(int i = 0; i < remHorsemen; i++) {
        addSoldier(eCharacterType::horseman);
    }
    for(int i = 0; i < remHoplites; i++) {
        addSoldier(eCharacterType::hoplite);
    }
    for(int i = 0; i < remRabble; i++) {
        addSoldier(eCharacterType::rockThrower);
    }
}

void eGameBoard::killCommonFolks(int toKill) {
    auto bs = mTimedBuildings;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bs.begin(), bs.end(), g);
    for (const auto b : bs) {
        const auto bt = b->type();
        if (bt == eBuildingType::commonHouse) {
            const auto sh = static_cast<eSmallHouse*>(b);
            const int pop = sh->people();
            const int shk = std::min(toKill, pop);
            toKill -= shk;
            sh->kill(shk);
            if (toKill <= 0) break;
        }
    }
}
void eGameBoard::walkerKilled() {
    killCommonFolks(5);
}

void eGameBoard::rockThrowerKilled() {
    killCommonFolks(4);
}

void eGameBoard::hopliteKilled() {
    auto bs = mTimedBuildings;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bs.begin(), bs.end(), g);
    for (const auto b : bs) {
        const auto bt = b->type();
        if (bt == eBuildingType::eliteHousing) {
            const auto eh = static_cast<eEliteHousing*>(b);
            const int pop = eh->people();
            const int shk = std::min(4, pop);
            eh->kill(shk);
            eh->removeArmor();
            return;
        }
    }
}

void eGameBoard::horsemanKilled() {
    auto bs = mTimedBuildings;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(bs.begin(), bs.end(), g);
    for (const auto b : bs) {
        const auto bt = b->type();
        if (bt == eBuildingType::eliteHousing) {
            const auto eh = static_cast<eEliteHousing*>(b);
            const int pop = eh->people();
            const int shk = std::min(4, pop);
            eh->kill(shk);
            eh->removeArmor();
            eh->removeHorse();
            return;
        }
    }
}

eEnlistedForces eGameBoard::getEnlistableForces() const {
    eEnlistedForces result;

    for(const auto& s : mSoldierBanners) {
        if(s->count() <= 0) continue;
        switch(s->type()) {
        case eBannerType::hoplite:
        case eBannerType::horseman:
        case eBannerType::amazon:
        case eBannerType::aresWarrior:
            result.fSoldiers.push_back(s);
            break;
        default:
            break;
        }
    }

    for(const auto& h : mHeroHalls) {
        const auto s = h->stage();
        if(s != eHeroSummoningStage::arrived) continue;
        const auto ht = h->heroType();
        result.fHeroes.push_back(ht);
    }

    const auto& cts = mWorldBoard->cities();
    for(const auto& c : cts) {
        if(!c->active()) continue;
        const auto type = c->type();
        const auto rel = c->relationship();
        const bool e = type == eCityType::colony ||
                       (type == eCityType::foreignCity &&
                        (rel == eForeignCityRelationship::ally ||
                         rel == eForeignCityRelationship::vassal));
        if(e && c->attitude() > 50) {
            result.fAllies.push_back(c);
        }
    }

    return result;
}

void eGameBoard::addGodQuest(eGodQuestEvent* const q) {
    eVectorHelpers::remove(mGodQuests, q);
    mGodQuests.push_back(q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::removeGodQuest(eGodQuestEvent* const q) {
    eVectorHelpers::remove(mGodQuests, q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::addCityRequest(eReceiveRequestEvent* const q) {
    mCityRequests.push_back(q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::removeCityRequest(eReceiveRequestEvent* const q) {
    eVectorHelpers::remove(mCityRequests, q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::addCityTroopsRequest(eTroopsRequestEvent* const q) {
    mCityTroopsRequests.push_back(q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::removeCityTroopsRequest(eTroopsRequestEvent* const q) {
    eVectorHelpers::remove(mCityTroopsRequests, q);
    if(mRequestUpdateHandler) mRequestUpdateHandler();
}

void eGameBoard::addConquest(ePlayerConquestEventBase* const q) {
    mConquests.push_back(q);
}

void eGameBoard::removeConquest(ePlayerConquestEventBase* const q) {
    eVectorHelpers::remove(mConquests, q);
}

eInvasionEvent* eGameBoard::invasionToDefend() const {
    const auto date = eGameBoard::date();
    for(const auto i : mInvasions) {
        const int ip = i->invasionPoint();
        const auto t = landInvasionTile(ip);
        if(!t) continue;
        const auto sDate = i->nextDate();
        if(sDate - date < 120) {
            return i;
        }
    }
    return nullptr;
}

void eGameBoard::addInvasion(eInvasionEvent* const i) {
    mInvasions.push_back(i);
}

void eGameBoard::removeInvasion(eInvasionEvent* const i) {
    eVectorHelpers::remove(mInvasions, i);
}

void eGameBoard::addArmyEvent(eArmyEventBase* const q) {
    mArmyEvents.push_back(q);
}

void eGameBoard::removeArmyEvent(eArmyEventBase* const q) {
    eVectorHelpers::remove(mArmyEvents, q);
}

void eGameBoard::registerAttackingGod(eCharacter* const c) {
    mAttackingGods.push_back(c);
    updateMusic();
}

void eGameBoard::startPlague(eSmallHouse* const h) {
    const auto plague = std::make_shared<ePlague>(*this);
    plague->spreadFrom(h);
    mPlagues.push_back(plague);
    eEventData ed;
    ed.fTile = h->centerTile();
    event(eEvent::plague, ed);
}

stdsptr<ePlague> eGameBoard::plagueForHouse(eSmallHouse* const h) {
    for(const auto& p : mPlagues) {
        if(p->hasHouse(h)) return p;
    }
    return nullptr;
}

void eGameBoard::healPlague(const stdsptr<ePlague>& p) {
    p->healAll();
    eVectorHelpers::remove(mPlagues, p);
}

void eGameBoard::healHouse(eSmallHouse* const h) {
    const auto p = plagueForHouse(h);
    if(p) {
        if(p->houseCount() == 1) healPlague(p);
        else p->healHouse(h);
    } else {
        h->setPlague(false);
    }
}

stdsptr<ePlague> eGameBoard::nearestPlague(
        const int tx, const int ty, int& dist) const {
    dist = __INT_MAX__/2;
    stdsptr<ePlague> result;
    for(const auto& p : mPlagues) {
        const auto& hs = p->houses();
        for(const auto h : hs) {
            const auto tt = h->centerTile();
            const int ttx = tt->x();
            const int tty = tt->y();
            const int d = sqrt((ttx - tx)*(ttx - tx) +
                               (tty - ty)*(tty - ty));
            if(d < dist) {
                dist = d;
                result = p;
            }
        }
    }
    return result;
}

void eGameBoard::updateMusic() {
    bool monsterActiveAttack = false;
    for(const auto m : mMonsters) {
        const auto a = m->action();
        if(const auto ma = dynamic_cast<eMonsterAction*>(a)) {
            const auto stage = ma->stage();
            if(stage == eMonsterAttackStage::none ||
               stage == eMonsterAttackStage::wait) {
                continue;
            }
            monsterActiveAttack = true;
            break;
        }
    }
    if(!monsterActiveAttack &&
       mInvasionHandlers.empty() &&
       mAttackingGods.empty()) {
        eMusic::playRandomMusic();
    } else {
        eMusic::playRandomBattleMusic();
    }
}

eTile* eGameBoard::entryPoint() const {
    for(const auto b : mBanners) {
        const auto type = b->type();
        if(type == eBannerTypeS::entryPoint) {
            return b->tile();
        }
    }
    return nullptr;
}

eTile* eGameBoard::exitPoint() const {
    for(const auto b : mBanners) {
        const auto type = b->type();
        if(type == eBannerTypeS::exitPoint) {
            return b->tile();
        }
    }
    return nullptr;
}

void eGameBoard::setPoseidonMode(const bool p) {
    mPoseidonMode = p;
    scheduleTerrainUpdate();
}

void eGameBoard::setWorldBoard(eWorldBoard* const wb) {
    mWorldBoard = wb;
    for(const auto& e : mGameEvents) {
        e->setWorldBoard(wb);
    }
}

void eGameBoard::registerSoldierBanner(const stdsptr<eSoldierBanner>& b) {
    if(b->militaryAid()) return;
    b->setRegistered(true);
    mSoldierBanners.push_back(b);
    switch(b->type()) {
    case eBannerType::rockThrower:
    case eBannerType::hoplite:
    case eBannerType::horseman:
        mPalaceSoldierBanners.push_back(b);
        break;
    default:
        break;
    }
}

bool eGameBoard::unregisterSoldierBanner(const stdsptr<eSoldierBanner>& b) {
    b->setRegistered(false);
    eVectorHelpers::remove(mSelectedBanners, b.get());
    eVectorHelpers::remove(mPalaceSoldierBanners, b);
    return eVectorHelpers::remove(mSoldierBanners, b);
}

void eGameBoard::addRootGameEvent(const stdsptr<eGameEvent>& e) {
    e->setGameBoard(this);
    e->setWorldBoard(mWorldBoard);
    mGameEvents.push_back(e);
}

void eGameBoard::removeRootGameEvent(const stdsptr<eGameEvent>& e) {
    eVectorHelpers::remove(mGameEvents, e);
}

void eGameBoard::addGameEvent(eGameEvent* const e) {
    mAllGameEvents.push_back(e);
}

void eGameBoard::removeGameEvent(eGameEvent* const e) {
    eVectorHelpers::remove(mAllGameEvents, e);
}

void eGameBoard::updateCoverage() {
    int totalPeople = 0;
    int commonPeople = 0;
    int sport = 0;
    int phil = 0;
    int drama = 0;
    int taxes = 0;
    double totalUnrest = 0;
    int totalSatisfaction = 0;
    int totalHygiene = 0;
    for(const auto b : mTimedBuildings) {
        if(const auto h = dynamic_cast<eHouseBase*>(b)) {
            const int p = h->people();
            if(h->athletesScholars() > 0) {
                sport += p;
            }
            if(h->philosophersInventors() > 0) {
                phil += p;
            }
            if(h->actorsAstronomers() > 0) {
                drama += p;
            }
            if(h->paidTaxes()) {
                taxes += p;
            }
            if(const auto ch = dynamic_cast<eSmallHouse*>(b)) {
                if(ch->disgruntled()) totalUnrest += p;
                totalSatisfaction += p*ch->satisfaction();
                totalHygiene += p*ch->hygiene();
                commonPeople += p;
            }
            totalPeople += p;
        }
    }
    if(totalPeople <= 0) {
        mAthleticsCoverage = 0;
        mPhilosophyCoverage = 0;
        mDramaCoverage = 0;
        mTaxesCoverage = 0;
    } else {
        mAthleticsCoverage = std::round(100.*sport/totalPeople);
        mPhilosophyCoverage = std::round(100.*phil/totalPeople);
        mDramaCoverage = std::round(100.*drama/totalPeople);
        mTaxesCoverage = std::round(100.*taxes/totalPeople);
    }
    mAllDiscCoverage = (mAthleticsCoverage + mPhilosophyCoverage + mDramaCoverage)/3;

    if(commonPeople == 0) {
        mUnrest = 0;
        mPopularity = 100;
        mHealth = 100;
    } else {
        mUnrest = std::round(100.*totalUnrest/commonPeople);
        mPopularity = std::round(1.*totalSatisfaction/commonPeople);
        mHealth = std::round(1.*totalHygiene/commonPeople);
    }
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

    eEventData ed;
    if(mult*coverage > 15) {
        showMessage(ed, msgs->fBegin);
    } else {
        showMessage(ed, msgs->fNoPart);
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
        const bool won = eRand::rand() % 101 < 100*chance;

        eEventData ed;
        if(won) {
            showMessage(ed, msgs->fWon);
            mWonGames++;
            int id = 0;
            switch(game) {
            case eGames::isthmian:
                id = 8;
                break;
            case eGames::nemean:
                id = 3;
                break;
            case eGames::pythian:
                id = 8;
                break;
            case eGames::olympian:
                id = 8;
                break;
            }
            allow(eBuildingType::commemorative, id);
        } else {
            const bool second = eRand::rand() % 101 < 200*chance;
            if(second) {
                showMessage(ed, msgs->fSecond);
            } else {
                showMessage(ed, msgs->fLost);
            }
        }
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
    const bool r = eVectorHelpers::remove(mAttackingGods, c);
    if(r) updateMusic();
    return eVectorHelpers::remove(mCharacters, c);
}

void eGameBoard::registerCharacterAction(eCharacterAction* const ca) {
    mCharacterActions.push_back(ca);
}

bool eGameBoard::unregisterCharacterAction(eCharacterAction* const ca) {
    return eVectorHelpers::remove(mCharacterActions, ca);
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
    const auto bt = b->type();
    if(eBuilding::sTimedBuilding(bt)) {
        mTimedBuildings.push_back(b);
    }
    if(bt == eBuildingType::commemorative ||
       bt == eBuildingType::godMonument) {
        mCommemorativeBuildings.push_back(b);
    }
    scheduleAppealMapUpdate();
}

bool eGameBoard::unregisterBuilding(eBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    eVectorHelpers::remove(mAllBuildings, b);
    eVectorHelpers::remove(mTimedBuildings, b);
    eVectorHelpers::remove(mCommemorativeBuildings, b);
    scheduleAppealMapUpdate();
    return true;
}

bool eGameBoard::unregisterCommonHouse(eSmallHouse* const ch) {
    const auto p = plagueForHouse(ch);
    if(p) {
        p->removeHouse(ch);
        const int c = p->houseCount();
        if(c <= 0) healPlague(p);
    }
    return true;
}

void eGameBoard::registerEmplBuilding(eEmployingBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mEmployingBuildings.push_back(b);
    const auto type = b->type();
    const bool sd = isShutDown(type);
    eSector s;
    const bool r = eSectorHelpers::sBuildingSector(type, s);
    if(r) {
        mSectorBuildings[s].push_back(b);
    }
    if(!sd) {
        const int me = b->maxEmployees();
        mEmplData.incTotalJobVacancies(me);
        if(r) mEmplDistributor.incMaxEmployees(s, me);
    }
    distributeEmployees();
}

bool eGameBoard::unregisterEmplBuilding(eEmployingBuilding* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    const bool rr = eVectorHelpers::remove(mEmployingBuildings, b);
    const auto type = b->type();
    eSector s;
    const bool r = eSectorHelpers::sBuildingSector(type, s);
    if(r) {
        eVectorHelpers::remove(mSectorBuildings[s], b);
    }
    if(rr) {
        const bool sd = isShutDown(type);
        if(!sd) {
            const int me = b->maxEmployees();
            mEmplData.incTotalJobVacancies(-me);
            if(r) mEmplDistributor.incMaxEmployees(s, -me);
        }
    }
    distributeEmployees();
    return rr;
}

void eGameBoard::registerTradePost(eTradePost* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mTradePosts.push_back(b);
    if(mButtonVisUpdater) mButtonVisUpdater();
}

bool eGameBoard::unregisterTradePost(eTradePost* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    const bool r = eVectorHelpers::remove(mTradePosts, b);
    if(r && mButtonVisUpdater) mButtonVisUpdater();
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

void eGameBoard::registerMuseum(eBuilding* const s) {
    if(!mRegisterBuildingsEnabled) return;
    mMuseum = s;
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::unregisterMuseum() {
    if(!mRegisterBuildingsEnabled) return;
    mMuseum = nullptr;
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

void eGameBoard::registerSanctuary(eSanctuary* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mSanctuaries.push_back(b);
}

bool eGameBoard::unregisterSanctuary(eSanctuary* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    return eVectorHelpers::remove(mSanctuaries, b);
    return true;
}

void eGameBoard::registerHeroHall(eHerosHall* const b) {
    if(!mRegisterBuildingsEnabled) return;
    mHeroHalls.push_back(b);
}

bool eGameBoard::unregisterHeroHall(eHerosHall* const b) {
    if(!mRegisterBuildingsEnabled) return false;
    return eVectorHelpers::remove(mHeroHalls, b);
    return true;
}

void eGameBoard::registerMissile(eMissile* const m) {
    mMissiles.push_back(m);
}

bool eGameBoard::unregisterMissile(eMissile* const m) {
    return eVectorHelpers::remove(mMissiles, m);
}

eStadium* eGameBoard::stadium() const {
    return static_cast<eStadium*>(mStadium);
}

eMuseum* eGameBoard::museum() const {
    return static_cast<eMuseum*>(mMuseum);
}

void eGameBoard::registerPalace(ePalace* const p) {
    if(!mRegisterBuildingsEnabled) return;
    mPalace = p;
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::unregisterPalace() {
    if(!mRegisterBuildingsEnabled) return;
    mPalace = nullptr;
    updateMaxSoldiers();
    distributeSoldiers();
    consolidateSoldiers();
    if(mButtonVisUpdater) mButtonVisUpdater();
}

void eGameBoard::registerMonster(eMonster* const m) {
    mMonsters.push_back(m);
}

void eGameBoard::unregisterMonster(eMonster* const m) {
    eVectorHelpers::remove(mMonsters, m);
    updateMusic();
}

eBanner* eGameBoard::banner(const eBannerTypeS type, const int id) const {
    for(const auto b : mBanners) {
        const int bid = b->id();
        if(bid != id) continue;
        const auto btype = b->type();
        if(btype != type) continue;
        return b;
    }
    return nullptr;
}

void eGameBoard::registerBanner(eBanner* const b) {
    const int id = b->id();
    const auto type = b->type();
    const auto bb = banner(type, id);
    const auto t = bb ? bb->tile() : nullptr;
    if(t) t->setBanner(nullptr);
    mBanners.push_back(b);
}

void eGameBoard::unregisterBanner(eBanner* const b) {
    eVectorHelpers::remove(mBanners, b);
}

void eGameBoard::registerAllSoldierBanner(eSoldierBanner* const b) {
    mAllSoldierBanners.push_back(b);
}

void eGameBoard::unregisterAllSoldierBanner(eSoldierBanner* const b) {
    eVectorHelpers::remove(mAllSoldierBanners, b);
}

std::vector<eAgoraBase*> eGameBoard::agoras() const {
    std::vector<eAgoraBase*> r;
    for(const auto b : mAllBuildings) {
        const auto a = dynamic_cast<eAgoraBase*>(b);
        if(!a) continue;
        r.push_back(a);
    }
    return r;
}

void eGameBoard::incTime(const int by) {
    if(mEpisodeLost) return;
    const int dayLen = eNumbers::sDayLength;
    { // autosave
        const int time = mTime + by;
        bool nextMonth = false;
        bool nextYear = false;
        const int nd = time/dayLen;
        auto date = mDate;
        date.nextDays(nd, nextMonth, nextYear);
        if(nextYear) {
            if(mAutosaver) mAutosaver();
        }
    }

    const int iMax = mPlannedActions.size() - 1;
    for(int i = iMax; i >= 0; i--) {
        const auto a = mPlannedActions[i];
        a->incTime(by, *this);
        if(a->finished()) {
            mPlannedActions.erase(mPlannedActions.begin() + i);
            delete a;
        }
    }

//    if(mTotalTime == 0) {
////        receiveRequest(mWorldBoard->cities()[0],
////                            eResourceType::fleece, 9, 0);
//        planGiftFrom(mWorldBoard->cities()[0],
//                     eResourceType::fleece, 16);
//    }

//    for(const auto& p : mPlagues) {
//        const int r = eRand::rand() % 5000;
//        const bool spread = r/by == 0;
//        if(spread) p->randomSpread();
//    }

    mProgressEarthquakes += by;
    const int earthquakeWait = eNumbers::sEarthquakeProgressPeriod;
    if(mProgressEarthquakes > earthquakeWait) {
        mProgressEarthquakes = 0;
        progressEarthquakes();
    }

    mSoldiersUpdate += by;
    const int sup = 1000;
    if(mSoldiersUpdate > sup) {
        mSoldiersUpdate -= sup;
        updateMaxSoldiers();
        distributeSoldiers();
        consolidateSoldiers();
    }

    for(const auto i : mInvasionHandlers) {
        i->incTime(by);
    }

    mCoverageUpdate += by;
    const int cup = 2000;
    if(mCoverageUpdate > cup) {
        mCoverageUpdate -= cup;
        updateCoverage();
    }

    const auto& msgs = &eMessages::instance;
    eEventData ed;
    const int pop = mPopData.population();
    if(pop >= 100 && !mPop100) {
        showMessage(ed, msgs->fPop100);
        mPop100 = true;
    } else if(pop >= 500 && !mPop500) {
        showMessage(ed, msgs->fPop500);
        mPop500 = true;
    } else if(pop >= 1000 && !mPop1000) {
        showMessage(ed, msgs->fPop1000);
        allow(eBuildingType::commemorative, 0);
        mPop1000 = true;
    } else if(pop >= 2000 && !mPop2000) {
        showMessage(ed, msgs->fPop2000);
        allow(eBuildingType::commemorative, 0);
        mPop2000 = true;
    } else if(pop >= 3000 && !mPop3000) {
        showMessage(ed, msgs->fPop3000);
        mPop3000 = true;
    } else if(pop >= 5000 && !mPop5000) {
        showMessage(ed, msgs->fPop5000);
        allow(eBuildingType::commemorative, 0);
        mPop5000 = true;
    } else if(pop >= 10000 && !mPop10000) {
        showMessage(ed, msgs->fPop10000);
        allow(eBuildingType::commemorative, 0);
        mPop10000 = true;
    } else if(pop >= 15000 && !mPop15000) {
        showMessage(ed, msgs->fPop15000);
        allow(eBuildingType::commemorative, 0);
        mPop15000 = true;
    } else if(pop >= 20000 && !mPop20000) {
        showMessage(ed, msgs->fPop20000);
        allow(eBuildingType::commemorative, 0);
        mPop20000 = true;
    } else if(pop >= 25000 && !mPop25000) {
        showMessage(ed, msgs->fPop25000);
        allow(eBuildingType::commemorative, 0);
        mPop25000 = true;
    }

    const int updateResWait = 1000;
    mUpdateResources += by;
    if(mUpdateResources > updateResWait) {
        mUpdateResources -= updateResWait;
        updateResources();
    }

    const int food = resourceCount(eResourceType::food);
    bool prolongedNoFood = false;
    if(mNoFood && food < 1 && pop > 10) {
        prolongedNoFood = mDate.year() - mNoFoodSince.year() > 1;
    } else if(food < 1 && pop > 10) {
        mNoFood = true;
        mNoFoodSince = mDate;
    } else {
        mNoFood = false;
    }
    const int u = mEmplData.unemployed();
    const int e = mEmplData.employable();
    const int v = mPopData.vacancies();
    const auto oldLimit = mImmigrationLimit;
    if(prolongedNoFood) {
        mImmigrationLimit = eILB::lackOfFood;
    } else if(mDrachmas < 0 && mDate.year() - mInDebtSince.year() > 1) {
        mImmigrationLimit = eILB::prolongedDebt;
    } else if(static_cast<int>(mWageRate) < static_cast<int>(eWageRate::low)) {
        mImmigrationLimit = eILB::lowWages;
    } else if(e < 10 ? false : (1.*u/e > 0.25 && u > 50)) {
        mImmigrationLimit = eILB::unemployment;
    } else if(static_cast<int>(mTaxRate) > static_cast<int>(eTaxRate::high)) {
        mImmigrationLimit = eILB::highTaxes;
    } else if(mExcessiveMilitaryServiceCount > 0) {
        mImmigrationLimit = eILB::excessiveMilitaryService;
    } else if(v <= 0) {
        mImmigrationLimit = eILB::lackOfVacancies;
    } else {
        mImmigrationLimit = eILB::none;
    }
    if(oldLimit != mImmigrationLimit) {
        switch(mImmigrationLimit) {
        case eILB::lackOfFood:
            showTip(eLanguage::zeusText(19, 112));
            break;
        case eILB::prolongedDebt:
            showTip(eLanguage::zeusText(19, 116));
            break;
        case eILB::lowWages:
            showTip(eLanguage::zeusText(19, 115));
            break;
        case eILB::unemployment:
            showTip(eLanguage::zeusText(19, 113));
            break;
        case eILB::highTaxes:
            showTip(eLanguage::zeusText(19, 114));
            break;
        case eILB::excessiveMilitaryService:
            showTip(eLanguage::zeusText(19, 117));
            break;
        case eILB::lackOfVacancies:
            showTip(eLanguage::zeusText(19, 111));
            break;
        case eILB::none:
            showTip(eLanguage::zeusText(19, 124));
        }
    }

    mTime += by;
    mTotalTime += by;
    bool nextMonth = false;
    bool nextYear = false;
    const int nd = mTime/dayLen;
    mDate.nextDays(nd, nextMonth, nextYear);
    mTime -= nd*dayLen;

    for(int i = 0; i < (int)mGameEvents.size(); i++) {
        const auto& e = mGameEvents[i];
        if(e->finished() && !e->hasActiveConsequences()) {
            eVectorHelpers::remove(mGameEvents, e);
            i--;
        } else {
            e->handleNewDate(mDate);
        }
    }

    if(nextYear) {
        mTaxesPaidLastYear = mTaxesPaidThisYear;
        mTaxesPaidThisYear = 0;
        mPeoplePaidTaxesLastYear = mPeoplePaidTaxesThisYear;
        mPeoplePaidTaxesThisYear = 0;

        for(const auto& c : mDefeatedBy) {
            if(!c->isRival()) continue;
            const auto rr = e::make_shared<eReceiveRequestEvent>(
                                eGameEventBranch::root);
            const auto type = c->recTributeType();
            const int count = c->recTributeCount();
            rr->initialize(0, type, count, c, false);
            rr->setTributeRequest(true);
            rr->initializeDate(mDate);
            rr->setGameBoard(this);
            rr->setWorldBoard(mWorldBoard);
            addRootGameEvent(rr);
        }
    }
    if(nextMonth) {
        const int d = std::ceil(mEmplData.pensions()/12.);
        incDrachmas(-d);
        {
            int nTot = 0;
            int nCalled = 0;
            for(const auto& s : mPalaceSoldierBanners) {
                const bool a = s->isAbroad();
                if(a) continue;
                const int c = s->count();
                nTot += c;
                const bool h = s->isHome();
                if(h) continue;
                nCalled += c;
            }
            if(nTot == 0 || static_cast<double>(nCalled)/nTot < 0.25) {
                mExcessiveMilitaryServiceCount--;
                mMonthsOfMilitaryService = 0;
            } else {
                mMonthsOfMilitaryService++;
                if(mMonthsOfMilitaryService > 6) {
                    mExcessiveMilitaryServiceCount = 2;
                }
            }
        }

        if(mDrachmas < 0) {
            const bool sameMonth = mDate.month() == mInDebtSince.month();
            const bool oneYear = mDate.year() - mInDebtSince.year() == 1;
            if(sameMonth && oneYear) {
                eEventData ed;
                event(eEvent::debtAnniversary, ed);
            }
        }

        mPopData.nextMonth();

        if(!mPoseidonMode) {
            const auto m = mDate.month();
            const int ng = std::abs(mDate.year() % 4);
            const auto game = static_cast<eGames>(ng);
            if(m == eMonth::june) {
                handleGamesBegin(game);
            } else if(m == eMonth::august) {
                handleGamesEnd(game);
            }
        }
    }

    const int ect = 5000;
    mEmploymentCheckTime += by;
    if(mEmploymentCheckTime > ect) {
        mEmploymentCheckTime -= ect;
        const double employable = mEmplData.employable();
        const double jobVacs = mEmplData.totalJobVacancies();
        int emplState = 0;
        if(employable < jobVacs*0.75) {
            emplState = 1;
        } else if(employable > jobVacs*1.25) {
            emplState = -1;
        } else {
            emplState = 0;
        }
        if(mLastEmploymentState != emplState) {
            const auto& inst = eMessages::instance;
            eEventData ed;
            if(emplState == -1) { // unemployed
                showMessage(ed, inst.fUnemployment);
            } else if(emplState == 1) { // employed
                showMessage(ed, inst.fEmployees);
            }
        }
        mLastEmploymentState = emplState;
    }

    if(nextMonth) {
        mWorldBoard->nextMonth(this);
    }
    if(nextYear) {
        mWorldBoard->nextYear();
        const auto cs = mWorldBoard->getTribute();
        for(const auto& c : cs) {
            if(c->conqueredByRival()) continue;
            tributeFrom(c, true);
        }
    }
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

    const int goalsCheckWait = 5050;
    mGoalsCheckTime += by;
    if(mGoalsCheckTime > goalsCheckWait) {
        mGoalsCheckTime -= goalsCheckWait;
        const bool f = checkGoalsFulfilled();
        if(f) {
            if(mEpisodeFinishedHandler) {
                mEpisodeFinishedHandler();
            }
        }
    }

    const int employmentUpdateWait = 5678;
    mEmploymentUpdateWait += by;
    if(mEmploymentUpdateWait > employmentUpdateWait) {
        if(mEmploymentUpdateScheduled) {
            mEmploymentUpdateWait = 0;
            distributeEmployees();
        }
    }
}

void eGameBoard::incFrame() {
    mFrame++;
}

void eGameBoard::handleFinishedTasks() {
    mThreadPool.handleFinished();
}

void eGameBoard::scheduleDataUpdate() {
    mThreadPool.scheduleDataUpdate();
}

void eGameBoard::payTaxes(const int d, const int people) {
    incDrachmas(d);
    mTaxesPaidThisYear += d;
    mPeoplePaidTaxesThisYear += people;
}

void eGameBoard::incDrachmas(const int d) {
    const bool wasInDebt = mDrachmas < 0;
    mDrachmas += d;
    const bool isInDebt = mDrachmas < 0;
    if(!wasInDebt && isInDebt) {
        mInDebtSince = mDate;
    }
}

void eGameBoard::setDate(const eDate& d) {
    mDate = d;
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
    distributeEmployees();
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

void eGameBoard::setRequestUpdateHandler(const eAction& ru) {
    mRequestUpdateHandler = ru;
}

void eGameBoard::setEventHandler(const eEventHandler& eh) {
    mEventHandler = eh;
}

void eGameBoard::event(const eEvent e, eEventData& ed) {
    if(mEventHandler) mEventHandler(e, ed);
}

void eGameBoard::setEpisodeFinishedHandler(const eAction& a) {
    mEpisodeFinishedHandler = a;
}

void eGameBoard::setAutosaver(const eAction& a) {
    mAutosaver = a;
}

void eGameBoard::setVisibilityChecker(const eVisibilityChecker& vc) {
    mVisibilityChecker = vc;
}

void eGameBoard::setTipShower(const eTipShower& ts) {
    mTipShower = ts;
}

void eGameBoard::showTip(const std::string& tip) const {
    if(mTipShower) mTipShower(tip);
}

void eGameBoard::setEnlistForcesRequest(const eEnlistRequest& req) {
    mEnlistRequester = req;
}

void eGameBoard::requestForces(const eEnlistAction& action,
                               const std::vector<eResourceType>& plunderResources,
                               const std::vector<stdsptr<eWorldCity>>& exclude) {
    if(mEnlistRequester) {
        auto f = getEnlistableForces();
        std::vector<bool> heroesAbroad;
        for(const auto h : f.fHeroes) {
            const auto hh = heroHall(h);
            const bool abroad = !hh ? true : hh->heroOnQuest();
            heroesAbroad.push_back(abroad);
        }
        for(const auto& e : exclude) {
            eVectorHelpers::remove(f.fAllies, e);
        }
        mEnlistRequester(f, heroesAbroad, action, plunderResources);
    }
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

void eGameBoard::showMessage(eEventData& ed,
                             const eMessageType& msg) {
    mMsgShower(ed, msg);
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

int eGameBoard::resourceCount(const eResourceType type) const {
    if(type == eResourceType::drachmas) {
        return mDrachmas;
    }
    int result = 0;
    for(auto& r : mResources) {
        if(!static_cast<bool>(r.first & type)) continue;
        result += r.second;
    }
    return result;
}

int eGameBoard::takeResource(const eResourceType type, const int count) {
    if(type == eResourceType::drachmas) {
        incDrachmas(-count);
        return count;
    }
    int r = 0;
    using eValidator = std::function<bool(eStorageBuilding*)>;
    const auto takeFunc = [&](const eValidator& v) {
        if(r >= count) return;
        for(const auto s : mStorBuildings) {
            if(!v(s)) continue;
            if(r >= count) return;
            r += s->take(type, count - r);
        }
    };
    takeFunc([&](eStorageBuilding* const s) {
        return s->empties(type);
    });
    takeFunc([&](eStorageBuilding* const s) {
        return !s->accepts(type);
    });
    takeFunc([&](eStorageBuilding* const s) {
        return !s->get(type);
    });
    takeFunc([&](eStorageBuilding* const s) {
        (void)s;
        return true;
    });
    return count - r;
}

int eGameBoard::eliteHouses() const {
    int r = 0;
    for(const auto b : mTimedBuildings) {
        const auto bt = b->type();
        if(bt == eBuildingType::eliteHousing) r++;
    }
    return r;
}

eSanctuary* eGameBoard::sanctuary(const eGodType god) const {
    for(const auto s : mSanctuaries) {
        if(s->godType() == god) return s;
    }
    return nullptr;
}

eHerosHall* eGameBoard::heroHall(const eHeroType hero) const {
    for(const auto h : mHeroHalls) {
        if(h->heroType() == hero) return h;
    }
    return nullptr;
}

int eGameBoard::countBanners(const eBannerType bt) const {
    int c = 0;
    for(const auto& bn : mSoldierBanners) {
        const int pid = bn->playerId();
        if(pid != 1) continue;
        if(bn->type() != bt) continue;
        c++;
    }
    return c;
}

int eGameBoard::countSoldiers(const eBannerType bt) const {
    int c = 0;
    for(const auto& bn : mSoldierBanners) {
        const int pid = bn->playerId();
        if(pid != 1) continue;
        if(bn->type() != bt) continue;
        c += bn->count();
    }
    return c;
}

void eGameBoard::addFulfilledQuest(const eGodQuest q) {
    mFulfilledQuests.push_back(q);
}

void eGameBoard::addSlayedMonster(const eMonsterType m) {
    mSlayedMonsters.push_back(m);
}

void eGameBoard::startEpisode(eEpisode* const e,
                              const eWC& lastPlayedColony) {
    if(mTimedBuildings.empty()) { // first episode
        for(const auto s : mSpawners) {
            const auto type = s->type();
            if(type == eBannerTypeS::boar ||
               type == eBannerTypeS::deer) {
                s->spawnMax();
            }
        }
    }
    mDefeatedBy.clear();
    for(int i = 0; i < static_cast<int>(mGameEvents.size()); i++) {
        const auto& e = mGameEvents[i];
        const auto type = e->type();
        if(type == eGameEventType::godQuest) continue;
        e->startingNewEpisode();
        if(e->finished()) {
            if(!e->hasActiveConsequences()) {
                mGameEvents.erase(mGameEvents.begin() + i);
                i--;
            }
        }
    }
    mGoals.clear();
    mWorldBoard = e->fWorldBoard;
    const auto& date = e->fStartDate;
    setDate(date);
    mDrachmas = e->fDrachmas;
    mWageMultiplier = e->fWageMultiplier;
    mPrices = e->fPrices;
    const auto& es = e->fEvents;
    for(const auto& ee : es) {
        const auto eee = ee->makeCopy();
        eee->setupStartDate(date);
        mGameEvents.push_back(eee);
    }
    mAvailableBuildings.startEpisode(e->fAvailableBuildings);
    setFriendlyGods(e->fFriendlyGods);
    mPoseidonMode = e->fAtlantean;
    const auto& gs = e->fGoals;
    for(const auto& g : gs) {
        const auto gg = g->makeCopy();
        gg->initializeDate(this);
        gg->update(this);
        mGoals.push_back(gg);
    }
    if(lastPlayedColony) {
        const auto a = new eColonyMonumentAction(lastPlayedColony);
        planAction(a);
    }
}

bool eGameBoard::checkGoalsFulfilled() const {
    if(mGoals.empty()) return false;
    bool result = true;
    for(const auto& g : mGoals) {
        g->update(this);
        const bool m = g->met();
        if(!m) result = false;
    }
    return result;
}

void eGameBoard::setManTowers(const bool m) {
    mManTowers = m;
}

void eGameBoard::musterAllSoldiers() {
    for(const auto& s : mSoldierBanners) {
        s->backFromHome();
    }
}

void eGameBoard::sendAllSoldiersHome() {
    for(const auto& s : mSoldierBanners) {
        s->goHome();
    }
}

void eGameBoard::addShutDown(const eResourceType type) {
    mShutDown.push_back(type);
    for(const auto b : mEmployingBuildings) {
        const auto bt = b->type();
        const auto is = eIndustryHelpers::sIndustries(bt);
        if(is.empty()) continue;
        const bool r = eVectorHelpers::contains(is, type);
        if(!r) continue;
        const bool sd = is.size() == 1 ? true : isShutDown(bt);
        if(!sd) continue;
        b->setShutDown(true);
        const int maxE = b->maxEmployees();
        mEmplData.incTotalJobVacancies(-maxE);
        eSector s;
        const bool ss = eSectorHelpers::sBuildingSector(bt, s);
        if(!ss) continue;
        mEmplDistributor.incMaxEmployees(s, -maxE);
    }
    distributeEmployees();
}

void eGameBoard::removeShutDown(const eResourceType type) {
    const auto oldShutDown = mShutDown;
    const auto wasShutDown = [&](const eBuildingType bt) {
        const auto is = eIndustryHelpers::sIndustries(bt);
        if(is.empty()) return false;
        for(const auto i : is) {
            const bool sd = eVectorHelpers::contains(oldShutDown, i);
            if(!sd) return false;
        }
        return true;
    };
    const bool r = eVectorHelpers::remove(mShutDown, type);
    if(!r) return;
    for(const auto b : mEmployingBuildings) {
        const auto bt = b->type();
        const auto is = eIndustryHelpers::sIndustries(bt);
        if(is.empty()) continue;
        const bool r = eVectorHelpers::contains(is, type);
        if(!r) continue;
        const bool sd = is.size() == 1 ? false : isShutDown(bt);
        if(sd) continue;
        const bool wasSD = wasShutDown(bt);
        if(!wasSD) continue;
        b->setShutDown(false);
        const int maxE = b->maxEmployees();
        mEmplData.incTotalJobVacancies(maxE);
        eSector s;
        const bool ss = eSectorHelpers::sBuildingSector(bt, s);
        if(!ss) continue;
        mEmplDistributor.incMaxEmployees(s, maxE);
    }
    distributeEmployees();
}

bool eGameBoard::isShutDown(const eResourceType type) const {
    return eVectorHelpers::contains(mShutDown, type);
}

bool eGameBoard::isShutDown(const eBuildingType type) const {
    if(type == eBuildingType::tower) return !mManTowers;
    const auto is = eIndustryHelpers::sIndustries(type);
    if(is.empty()) return false;
    for(const auto i : is) {
        const bool sd = isShutDown(i);
        if(!sd) return false;
    }
    return true;
}

int eGameBoard::industryJobVacancies(const eResourceType type) const {
    const auto bs = eIndustryHelpers::sBuildings(type);
    int result = 0;
    for(const auto b : bs) {
        result += countBuildings(b);
    }
    return result;
}

void eGameBoard::distributeEmployees(const eSector s) {
    int e = mEmplDistributor.employees(s);
    const int maxE = mEmplDistributor.maxEmployees(s);
    const double frac = e/static_cast<double>(maxE);
    const auto& sb = mSectorBuildings[s];
    struct eSectorReminder {
        double fRem;
        eEmployingBuilding* fB;
    };

    std::vector<eSectorReminder> reminders;
    for(const auto b : sb) {
        const auto type = b->type();
        const bool sd = isShutDown(type);
        if(sd) {
            b->setShutDown(true);
            continue;
        } else {
            b->setShutDown(false);
        }
        const int me = b->maxEmployees();
        const double eeF = frac*me;
        const int ee = std::floor(eeF);
        b->setEmployed(ee);
        e -= ee;
        reminders.push_back({(eeF - ee)/me, b});
    }

    const auto comp = [](const eSectorReminder& r1, const eSectorReminder& r2) {
        return r1.fRem > r2.fRem;
    };
    std::sort(reminders.begin(), reminders.end(), comp);

    while(e > 0) {
        bool changed = false;
        for(auto& r : reminders) {
            if(e <= 0) break;
            const auto b = r.fB;
            const int me = b->maxEmployees();
            const int ee = b->employed();
            if(ee >= me) continue;
            b->setEmployed(ee + 1);
            e--;
            changed = true;
        }
        if(!changed) break;
    }
}

void eGameBoard::distributeEmployees() {
    for(const auto& s : mSectorBuildings) {
        distributeEmployees(s.first);
    }
}

void eGameBoard::scheduleDistributeEmployees() {
    mEmploymentUpdateScheduled = true;
}

void eGameBoard::incPopulation(const int by) {
    mPopData.incPopulation(by);
    scheduleDistributeEmployees();
}

void eGameBoard::topElevationExtremas(int& min, int& max) const {
    min = 10000;
    max = -10000;
    for(int x = 0; x < mWidth; x++) {
        const auto t = dtile(x, 0);
        const int a = t->altitude();
        min = std::min(min, a);
        max = std::max(max, a);
    }
}

void eGameBoard::rightElevationExtremas(int& min, int& max) const {
    min = 10000;
    max = -10000;
    for(int y = 0; y < mHeight; y++) {
        const auto t = dtile(mWidth - 1, y);
        const int a = t->altitude();
        min = std::min(min, a);
        max = std::max(max, a);
    }
}

void eGameBoard::bottomElevationExtremas(int& min, int& max) const {
    min = 10000;
    max = -10000;
    for(int x = 0; x < mWidth; x++) {
        const auto t = dtile(x, mHeight - 1);
        const int a = t->altitude();
        min = std::min(min, a);
        max = std::max(max, a);
    }
}

void eGameBoard::leftElevationExtremas(int& min, int& max) const {
    min = 10000;
    max = -10000;
    for(int y = 0; y < mHeight; y++) {
        const auto t = dtile(0, y);
        const int a = t->altitude();
        min = std::min(min, a);
        max = std::max(max, a);
    }
}

void eGameBoard::minMaxAltitude(int& min, int& max) const {
    min = 10000;
    max = -10000;
    for(int x = 0; x < mWidth; x++) {
        for(int y = 0; y < mHeight; y++) {
            const auto t = dtile(x, y);
            const int a = t->altitude();
            if(a > max) max = a;
            if(a < min) min = a;
        }
    }
}

eMilitaryAid* eGameBoard::militaryAid(const stdsptr<eWorldCity>& c) const {
    for(const auto& m : mMilitaryAid) {
        if(m->fCity == c) return m.get();
    }
    return nullptr;
}

void eGameBoard::removeMilitaryAid(const stdsptr<eWorldCity>& c) {
    const int iMax = mMilitaryAid.size();
    for(int i = 0; i < iMax; i++) {
        const auto& m = mMilitaryAid[i];
        if(m->fCity != c) continue;
        mMilitaryAid.erase(mMilitaryAid.begin() + i);
        break;
    }
}

void eGameBoard::addMilitaryAid(const stdsptr<eMilitaryAid>& a) {
    mMilitaryAid.push_back(a);
}

void eGameBoard::setEpisodeLost() const {
    mEpisodeLost = true;
}

int eGameBoard::tradingPartners() const {
    int n = 0;
    for(const auto t : mTradePosts) {
        const bool trades = t->trades();
        if(!trades) continue;
        n++;
    }
    return n;
}

eOrientation randomOrientation() {
    std::vector<eOrientation> os{eOrientation::topRight,
                                 eOrientation::bottomRight,
                                 eOrientation::bottomLeft,
                                 eOrientation::topLeft};
    return os[eRand::rand() % os.size()];
}

void eGameBoard::earthquake(eTile* const startTile, const int size) {
    struct eQuakeEnd {
        eTile* fTile = nullptr;
        eOrientation fLastO = randomOrientation();
    };

    const auto quake = std::make_shared<eEarthquake>();
    mEarthquakes.push_back(quake);

    std::queue<eQuakeEnd> ends;
    quake->fStartTile = startTile;
    auto& tiles = quake->fTiles;
    tiles.push_back(startTile);
    ends.push({startTile});
    
    while((int)tiles.size() < size && !ends.empty()) {
        const auto t = ends.front();
        ends.pop();
        std::vector<eOrientation> os{eOrientation::topRight,
                                     eOrientation::bottomRight,
                                     eOrientation::bottomLeft,
                                     eOrientation::topLeft};

        // Replace random_shuffle with shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(os.begin(), os.end(), g);

        if(eRand::rand() % 7) {
            os.insert(os.begin(), t.fLastO);
        }
        for(const auto o : os) {
            const auto tt = t.fTile->neighbour<eTile>(o);
            if(!tt) continue;
            const auto terr = tt->terrain();
            if(terr == eTerrain::dry ||
               terr == eTerrain::fertile ||
               terr == eTerrain::forest ||
               terr == eTerrain::choppedForest) {
                ends.push({tt, o});
                tiles.push_back(tt);
                if(eRand::rand() % 5 == 0) {
                    ends.push({tt});
                }
                break;
            }
        }
    }
}

bool eGameBoard::duringEarthquake() const {
    return !mEarthquakes.empty();
}

void eGameBoard::defeatedBy(const stdsptr<eWorldCity>& c) {
    const bool r = eVectorHelpers::contains(mDefeatedBy, c);
    if(r) {
        setEpisodeLost();
    } else {
        mDefeatedBy.push_back(c);
    }
}

void eGameBoard::progressEarthquakes() {
    if(mEarthquakes.empty()) return;
    eSounds::playEarthquakeSound();
    for(int i = 0; i < (int)mEarthquakes.size(); i++) {
        const auto& e = mEarthquakes[i];
        const auto st = e->fStartTile;
        const int stx = st->x();
        const int sty = st->y();
        const auto prcs = [this, e, stx, sty](const int x, const int y) {
            const int tx = stx + x;
            const int ty = sty + y;
            const auto t = tile(tx, ty);
            if(!t) return false;
            const bool r = eVectorHelpers::contains(e->fTiles, t);
            if(!r) return false;
            t->setTerrain(eTerrain::quake);
            if(const auto ub = t->underBuilding()) {
                const auto type = ub->type();
                const bool s = eBuilding::sSanctuaryBuilding(type);
                if(s) {
                } else if(type == eBuildingType::ruins) {
                    ub->erase();
                } else {
                    if(const auto as = dynamic_cast<eAgoraSpace*>(ub)) {
                        const auto a = as->agora();
                        a->collapse();
                    } else if(const auto v = dynamic_cast<eVendor*>(ub)) {
                        const auto a = v->agora();
                        a->collapse();
                    } else if(const auto r = dynamic_cast<eRoad*>(ub)) {
                        const auto a = r->underAgora();
                        if(a) a->collapse();
                        const auto g = r->underGatehouse();
                        if(g) g->collapse();
                        r->eBuilding::erase();
                    } else {
                        ub->collapse();
                    }
                    eSounds::playCollapseSound();
                }
            }
            eVectorHelpers::removeAll(e->fTiles, t);
            return false;
        };
        eIterateSquare::iterateSquare(e->fLastDim, prcs);
        e->fLastDim++;
        if(e->fTiles.empty() || e->fLastDim > 50) {
            eVectorHelpers::remove(mEarthquakes, e);
            i--;
        }
    }
    scheduleTerrainUpdate();
}
