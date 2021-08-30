#include "etiletotexture.h"

#include <algorithm>

#include "engine/etile.h"

#include "eterraintextures.h"

#include "ebeachtodry.h"
#include "ewatertodry.h"
#include "efertiletodry.h"
#include "eforesttodry.h"
#include "ewatertobeach.h"
#include "ewatercorner.h"
#include "estonestodry.h"

#include "buildings/ebuilding.h"

std::shared_ptr<eTexture> getStonesTexture(eTile* const tile,
                          const eTextureCollection& small,
                          const eTextureCollection& large,
                          const eTextureCollection& huge,
                          int& futureDim, int& drawDim) {
    eStonesToDry::get(tile, futureDim, drawDim);
    if(drawDim == 0) {
        return std::shared_ptr<eTexture>();
    } else if(drawDim == 1) {
        const auto& coll = small;
        const int texId = tile->seed() % coll.size();
        return coll.getTexture(texId);
    } else if(drawDim == 2) {
        const auto& coll = large;
        const int texId = tile->seed() % coll.size();
        return coll.getTexture(texId);
    } else if(drawDim == 3) {
        const auto& coll = huge;
        const int texId = tile->seed() % coll.size();
        return coll.getTexture(texId);
    }
    return std::shared_ptr<eTexture>();
}

