﻿#include "eactionwithcomeback.h"

#include "characters/echaracter.h"
#include "emovetoaction.h"

#include "engine/etask.h"

eActionWithComeback::eActionWithComeback(
        eCharacter* const c,
        eTile* const startTile,
        const eCharActionType type) :
    eComplexAction(c, type),
    mStartTile(startTile) {}

eActionWithComeback::eActionWithComeback(eCharacter* const c,
                                         const eCharActionType type) :
    eActionWithComeback(c, c->tile(), type) {}

bool eActionWithComeback::decide() {
    if(mGoBackFail) {
        mGoBackFail = false;
        teleportDecision();
        return true;
    }
    return false;
}

void eActionWithComeback::read(eReadStream& src) {
    eComplexAction::read(src);
    mStartTile = src.readTile(board());
    src >> mGoBackRect;
    src >> mFinishOnComeback;
    src >> mDefaultTry;
    src >> mGoBackFail;
    src >> mDiagonalOnly;
}

void eActionWithComeback::write(eWriteStream& dst) const {
    eComplexAction::write(dst);
    dst.writeTile(mStartTile);
    dst << mGoBackRect;
    dst << mFinishOnComeback;
    dst << mDefaultTry;
    dst << mGoBackFail;
    dst << mDiagonalOnly;
}

void eActionWithComeback::goBack(stdsptr<eWalkableObject> walkable) {
    mDefaultTry = false;
    mGoBackFail = false;
    goBackInternal(walkable);
}

void eActionWithComeback::goBack(eBuilding* const b,
                                 const stdsptr<eWalkableObject>& walkable) {
    const auto type = b->type();
    if(type == eBuildingType::commonAgora ||
       type == eBuildingType::grandAgora) {
        goBack(walkable);
    } else {
        const auto rect = b->tileRect();
        goBack(rect, walkable);
    }
}

void eActionWithComeback::goBack(const SDL_Rect& rect,
                                 const stdsptr<eWalkableObject>& walkable) {
    mGoBackRect = rect;
    const auto w = eWalkableObject::sCreateRect(rect, walkable);
    eActionWithComeback::goBack(w);
}

void eActionWithComeback::goBackInternal(stdsptr<eWalkableObject> walkable) {
    if(mGoBackFail) return;
    if(mDefaultTry) {
        if(SDL_RectEmpty(&mGoBackRect)) {
            walkable = eWalkableObject::sCreateDefault();
        } else {
            walkable = eWalkableObject::sCreateRect(
                    mGoBackRect, eWalkableObject::sCreateDefault());
        }
    }
    const auto c = character();
    const auto ct = c->tile();
    if(ct == mStartTile) return teleportDecision();

    const int startX = mStartTile->x();
    const int startY = mStartTile->y();
    const auto finalTile = [startX, startY](eTileBase* const t) {
        return t->x() == startX && t->y() == startY;
    };
    const auto failFunc = std::make_shared<eAWC_goBackFail>(board(), this, walkable);

    stdsptr<eAWC_goBackFinish> finishFunc;
    if(mFinishOnComeback) {
        finishFunc = std::make_shared<eAWC_goBackFinish>(board(), this);
    }
    const auto a = e::make_shared<eMoveToAction>(c);
    a->setStateRelevance(eStateRelevance::buildings |
                         eStateRelevance::terrain);
    a->setFinishAction(finishFunc);
    a->setFailAction(failFunc);

    const stdptr<eActionWithComeback> tptr(this);
    a->setFindFailAction([tptr, this, walkable]() {
        if(!tptr) return;
        if(mDefaultTry) {
            mGoBackFail = true;
        } else {
            mDefaultTry = true;
        }
    });
    a->setDiagonalOnly(mDiagonalOnly);
    if(const auto ub = mStartTile->underBuilding()) {
        walkable = eWalkableObject::sCreateRect(ub, walkable);
    }
    a->start(finalTile, walkable);
    setCurrentAction(a);
}

void eActionWithComeback::teleportDecision() {
    const auto c = character();

    c->changeTile(mStartTile);
    c->setX(0.5);
    c->setY(0.5);

    if(mFinishOnComeback) {
        setState(eCharacterActionState::finished);
    }
}
