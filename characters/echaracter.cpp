#include "echaracter.h"

#include "engine/egameboard.h"

#include "actions/edieaction.h"
#include "actions/efightaction.h"

eCharacter::eCharacter(eGameBoard& board, const eCharacterType type) :
    eCharacterBase(type), mBoard(board) {
    mBoard.registerCharacter(this);
}

eCharacter::~eCharacter() {
    mBoard.unregisterCharacter(this);
}

bool eCharacter::canFight(eCharacter* const c) {
    if(dead()) return false;
    if(playerId() == c->playerId()) return false;
    return true;
}

void eCharacter::fight(eCharacter* const c) {
    const auto a = takeAction();
    a->pause();
    setAction(e::make_shared<eFightAction>(this, c, [this, a]() {
        if(dead()) {
            setAction(e::make_shared<eDieAction>(this, [a]() {
                a->setState(eCharacterActionState::failed);
            }));
        } else {
            const auto aa = a;
            setAction(aa);
            aa->resume();
        }
    }));
}

void eCharacter::changeTile(eTile* const t) {
    const auto tsptr = ref<eCharacter>();
    if(mTile) mTile->removeCharacter(tsptr);
    mTile = t;
    if(t) t->addCharacter(tsptr);

    if(t && mProvide != eProvide::none && mProvideCount > 0) {
        for(const int x : {-1, 0, 1}) {
            for(const int y : {-1, 0, 1}) {
                const auto tt = t->tileRel<eTile>(x, y);
                if(!tt) continue;
                if(const auto b = tt->underBuilding()) {
                    const int r = b->provide(mProvide, mProvideCount);
                    mProvideCount -= r;
                    if(mProvideCount <= 0) return;
                }
            }
        }
    }
}

void eCharacter::incTime(const int by) {
    mTime += by;
    if(mAction) {
        if(mAction->state() != eCharacterActionState::running) {
            changeTile(nullptr);
            mAction.reset();
        } else {
            mAction->increment(by);
        }
    } else {
        changeTile(nullptr);
    }
}

void eCharacter::setOrientation(const eOrientation o) {
    mOrientation = o;
}

void eCharacter::setAction(const stdsptr<eCharacterAction>& a) {
    mAction = a;
}

stdsptr<eCharacterAction> eCharacter::takeAction() {
    const auto a = mAction;
    mAction .reset();
    return a;
}

void eCharacter::setActionType(const eCharacterActionType t) {
    mActionStartTime = textureTime();
    eCharacterBase::setActionType(t);
}

void eCharacter::setHasSecondaryTexture(const bool st) {
    mHasSecondaryTexture = st;
}

void eCharacter::setProvide(const eProvide p, const int n) {
    mProvide = p;
    mProvideCount = n;
}
