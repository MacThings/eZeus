#include "eroad.h"

#include "textures/egametextures.h"

#include "engine/egameboard.h"
#include "eagorabase.h"
#include "egatehouse.h"
#include "ehippodromepiece.h"
#include "elanguage.h"

eRoad::eRoad(eGameBoard& board, const eCityId cid) :
    eBuilding(board, eBuildingType::road, 1, 1, cid) {}

void eRoad::erase() {
    if(mAboveHippodrome) {
        mAboveHippodrome->erase();
    } else if(isBridge()) {
        auto& board = getBoard();
        const auto cid = cityId();
        const bool a = board.hasActiveInvasions(cid);
        if(a) {
            board.showTip(cid, eLanguage::zeusText(19, 229)); // can't demolish during invasion
            return;
        }
        std::vector<eTile*> tiles;
        bridgeConnectedTiles(tiles);
        for(const auto t : tiles) {
            const auto c = t->characters();
            if(!c.empty()) {
                board.showTip(cid, eLanguage::zeusText(19, 24)); // can't demolish water crossing with people
                return;
            }
        }
        for(const auto t : tiles) {
            const auto b = t->underBuilding();
            if(!b) continue;
            b->eBuilding::erase();
        }
    } else {
        if(mRoadblock) setRoadblock(false);
        else eBuilding::erase();
    }
}