std::shared_ptr<eTexture> eTileToTexture::get(eTile* const tile,
                             const eTerrainTextures& textures,
                             const eTileSize tileSize,
                             const bool drawElev,
                             int& futureDim,
                             int& drawDim) {
    drawDim = 1;
    futureDim = 1;
    const int seed = tile->seed();

    const bool hr = tile->hasRoad();

    if(drawElev) {
        const auto tr = tile->topRight();
        const auto r = tile->right();
        const auto br = tile->bottomRight();
        const auto b = tile->bottom();
        const auto bl = tile->bottomLeft();
        const auto l = tile->left();
        const auto tl = tile->topLeft();
        const auto t = tile->top();

        const int a = tile->altitude();

        const int tra = tr ? tr->altitude() : a;
        const int ra = r ? r->altitude() : a;
        const int bra = br ? br->altitude() : a;
        const int ba = b ? b->altitude() : a;
        const int bla = bl ? bl->altitude() : a;
        const int la = l ? l->altitude() : a;
        const int tla = tl ? tl->altitude() : a;
        const int ta = t ? t->altitude() : a;

        const auto& elev = textures.fElevation;
        const bool w = tile->walkableElev();
        if(tra > a && tla > a) {
            return elev.getTexture(8);
        } else if(tra > a && bra > a) {
            return elev.getTexture(9);
        } else if(bla > a && bra > a) {
            return elev.getTexture(10);
        } else if(bla > a && tla > a) {
            return elev.getTexture(11);
        } else if(bla > a) {
            if(w) return elev.getTexture(12 + (hr ? 4 : 0));
            else return elev.getTexture(1);
        } else if(tla > a) {
            if(w) return elev.getTexture(13 + (hr ? 4 : 0));
            else return elev.getTexture(3);
        } else if(tra > a) {
            if(w) return elev.getTexture(14 + (hr ? 4 : 0));
            else return elev.getTexture(5);
        } else if(bra > a) {
            if(w) return elev.getTexture(15 + (hr ? 4 : 0));
            else return elev.getTexture(7);
        } else if(la > a) {
            return elev.getTexture(2);
        } else if(ta > a) {
            return elev.getTexture(4);
        } else if(ra > a) {
            return elev.getTexture(6);
        } else if(ba > a) {
            return elev.getTexture(0);
        }
    }

    if(hr) return tile->building()->getTexture(tileSize);

    switch(tile->terrain()) {
    case eTerrain::dry: {
        const auto& vec = textures.fDryToScrubTerrainTexs;
        const int scrubCount = vec[0].size();
        const int scrub = tile->scrubId(scrubCount) - 1;
        if(scrub == -1) { // zero scrub
            const auto& coll = textures.fDryTerrainTexs;
            const int texId = seed % coll.size();
            return coll.getTexture(texId);
        } else if(scrub == scrubCount - 1) { // full scrub
            const auto& coll = textures.fScrubTerrainTexs;
            const int texId = seed % coll.size();
            return coll.getTexture(texId);
        } else { // partial scrub
            const int collId = seed % vec.size();
            const auto& coll = vec[collId];
            return coll.getTexture(scrub);
        }
    } break;
    case eTerrain::beach: {
        const int toDryId = eBeachToDry::get(tile);
        if(toDryId != -1) {
            const auto& coll = textures.fBeachToDryTerrainTexs;
            return coll.getTexture(toDryId);
        }
        const auto& coll = textures.fBeachTerrainTexs;
        const int texId = seed % coll.size();
        return coll.getTexture(texId);
    } break;
    case eTerrain::water: {
        const int cornerId = eWaterCorner::get(tile);
        if(cornerId != -1) {
            const auto& coll = textures.fWaterToBeachToDryTerrainTexs;
            return coll.getTexture(cornerId);
        }
        const int toBeachId = eWaterToBeach::get(tile);
        if(toBeachId != -1) {
            const auto& texs = textures.fWaterToBeachTerrainTexs[toBeachId];
            const int texId = seed % texs.size();
            return texs.getTexture(texId);
        }
        const int toDryId = eWaterToDry::get(tile);
        if(toDryId != -1) {
            const auto& texs = textures.fWaterToDryTerrainTexs[toDryId];
            const int texId = seed % texs.size();
            return texs.getTexture(texId);
        }

        const int texId = seed % textures.fWaterTerrainTexs.size();
        const auto& coll = textures.fWaterTerrainTexs;
        return coll.getTexture(texId);
    } break;
    case eTerrain::fertile: {
        const auto id = eFertileToDry::get(tile);
        const int scrubCount = textures.fFertileToScrubTerrainTexs.size();
        const int scrub = tile->scrubId(scrubCount) - 1;
        switch(id) {
        case eFertileToDryId::none: {
            if(scrub == -1) {
                const auto& coll = textures.fFertileTerrainTexs;
                const int texId = seed % coll.size();
                return coll.getTexture(texId);
            } else {
                const auto& coll = textures.fFertileToScrubTerrainTexs[2];
                return coll.getTexture(scrub);
            }
        } break;
        case eFertileToDryId::somewhere: {
            if(scrub == -1) {
                const auto& coll = textures.fFertileToDryTerrainTexs;
                const int texId = seed % coll.size();
                return coll.getTexture(texId);
            } else {
                const int collId = seed % 2;
                const auto& coll = textures.fFertileToScrubTerrainTexs[collId];
                return coll.getTexture(scrub);
            }
        } break;
        }
    } break;
    case eTerrain::forest: {
        const auto id = eForestToDry::get(tile);
        switch(id) {
        case eForestToDryId::none: {
            const auto& coll = textures.fForestTerrainTexs;
            const int texId = seed % coll.size();
            return coll.getTexture(texId);
        } break;
        case eForestToDryId::somewhere: {
            const int scrubCount = textures.fForestToScrubTerrainTexs.size();
            const int scrub = tile->scrubId(scrubCount) - 1;
            if(scrub == -1) {
                const auto& vec = textures.fForestToDryTerrainTexs;
                const int collId = seed % vec.size();
                const auto& coll = vec[collId];
                const int texId = seed % coll.size();
                return coll.getTexture(texId);
            } else {
                const auto& coll = textures.fForestToScrubTerrainTexs;
                return coll.getTexture(scrub);
            }
        } break;
        }
    } break;
    case eTerrain::choppedForest: {
        const int scrubCount = textures.fChoppedForestToScrubTerrainTexs.size();
        const int scrub = tile->scrubId(scrubCount) - 1;
        if(scrub == -1) {
            const auto& coll = textures.fChoppedForestTerrainTexs;
            const int texId = seed % coll.size();
            return coll.getTexture(texId);
        } else {
            const auto& coll = textures.fChoppedForestToScrubTerrainTexs;
            return coll.getTexture(scrub);
        }
    } break;


    case eTerrain::flatStones: {
        return getStonesTexture(tile, textures.fFlatStonesTerrainTexs,
                                textures.fLargeFlatStonesTerrainTexs,
                                textures.fHugeFlatStonesTerrainTexs,
                                futureDim, drawDim);
    } break;
    case eTerrain::copper: {
        return getStonesTexture(tile, textures.fBronzeTerrainTexs,
                                textures.fLargeBronzeTerrainTexs,
                                textures.fHugeBronzeTerrainTexs,
                                futureDim, drawDim);
    } break;
    case eTerrain::silver: {
        return getStonesTexture(tile, textures.fSilverTerrainTexs,
                                textures.fLargeSilverTerrainTexs,
                                textures.fHugeSilverTerrainTexs,
                                futureDim, drawDim);
    } break;
    case eTerrain::tallStones: {
        return getStonesTexture(tile, textures.fTallStoneTerrainTexs,
                                textures.fLargeTallStoneTerrainTexs,
                                textures.fHugeTallStoneTerrainTexs,
                                futureDim, drawDim);
    } break;
    case eTerrain::tinyStones: {
        const auto& coll = textures.fTinyStones;
        const int texId = seed % coll.size();
        return coll.getTexture(texId);
    } break;


    case eTerrain::dryBased:
        break;
    }
    return textures.fInvalidTex;
}
