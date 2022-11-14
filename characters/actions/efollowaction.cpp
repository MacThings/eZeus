#include "efollowaction.h"

#include "../echaracter.h"

#include "emovepathaction.h"

eFollowAction::eFollowAction(eCharacter* const f,
                             eCharacter* const c) :
    eComplexAction(c, eCharActionType::followAction),
    mFollow(f) {
    c->setActionType(eCharacterActionType::stand);
}

eFollowAction::eFollowAction(eCharacter* const c) :
    eFollowAction(nullptr, c) {}

void eFollowAction::setDistance(const int d) {
    mDistance = d;
}

void eFollowAction::read(eReadStream& src) {
    eComplexAction::read(src);
    src.readCharacter(&board(), [this](eCharacter* const c) {
        mFollow = c;
    });
    src >> mDistance;
    int s;
    src >> s;
    for(int i = 0; i < s; i++) {
        ePathNode n;
        n.fTile = src.readTile(board());
        src >> n.fO;
        mTiles.push_back(n);
    }
}

void eFollowAction::write(eWriteStream& dst) const {
    eComplexAction::write(dst);
    dst.writeCharacter(mFollow);
    dst << mDistance;
    dst << mTiles.size();
    for(const auto& t : mTiles) {
        dst.writeTile(t.fTile);
        dst << t.fO;
    }
}

void eFollowAction::increment(const int by) {
    const auto ft = mFollow ? mFollow->tile() : nullptr;
    const auto c = character();
    if(!ft) {
        const auto ca = currentAction();
        if(ca) return eComplexAction::increment(by);
        if(mTiles.empty()) {
            return setState(eCharacterActionState::finished);
        } else {
            const auto walkable = [](eTileBase* const) { return true; };
            std::vector<eOrientation> path;
            path.reserve(mTiles.size());
            for(int i = 0; i < mDistance && !mTiles.empty(); i++) {
                const auto& f = mTiles.front();
                path.push_back(f.fO);
                mTiles.pop_front();
            }
            mTiles = {};
            const auto a = e::make_shared<eMovePathAction>(c, path, walkable);
            setCurrentAction(a);
        }
        return eComplexAction::increment(by);
    }
    if(!mTiles.empty()) {
        auto& b = mTiles.back();
        if(ft == b.fTile) {
            b.fO = mFollow->orientation();
            eComplexAction::increment(by);
            return;
        }
    }
    mTiles.push_back({ft, mFollow->orientation()});
    const int nt = mTiles.size();
    if(nt < mDistance + 2) return;
    if(nt > mDistance + 2) mTiles.pop_front();
    const auto pn = mTiles.front();
    c->setActionType(eCharacterActionType::walk);
    c->changeTile(pn.fTile);
    c->setOrientation(pn.fO);
    const auto walkable = [](eTileBase* const) { return true; };
    const std::vector<eOrientation> path = {pn.fO};
    const auto a = e::make_shared<eMovePathAction>(c, path, walkable);
    setCurrentAction(a);
    eComplexAction::increment(by);
}