std::shared_ptr<eTexture> eRoad::getTexture(const eTileSize size) const {
    if(mAboveHippodrome) return nullptr;

    auto& board = getBoard();
    const auto dir = board.direction();

    const int sizeId = static_cast<int>(size);
    const auto& trrTexs = eGameTextures::terrain()[sizeId];
    const auto& builTexs = eGameTextures::buildings()[sizeId];

    if(mRoadblock) {
        eGameTextures::loadRoadblock();
        return builTexs.fRoadblock;
    }

    if(mUnderAgora) {
        const auto& coll = builTexs.fAgoraRoad;
        return coll.getTexture(seed() % coll.size());
    }

    if(mUnderGatehouse) {
        const auto& r = tileRect();
        const auto& rr = mUnderGatehouse->tileRect();
        const int dx = r.x - rr.x;
        const int dy = r.y - rr.y;
        if(dy == 0) {
            return mUnderGatehouse->getTextureEntranceBack(size).fTex;
        } else if(dy == 1) {
            return mUnderGatehouse->getTextureEntrance(size).fTex;
        } else if(dx == 0) {
            return mUnderGatehouse->getTextureEntranceBack(size).fTex;
        } else if(dx == 1) {
            return mUnderGatehouse->getTextureEntrance(size).fTex;
        }
    }

    const auto ti = centerTile();
    const auto tr = ti->topRightRotated(dir);
    const auto br = ti->bottomRightRotated(dir);
    const auto bl = ti->bottomLeftRotated(dir);
    const auto tl = ti->topLeftRotated(dir);

    const bool trRoad = !tr || tr->hasRoad();
    const bool brRoad = !br || br->hasRoad();
    const bool blRoad = !bl || bl->hasRoad();
    const bool tlRoad = !tl || tl->hasRoad();

    const auto& tbcoll = trrTexs.fToBeachRoad;
    const auto& bcoll = trrTexs.fBeachRoad;

    const bool bridge = ti->hasBridge();
    if(bridge) {
        eGameTextures::loadBridge();

        int texId = 10;

        const bool trb = tr && tr->hasBridge();
        const bool brb = br && br->hasBridge();
        const bool blb = bl && bl->hasBridge();
        const bool tlb = tl && tl->hasBridge();

        const bool trw = tr && (tr->hasWater() || tr->hasQuake());
        const bool brw = br && (br->hasWater() || br->hasQuake());
        const bool blw = bl && (bl->hasWater() || bl->hasQuake());
        const bool tlw = tl && (tl->hasWater() || tl->hasQuake());

        if((tlb || brb) || ((trw && !trb) || (blw && !blb))) {
            texId = 10;
        } else if((trb || blb) || ((tlw && !tlb) || (brw && !brb))) {
            texId = 11;
        }
        return builTexs.fBridge.getTexture(texId);
    }

    const auto trt = tr ? tr->underBuildingType() : eBuildingType::none;
    const auto brt = br ? br->underBuildingType() : eBuildingType::none;
    const auto tlt = tl ? tl->underBuildingType() : eBuildingType::none;
    const auto blt = bl ? bl->underBuildingType() : eBuildingType::none;

    const bool avn = trt == eBuildingType::avenue ||
                     brt == eBuildingType::avenue ||
                     tlt == eBuildingType::avenue ||
                     blt == eBuildingType::avenue;

    if(avn) {
        const bool tlb = tlt == eBuildingType::avenue ||
                         tlt == eBuildingType::road;
        const bool brb = brt == eBuildingType::avenue ||
                         brt == eBuildingType::road;
        const bool blb = blt == eBuildingType::avenue ||
                         blt == eBuildingType::road;
        const bool trb = trt == eBuildingType::avenue ||
                         trt == eBuildingType::road;

        const auto t = ti->topRotated<eTile>(dir);
        const auto b = ti->bottomRotated<eTile>(dir);
        const auto l = ti->leftRotated<eTile>(dir);
        const auto r = ti->rightRotated<eTile>(dir);

        const auto tt = t ? t->underBuildingType() : eBuildingType::none;
        const auto bt = b ? b->underBuildingType() : eBuildingType::none;
        const auto lt = l ? l->underBuildingType() : eBuildingType::none;
        const auto rt = r ? r->underBuildingType() : eBuildingType::none;

        const bool tb = tt == eBuildingType::avenue ||
                        tt == eBuildingType::road;
        const bool lb = lt == eBuildingType::avenue ||
                        lt == eBuildingType::road;
        const bool bb = bt == eBuildingType::avenue ||
                        bt == eBuildingType::road;
        const bool rb = rt == eBuildingType::avenue ||
                        rt == eBuildingType::road;

        int id = 0;
        if(tlb && brb && blb && trb) {
            if(!tb && !lb && !bb && !rb) {
                id = 0;
            } else if(!tb && !lb && !bb && rb) {
                id = 1;
            } else if(!tb && !lb && bb && !rb) {
                id = 2;
            } else if(!tb && lb && !bb && !rb) {
                id = 3;
            } else if(tb && !lb && !bb && !rb) {
                id = 4;
            } else if(tb && lb && !bb && !rb) {
                id = 5;
            } else if(tb && !lb && !bb && rb) {
                id = 6;
            } else if(!tb && !lb && bb && rb) {
                id = 7;
            } else if(!tb && lb && bb && !rb) {
                id = 8;
            } else if(!tb && lb && !bb && rb) {
                id = 9;
            } else if(tb && !lb && bb && !rb) {
                id = 10;
            } else if(tb && lb && !bb && rb) {
                id = 11;
            } else if(tb && lb && bb && !rb) {
                id = 12;
            } else if(tb && !lb && bb && rb) {
                id = 13;
            } else if(!tb && lb && bb && rb) {
                id = 14;
            } else if(tb && lb && bb && rb) {
                id = 15 + seed() % 4;
            }
        } else if(trb && brb && blb) {
            if(rb && bb) {
                id = 19;
            } else if(bb) {
                id = 20;
            } else if(rb) {
                id = 21;
            } else {
                id = 22;
            }
        } else if(tlb && brb && blb) {
            if(lb && bb) {
                id = 23;
            } else if(lb) {
                id = 24;
            } else if(bb) {
                id = 25;
            } else {
                id = 26;
            }
        } else if(tlb && trb && blb) {
            if(lb && tb) {
                id = 27;
            } else if(tb) {
                id = 28;
            } else if(lb) {
                id = 29;
            } else {
                id = 30;
            }
        } else if(tlb && trb && brb) {
            if(rb && tb) {
                id = 31;
            } else if(rb) {
                id = 32;
            } else if(tb) {
                id = 33;
            } else {
                id = 34;
            }
        } else if(trb && brb) {
            if(rb) {
                id = 35;
            } else {
                id = 36;
            }
        } else if(blb && brb) {
            if(bb) {
                id = 37;
            } else {
                id = 38;
            }
        } else if(blb && tlb) {
            if(lb) {
                id = 39;
            } else {
                id = 40;
            }
        } else if(tlb && trb) {
            if(tb) {
                id = 41;
            } else {
                id = 42;
            }
        } else if(blb && trb) {
            id = 43;
        } else if(tlb && brb) {
            id = 44;
        } else if(trb) {
            id = 45;
        } else if(brb) {
            id = 46;
        } else if(blb) {
            id = 47;
        } else if(tlb) {
            id = 48;
        }
        return builTexs.fAvenueRoad.getTexture(id);
    }

    if(ti->terrain() == eTerrain::beach) {
        if(bl && bl->terrain() == eTerrain::dry) {
            return tbcoll.getTexture(0);
        } else if(tl && tl->terrain() == eTerrain::dry) {
            return tbcoll.getTexture(1);
        } else if(tr && tr->terrain() == eTerrain::dry) {
            return tbcoll.getTexture(2);
        } else if(br && br->terrain() == eTerrain::dry) {
            return tbcoll.getTexture(3);
        }


        if(tlRoad && blRoad && trRoad && brRoad) return bcoll.getTexture(17);
        if(trRoad && brRoad && blRoad) return bcoll.getTexture(13);
        if(tlRoad && blRoad && brRoad) return bcoll.getTexture(14);
        if(tlRoad && blRoad && trRoad) return bcoll.getTexture(15);
        if(tlRoad && brRoad && trRoad) return bcoll.getTexture(16);

        if(blRoad && trRoad) {
            if(seed() % 2) return bcoll.getTexture(0);
            else return bcoll.getTexture(2);
        }
        if(tlRoad && brRoad) {
            if(seed() % 2) return bcoll.getTexture(1);
            else return bcoll.getTexture(3);
        }

        if(trRoad && brRoad) return bcoll.getTexture(4);
        if(blRoad && brRoad) return bcoll.getTexture(5);
        if(blRoad && tlRoad) return bcoll.getTexture(6);
        if(tlRoad && trRoad) return bcoll.getTexture(7);
        if(trRoad) return bcoll.getTexture(8);
        if(brRoad) return bcoll.getTexture(9);
        if(blRoad) return bcoll.getTexture(10);
        if(tlRoad) return bcoll.getTexture(11);

        return bcoll.getTexture(12);

    } else {
        const auto& pcoll = trrTexs.fPrettyRoad;
        const auto& rcoll = trrTexs.fRoad;

        const auto& b = getBoard();
        const auto t = centerTile();

        const int dx = t->dx();
        const int dy = t->dy();
        const double a = b.appeal(dx, dy);

        const eTextureCollection* coll;
        if(a > 3) {
            coll = &pcoll;
        } else {
            coll = &rcoll;
        }

        if(tlRoad && blRoad && trRoad && brRoad) return coll->getTexture(17);
        if(trRoad && brRoad && blRoad) return coll->getTexture(13);
        if(tlRoad && blRoad && brRoad) return coll->getTexture(14);
        if(tlRoad && blRoad && trRoad) return coll->getTexture(15);
        if(tlRoad && brRoad && trRoad) return coll->getTexture(16);

        if(blRoad && trRoad) {
            if(seed() % 2) return coll->getTexture(0);
            else return coll->getTexture(2);
        }
        if(tlRoad && brRoad) {
            if(seed() % 2) return coll->getTexture(1);
            else return coll->getTexture(3);
        }

        if(trRoad && brRoad) return coll->getTexture(4);
        if(blRoad && brRoad) return coll->getTexture(5);
        if(blRoad && tlRoad) return coll->getTexture(6);
        if(tlRoad && trRoad) return coll->getTexture(7);
        if(trRoad) return coll->getTexture(8);
        if(brRoad) return coll->getTexture(9);
        if(blRoad) return coll->getTexture(10);
        if(tlRoad) return coll->getTexture(11);

        return coll->getTexture(12);
    }
    return std::shared_ptr<eTexture>();
}

