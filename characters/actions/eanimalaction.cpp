#include "eanimalaction.h"

#include "emovearoundaction.h"
#include "characters/echaracter.h"
#include "ewaitaction.h"
#include "enumbers.h"

eAnimalAction::eAnimalAction(eCharacter* const c,
                             const int spawnerX, const int spawnerY,
                             const stdsptr<eWalkableObject>& tileWalkable,
                             const eCharActionType type) :
    eComplexAction(c, type),
    mSpawnerX(spawnerX), mSpawnerY(spawnerY),
    mTileWalkable(tileWalkable) {}

eAnimalAction::eAnimalAction(eCharacter* const c) :
    eAnimalAction(c, 0, 0) {}

bool eAnimalAction::decide() {
    const auto c = character();
    if(eRand::rand() % 2 == 0) {
        c->setActionType(eCharacterActionType::walk);
        const auto m = e::make_shared<eMoveAroundAction>(
                           c, mSpawnerX, mSpawnerY,
                           mTileWalkable);
        m->setMaxDistance(eNumbers::sAnimalMoveRange);
        m->setTime(mWalkTime);
        setCurrentAction(m);
    } else {
        c->setActionType(eCharacterActionType::lay);
        const auto w = e::make_shared<eWaitAction>(c);
        w->setTime(mLayTime);
        setCurrentAction(w);
    }
    return true;
}

void eAnimalAction::read(eReadStream& src) {
    eComplexAction::read(src);
    src >> mSpawnerX;
    src >> mSpawnerY;
    mTileWalkable = src.readWalkable();
    src >> mLayTime;
    src >> mWalkTime;
}

void eAnimalAction::write(eWriteStream& dst) const {
    eComplexAction::write(dst);
    dst << mSpawnerX;
    dst << mSpawnerY;
    dst.writeWalkable(mTileWalkable.get());
    dst << mLayTime;
    dst << mWalkTime;
}
