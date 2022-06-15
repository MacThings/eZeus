#include "eroad.h"

#include "textures/egametextures.h"

#include "engine/egameboard.h"

eRoad::eRoad(eGameBoard& board) :
    eBuilding(board, eBuildingType::road, 1, 1),
    mTextures(eGameTextures::buildings()) {

}

std::shared_ptr<eTexture> eRoad::getTexture(const eTileSize size) const {
    const int sizeId = static_cast<int>(size);
    const auto& sizeColl = mTextures[sizeId];
    const auto& tbcoll = sizeColl.fToBeachRoad;
    const auto& bcoll = sizeColl.fBeachRoad;

    const auto ti = centerTile();
    const auto tr = ti->topRight();
    const auto br = ti->bottomRight();
    const auto bl = ti->bottomLeft();
    const auto tl = ti->topLeft();

    const bool trRoad = !tr || tr->hasRoad();
    const bool brRoad = !br || br->hasRoad();
    const bool blRoad = !bl || bl->hasRoad();
    const bool tlRoad = !tl || tl->hasRoad();

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

        const auto t = ti->top<eTile>();
        const auto b = ti->bottom<eTile>();
        const auto l = ti->left<eTile>();
        const auto r = ti->right<eTile>();

        const auto tt = t ? t->underBuildingType() : eBuildingType::none;
        const auto bt = b ? b->underBuildingType() : eBuildingType::none;
        const auto lt = l ? l->underBuildingType() : eBuildingType::none;
        const auto rt = r ? r->underBuildingType() : eBuildingType::none;

        const bool tb = tt == eBuildingType::avenue ||
                        tt == eBuildingType::road;
        const bool lb = bt == eBuildingType::avenue ||
                        bt == eBuildingType::road;
        const bool bb = lt == eBuildingType::avenue ||
                        lt == eBuildingType::road;
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
        return sizeColl.fAvenueRoad.getTexture(id);
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
        const auto& pcoll = sizeColl.fPrettyRoad;
        const auto& rcoll = sizeColl.fRoad;

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
