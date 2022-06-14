#include "etradepost.h"

#include "textures/egametextures.h"

#include "engine/egameboard.h"

#include "epathfindtask.h"
#include "ewalkablehelpers.h"

#include "characters/etrader.h"
#include "characters/actions/etraderaction.h"

eTradePost::eTradePost(eGameBoard& board, eWorldCity& city) :
    eWarehouseBase(board, eBuildingType::tradePost, 4, 4, 24,
                   eResourceType::warehouse, 15),
    mCity(city) {
    setOverlayEnabledFunc([]() { return true; });
    setOrders(eResourceType::none, eResourceType::none);
    getBoard().registerTradePost(this);
}

eTradePost::~eTradePost() {
    getBoard().unregisterTradePost(this);
}

std::shared_ptr<eTexture> eTradePost::getTexture(const eTileSize size) const {
    const int sizeId = static_cast<int>(size);
    const auto& blds = eGameTextures::buildings();
    return blds[sizeId].fTradingPost;
}

std::vector<eOverlay> eTradePost::getOverlays(const eTileSize size) const {
    std::vector<eOverlay> os;
    const int sizeId = static_cast<int>(size);
    const auto& blds = eGameTextures::buildings();
    const auto texs = blds[sizeId];
    if(enabled()) {
        const auto& coll = texs.fTradingPostOverlay;
        const int texId = textureTime() % coll.size();
        auto& o = os.emplace_back();
        o.fTex = coll.getTexture(texId);
        o.fX = -3.1;
        o.fY = -7.2;
    }
    const eXY xy = {{-1.5, -3.5},
                    {-1.5, -2.5},
                    {-1.5, -1.5},

                    {-0.5, -4.5},
                    {-0.5, -3.5},
                    {-0.5, -2.5},
                    {-0.5, -1.5},

                    {0.5, -4.5},
                    {0.5, -3.5},
                    {0.5, -2.5},
                    {0.5, -1.5},

                    {1.5, -4.5},
                    {1.5, -3.5},
                    {1.5, -2.5},
                    {1.5, -1.5}};

    getSpaceOverlays(size, os, xy);

    return os;
}

void eTradePost::timeChanged(const int by) {
    const int rt = 30000;
    mRouteTimer += by;
    if(mRouteTimer > rt) {
        mRouteTimer -= rt;
        spawnTrader();
    }
    eWarehouseBase::timeChanged(by);
}

void eTradePost::setOrders(const eResourceType imports,
                           const eResourceType exports) {
    mImports = imports;
    mExports = exports;

    eStorageBuilding::setOrders(mExports, mImports, eResourceType::none);
}

void eTradePost::getOrders(eResourceType& imports,
                           eResourceType& exports) const {
    imports = mImports;
    exports = mExports;
}

void eTradePost::updateRouteStart() {
    auto& brd = getBoard();
    auto& tp = brd.threadPool();

    const auto walkable = eWalkableHelpers::sBuildingWalkable(
                              this, eWalkableHelpers::sDefaultWalkable);

    const auto t = centerTile();
    const int tx = t->x();
    const int ty = t->y();

    const auto startTile = [tx, ty](eThreadBoard& board) {
        return board.tile(tx, ty);
    };

    const auto finalTile = std::make_shared<std::pair<int, int>>();

    const auto final = [tx, ty, finalTile](eTileBase* const t) {
        const int dx = tx - t->x();
        const int dy = ty - t->y();
        const int dist = sqrt(dx*dx + dy*dy);
        if(dist < 40) return false;
        *finalTile = {t->x(), t->y()};
        if(!t->topRight()) return true;
        if(!t->bottomRight()) return true;
        if(!t->bottomLeft()) return true;
        if(!t->topLeft()) return true;
        return false;
    };

    stdptr<eTradePost> tptr(this);

    const auto finishFunc = [tptr, this, finalTile](
                            std::vector<eOrientation>) {
        if(!tptr) return;
        const int tx = finalTile->first;
        const int ty = finalTile->second;
        mRouteStart = getBoard().tile(tx, ty);
    };

    const auto findFailFunc = [tptr, this]() {
        if(!tptr) return;
        mRouteStart = nullptr;
    };

    const auto pft = new ePathFindTask(startTile, walkable,
                                       final, finishFunc,
                                       findFailFunc, true,
                                       200);
    tp.queueTask(pft);
}

void eTradePost::spawnTrader() {
    if(!mRouteStart) return updateRouteStart();
    auto& board = getBoard();

    const auto r = e::make_shared<eTrader>(board);
    r->changeTile(mRouteStart);
    r->createFollowers();

    const auto ta = e::make_shared<eTraderAction>(r.get(), [](){}, [](){});
    ta->setFinishOnComeback(true);
    ta->setTradePost(this);
    r->setAction(ta);
}

int eTradePost::buy(const int cash) {
    int spent = 0;
    for(auto& b : mCity.buys()) {
        const auto expt = mExports & b.fType;
        const bool exp = static_cast<bool>(expt);
        if(!exp) continue;
        if(b.fUsed >= b.fMax) continue;
        if(b.fPrice > cash) continue;
        const int c = count(b.fType);
        if(c <= 0) continue;
        take(b.fType, 1);
        b.fUsed++;
        spent += b.fPrice;
    }
    auto& brd = getBoard();
    brd.incDrachmas(spent);
    return spent;
}

int eTradePost::sell(const int items) {
    int earned = 0;
    for(auto& b : mCity.sells()) {
        const auto impt = mImports & b.fType;
        const bool imp = static_cast<bool>(impt);
        if(!imp) continue;
        if(b.fUsed >= b.fMax) continue;
        if(b.fPrice > items) continue;
        const int c = spaceLeftDontAccept(b.fType);
        if(c <= 0) continue;
        addNotAccept(b.fType, 1);
        b.fUsed++;
        earned += b.fPrice;
    }
    auto& brd = getBoard();
    brd.incDrachmas(-earned);
    return earned;
}