std::shared_ptr<eTexture> eRoad::getHippodromeTexture(const eTileSize size) const {
    auto& board = getBoard();
    const auto dir = board.direction();

    const int sizeId = static_cast<int>(size);
    const auto& builTexs = eGameTextures::buildings()[sizeId];

    const auto ti = centerTile();
    const auto tr = ti->topRight();
    const auto br = ti->bottomRight();
    const auto bl = ti->bottomLeft();
    const auto tl = ti->topLeft();

    const bool trRoad = !tr || tr->hasRoad();
    const bool brRoad = !br || br->hasRoad();
    const bool blRoad = !bl || bl->hasRoad();
    const bool tlRoad = !tl || tl->hasRoad();

    if(mAboveHippodrome) {
        int texId1;
        int texId2;
        int texId3;
        int texId4;
        int texId5;
        int texId6;
        switch(dir) {
        case eWorldDirection::N: {
            texId1 = 8;
            texId2 = 10;
            texId3 = 13;
            texId4 = 11;
            texId5 = 9;
            texId6 = 12;
        } break;
        case eWorldDirection::E: {
            texId1 = 13;
            texId2 = 11;
            texId3 = 10;
            texId4 = 8;
            texId5 = 12;
            texId6 = 9;
        } break;
        case eWorldDirection::S: {
            texId1 = 10;
            texId2 = 8;
            texId3 = 11;
            texId4 = 13;
            texId5 = 9;
            texId6 = 12;
        } break;
        case eWorldDirection::W: {
            texId1 = 11;
            texId2 = 13;
            texId3 = 8;
            texId4 = 10;
            texId5 = 12;
            texId6 = 9;
        } break;
        }
        const auto& coll = builTexs.fHippodrome;
        const auto& r = tileRect();
        const auto& rr = mAboveHippodrome->tileRect();
        if(r.y == rr.y + rr.h - 1) {
            if(trRoad) {
                return coll.getTexture(texId1);
            }
        }
        if(r.y == rr.y) {
            if(blRoad) {
                return coll.getTexture(texId2);
            }
        }
        if(r.x == rr.x + rr.w - 1) {
            if(tlRoad) {
                return coll.getTexture(texId3);
            }
        }
        if(r.x == rr.x) {
            if(brRoad) {
                return coll.getTexture(texId4);
            }
        }
        if(trRoad && blRoad) return coll.getTexture(texId5);
        if(tlRoad && brRoad) return coll.getTexture(texId6);
    }
    return nullptr;
}

