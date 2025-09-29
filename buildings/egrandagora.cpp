#include "egrandagora.h"

int swGA(const eAgoraOrientation o) {
    switch(o) {
    case eAgoraOrientation::bottomLeft:
    case eAgoraOrientation::topRight:
        return 6;
    case eAgoraOrientation::bottomRight:
    case eAgoraOrientation::topLeft:
        return 5;
    }
    return 0;
}

int shGA(const eAgoraOrientation o) {
    switch(o) {
    case eAgoraOrientation::bottomLeft:
    case eAgoraOrientation::topRight:
        return 5;
    case eAgoraOrientation::bottomRight:
    case eAgoraOrientation::topLeft:
        return 6;
    }
    return 0;
}

eGrandAgora::eGrandAgora(const eAgoraOrientation o, eGameBoard& board,
                         const eCityId cid) :
    eAgoraBase(board, eBuildingType::grandAgora,
               swGA(o), shGA(o), o, cid, 6) {}

SDL_Point eGrandAgora::pt(const int id) const {
    const auto rect = tileRect();
    const int rx = rect.x;
    const int ry = rect.y;
    switch(mO) {
    case eAgoraOrientation::bottomRight:
    case eAgoraOrientation::topLeft: {
        if(id < 3) return eAgoraBase::pt(rx, ry, eAgoraOrientation::topLeft, id);
        else return eAgoraBase::pt(rx + 2, ry, eAgoraOrientation::bottomRight, id - 3);
    } break;
    case eAgoraOrientation::bottomLeft:
    case eAgoraOrientation::topRight: {
        if(id < 3) return eAgoraBase::pt(rx, ry, eAgoraOrientation::topRight, id);
        else return eAgoraBase::pt(rx, ry + 2, eAgoraOrientation::bottomLeft, id - 3);
    } break;
    }
    return {0, 0};
}
