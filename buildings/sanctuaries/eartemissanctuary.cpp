#include "eartemissanctuary.h"

#include "textures/ebuildingtextures.h"

eArtemisSanctuary::eArtemisSanctuary(
        const int sw, const int sh, eGameBoard& board) :
    eSanctuary(board, eBuildingType::templeArtemis, sw, sh, 60) {

}