void eRoad::setUnderAgora(eAgoraBase* const a) {
    mUnderAgora = a;
}

void eRoad::setUnderGatehouse(eGatehouse* const g) {
    mUnderGatehouse = g;
}

void eRoad::setAboveHippodrome(eHippodromePiece * const h) {
    mAboveHippodrome = h;
}

bool eRoad::isBridge() const {
    const auto t = centerTile();
    if(!t) return false;
    return t->hasBridge();
}

bool eRoad::isRoadblock() const {
    return mRoadblock;
}

void eRoad::setRoadblock(const bool rb) {
    const auto t = centerTile();
    if(t) t->setRoadblock(rb);
    mRoadblock = rb;
}

void eRoad::bridgeConnectedTiles(std::vector<eTile*>& tiles) const {
    eTile* const startTile = centerTile();
    if(!startTile) return;
    if(!startTile->hasBridge()) return;
    tiles.push_back(startTile);
    auto tl = startTile->topLeft<eTile>();
    auto tr = startTile->topRight<eTile>();
    auto br = startTile->bottomRight<eTile>();
    auto bl = startTile->bottomLeft<eTile>();
    while(tl) {
        const bool hb = tl->hasBridge();
        if(hb) tiles.push_back(tl);
        else break;
        tl = tl->topLeft<eTile>();
    }
    while(tr) {
        const bool hb = tr->hasBridge();
        if(hb) tiles.push_back(tr);
        else break;
        tr = tr->topRight<eTile>();
    }
    while(br) {
        const bool hb = br->hasBridge();
        if(hb) tiles.push_back(br);
        else break;
        br = br->bottomRight<eTile>();
    }
    while(bl) {
        const bool hb = bl->hasBridge();
        if(hb) tiles.push_back(bl);
        else break;
        bl = bl->bottomLeft<eTile>();
    }
}

void eRoad::write(eWriteStream &dst) const {
    eBuilding::write(dst);
    dst << mRoadblock;
    dst.writeBuilding(mUnderAgora);
    dst.writeBuilding(mUnderGatehouse);
    dst.writeBuilding(mAboveHippodrome);
    dst << mCharacterAltitude;
}

void eRoad::read(eReadStream &src) {
    eBuilding::read(src);
    auto& board = getBoard();
    src >> mRoadblock;
    src.readBuilding(&board, [this](eBuilding* const bb) {
        setUnderAgora(static_cast<eAgoraBase*>(bb));
    });
    src.readBuilding(&board, [this](eBuilding* const bb) {
        setUnderGatehouse(static_cast<eGatehouse*>(bb));
    });
    src.readBuilding(&board, [this](eBuilding* const bb) {
        setAboveHippodrome(static_cast<eHippodromePiece*>(bb));
    });
    src >> mCharacterAltitude;
}
