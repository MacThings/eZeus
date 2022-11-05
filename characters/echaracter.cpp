#include "echaracter.h"

#include "engine/egameboard.h"

#include "actions/edieaction.h"
#include "actions/efightaction.h"

#include "gods/egod.h"
#include "heroes/ehero.h"
#include "audio/esounds.h"

eCharacter::eCharacter(eGameBoard& board,
                       const eCharacterType type) :
    eObject(board), eCharacterBase(type) {
    getBoard().registerCharacter(this);
}

eCharacter::~eCharacter() {
    getBoard().unregisterCharacter(this);
}

bool eCharacter::canFight(eCharacter* const c) {
    if(dead()) return false;
    if(playerId() == c->playerId()) return false;
    if(attack() == 0 && c->attack() == 0) return false;
    const auto ct = c->type();
    const auto tt = type();
    if(ct == eCharacterType::boar ||
       ct == eCharacterType::deer) {
        return tt == eCharacterType::hunter;
    }
    bool isGod;
    eGod::sCharacterToGodType(tt, &isGod);
    if(isGod) return false;
    bool isHero = false;
    eHero::sCharacterToHeroType(tt, &isHero);
    if(isHero) return false;
    bool isMonster1;
    eMonster::sCharacterToMonsterType(tt, &isMonster1);
    bool isMonster2;
    eMonster::sCharacterToMonsterType(ct, &isMonster2);
    if(isMonster1 && isMonster2) return false;
    return true;
}

void eCharacter::provideToBuilding(eBuilding* const b) {
    const int r = b->provide(mProvide, mProvideCount);
    mProvideCount -= r;
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

void eCharacter::kill() {
    changeTile(nullptr);
}

void eCharacter::killWithCorpse() {
    const stdptr<eCharacter> c(this);
    const auto finish = [c]() {
        if(!c) return;
        c->kill();
    };
    auto& board = getBoard();
    board.ifVisible(tile(), [&]() {
        eSounds::playDieSound(this);
    });
    const auto a = e::make_shared<eDieAction>(this, finish);
    setAction(a);
}

double eCharacter::absX() const {
    if(!mTile) return mX;
    return mX + mTile->x();
}

double eCharacter::absY() const {
    if(!mTile) return mY;
    return mY + mTile->y();
}

void eCharacter::changeTile(eTile* const t, const bool prepend) {
    const auto tsptr = ref<eCharacter>();
    if(mTile) mTile->removeCharacter(tsptr);
    mTile = t;
    if(t) t->addCharacter(tsptr, prepend);
    else deleteLater();
    if(t && mProvide != eProvide::none && mProvideCount > 0) {
        for(const int x : {-1, 0, 1}) {
            for(const int y : {-1, 0, 1}) {
                const auto tt = t->tileRel<eTile>(x, y);
                if(!tt) continue;
                if(const auto b = tt->underBuilding()) {
                    provideToBuilding(b);
                    if(mProvideCount <= 0) return;
                }
            }
        }
    }
}

void eCharacter::incTime(const int by) {
    mTime += by;
    const auto at = actionType();
    if(at == eCharacterActionType::fight ||
       at == eCharacterActionType::fight2) {
        mSoundPlayTime += by;
        const int soundPlayTime = 500;
        if(mSoundPlayTime > soundPlayTime) {
            mSoundPlayTime -= soundPlayTime;
            auto& board = getBoard();
            board.ifVisible(tile(), [&]() {
                if(rand() % 2) {
                    eSounds::playHitSound(this);
                } else {
                    eSounds::playAttackSound(this);
                }
            });
        }
    }
    if(mAction) {
        if(mAction->state() != eCharacterActionState::running) {
            mAction.reset();
            changeTile(nullptr);
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
    mAction.reset();
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

bool eCharacter::isSoldier() const {
    const auto t = type();
    return t == eCharacterType::hoplite ||
           t == eCharacterType::horseman ||
           t == eCharacterType::rockThrower;
}

bool eCharacter::defend(const double a) {
    if(dead()) return true;

    bool isMonster = false;
    eMonster::sCharacterToMonsterType(type(), &isMonster);
    if(isMonster) return false;;
    bool isGod = false;
    eGod::sCharacterToGodType(type(), &isGod);
    if(isGod) return false;

    setHP(hp() - a);
    if(hp() <= 0) {
        killWithCorpse();
        return true;
    } else {
        return false;
    }
    return dead();
}
