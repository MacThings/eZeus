#include "ewatermonster.h"

#include "textures/egametextures.h"

eWaterMonster::eWaterMonster(eGameBoard& board,
                             const eCharTexs charTexs,
                             const eMonsterType type) :
    eMonster(board, type), mCharTexs(charTexs) {}

std::shared_ptr<eTexture>
eWaterMonster::getTexture(const eTileSize size) const {
    const int id = static_cast<int>(size);
    const auto& gTexs = eGameTextures::characters();
    const auto& texs = gTexs[id].*mCharTexs;
    const eTextureCollection* coll = nullptr;
    bool reverse = false;
    bool wrap = true;
    const int oid = static_cast<int>(rotatedOrientation());
    const auto a = actionType();
    switch(a) {
    case eCharacterActionType::stand:
        return texs.fWalk[oid].getTexture(0);
    case eCharacterActionType::collect:
    case eCharacterActionType::fight:
    case eCharacterActionType::bless:
    case eCharacterActionType::curse: {
        wrap = true;
        coll = &texs.fFight[oid];
    } break;
    case eCharacterActionType::fight2: {
        wrap = true;
        coll = &texs.fFight2[oid];
    } break;
    case eCharacterActionType::carry:
    case eCharacterActionType::walk: {
        coll = &texs.fWalk[oid];
    } break;
    case eCharacterActionType::disappear:
    case eCharacterActionType::die:
        wrap = false;
        coll = &texs.fDie;
        break;
    default:
        return nullptr;
    }

    return eCharacter::getTexture(coll, wrap, reverse);
}
