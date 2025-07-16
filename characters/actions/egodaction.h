#ifndef EGODACTION_H
#define EGODACTION_H

#include "egodmonsteraction.h"

#include "characters/gods/egod.h"
#include "engine/egameboard.h"
#include "audio/esounds.h"
#include "buildings/esmallhouse.h"

class eDestructionTextures;
enum class eGodSound;

enum class eGodActType {
    lookForSoldierAttack,
    lookForBless,
    playMonsterBuildingAttackSound,
    playFightGodHitSoundGodAct,
    lookForAttack,

    provideResource,
    provideAgora,
    provideDrachmas,
    atlasHelp,
    apolloHelp,
    aphroditeHelp,

    lookForPlague,
    lookForEvict,
    lookForTargetedBless,
    lookForTargetedAttack
};

class eGodAct {
public:
    eGodAct(eGameBoard& board, const eGodActType type) :
        mType(type), mBoard(board) {}

    virtual eMissileTarget find(eTile* const t) = 0;
    virtual void act() = 0;

    eGodActType type() const { return mType; }
    eGameBoard& board() { return mBoard; }

    virtual void read(eReadStream& src) = 0;
    virtual void write(eWriteStream& dst) const = 0;

    static stdsptr<eGodAct> sCreate(eGameBoard& board, const eGodActType t);
private:
    const eGodActType mType;
    eGameBoard& mBoard;
};

class ePlayMonsterBuildingAttackSoundGodAct : public eGodAct {
public:
    ePlayMonsterBuildingAttackSoundGodAct(
            eGameBoard& board, eBuilding* const b) :
        eGodAct(board, eGodActType::playMonsterBuildingAttackSound),
        mBuilding(b) {}
    ePlayMonsterBuildingAttackSoundGodAct(eGameBoard& board) :
        ePlayMonsterBuildingAttackSoundGodAct(board, nullptr) {}

    eMissileTarget find(eTile* const t) {
        (void)t;
        const auto null = static_cast<eTile*>(nullptr);
        return null;
    }

    void act() {
        if(!mBuilding) return;
        const auto b = mBuilding.get();
        auto& board = b->getBoard();
        board.ifVisible(b->centerTile(), [&]() {
            eSounds::playFireballHitSound();
        });
    }

    void read(eReadStream& src) {
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mBuilding = b;
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeBuilding(mBuilding);
    }
private:
    stdptr<eBuilding> mBuilding;
};

class ePlayFightGodHitSoundGodAct : public eGodAct {
public:
    ePlayFightGodHitSoundGodAct(eGameBoard& board, eGod* const g) :
        eGodAct(board, eGodActType::playFightGodHitSoundGodAct),
        mG(g) {}
    ePlayFightGodHitSoundGodAct(eGameBoard& board) :
        ePlayFightGodHitSoundGodAct(board, nullptr) {}

    eMissileTarget find(eTile* const t) {
        (void)t;
        const auto null = static_cast<eTile*>(nullptr);
        return null;
    }

    void act() {
        if(!mG) return;
        board().ifVisible(mG->tile(), [&]() {
            eSounds::playGodSound(mG->type(), eGodSound::hit);
        });
    }

    void read(eReadStream& src) {
        src.readCharacter(&board(), [this](eCharacter* const g) {
            mG = static_cast<eGod*>(g);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacter(mG);
    }
private:
    stdptr<eGod> mG;
};

class eLookForPlagueGodAct : public eGodAct {
public:
    eLookForPlagueGodAct(eGameBoard& board) :
        eGodAct(board, eGodActType::lookForPlague) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto b = t->underBuilding();
        if(!b) return null;
        const auto type = b->type();
        if(type != eBuildingType::commonHouse) return null;
        const auto ch = static_cast<eSmallHouse*>(b);
        const bool p = ch->plague();
        if(p) return null;
        const auto tile = b->centerTile();
        const int tx = tile->x();
        const int ty = tile->y();
        int dist;
        auto& board = this->board();
        const auto cid = tile->cityId();
        board.nearestPlague(cid, tx, ty, dist);
        if(dist < 10) return null;
        mTarget = static_cast<eSmallHouse*>(b);
        return tile;
    }

    void act() {
        if(mTarget) {
            auto& board = this->board();
            board.startPlague(mTarget);
        }
    }

    void read(eReadStream& src) {
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mTarget = static_cast<eSmallHouse*>(b);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeBuilding(mTarget);
    }
private:
    stdptr<eSmallHouse> mTarget;
};

class eLookForEvictGodAct : public eGodAct {
public:
    eLookForEvictGodAct(eGameBoard& board) :
        eGodAct(board, eGodActType::lookForEvict) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto b = t->underBuilding();
        if(!b) return null;
        const auto type = b->type();
        if(type != eBuildingType::commonHouse &&
           type != eBuildingType::eliteHousing) {
            return null;
        }
        const auto hb = static_cast<eHouseBase*>(b);
        if(hb->people() <= 0) return null;
        mTarget = hb;
        return b->centerTile();
    }

    void act() {
        if(mTarget) {
            mTarget->leave();
        }
    }

    void read(eReadStream& src) {
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mTarget = static_cast<eHouseBase*>(b);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeBuilding(mTarget);
    }
private:
    stdptr<eHouseBase> mTarget;
};


class eLookForBlessGodActBase : public eGodAct {
public:
    using eGodAct::eGodAct;

    void act() {
        if(mTarget) {
            const auto type = mTarget->type();
            const bool batch = type == eBuildingType::oliveTree ||
                               type == eBuildingType::vine ||
                               type == eBuildingType::orangeTree;
            if(batch) {
                auto& board = this->board();
                const auto tile = mTarget->centerTile();
                const int tx = tile->x();
                const int ty = tile->y();
                for(int x = tx - 3; x <= tx + 3; x++) {
                    for(int y = ty - 3; y <= ty + 3; y++) {
                        const auto tile = board.tile(x, y);
                        if(!tile) continue;
                        const auto ub = tile->underBuilding();
                        if(!ub) continue;
                        const auto utype = ub->type();
                        if(type == utype) ub->setBlessed(mBless);
                    }
                }
            }
            mTarget->setBlessed(mBless);
        }
    }

    void read(eReadStream& src) {
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mTarget = b;
        });
        src >> mBless;
    }

    void write(eWriteStream& dst) const {
        dst.writeBuilding(mTarget);
        dst << mBless;
    }
protected:
    stdptr<eBuilding> mTarget;
    double mBless = 0;
};

class eLookForTargetedBlessGodAct : public eLookForBlessGodActBase {
public:
    eLookForTargetedBlessGodAct(eGameBoard& board, const double bless,
                                const eGodType type) :
        eLookForBlessGodActBase(board, eGodActType::lookForTargetedBless),
        mType(type) {
        mBless = bless;
    }
    eLookForTargetedBlessGodAct(eGameBoard& board) :
        eLookForTargetedBlessGodAct(board, 0, eGodType::zeus) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto b = t->underBuilding();
        if(!b) {
            return null;
        }
        const auto type = b->type();
        if(!eBuilding::sBlessable(type)) {
            return null;
        }
        if(!eGod::sTarget(mType, type)) {
            return null;
        }
        if(b->blessed() || b->cursed()) {
            return null;
        }
        mTarget = b;
        return b->centerTile();
    }

    void read(eReadStream& src) {
        eLookForBlessGodActBase::read(src);
        src >> mType;
    }

    void write(eWriteStream& dst) const {
        eLookForBlessGodActBase::write(dst);
        dst << mType;
    }
private:
    eGodType mType;
};

class eLookForBlessGodAct : public eLookForBlessGodActBase {
public:
    eLookForBlessGodAct(eGameBoard& board, const double bless) :
        eLookForBlessGodActBase(board, eGodActType::lookForBless) {
        mBless = bless;
    }
    eLookForBlessGodAct(eGameBoard& board) :
        eLookForBlessGodAct(board, 0) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto b = t->underBuilding();
        if(!b) return null;
        if(!eBuilding::sBlessable(b->type())) return null;
        if(b->blessed() || b->cursed()) return null;
        mTarget = b;
        return b->centerTile();
    }
};

class eLookForSoldierAttackGodAct : public eGodAct {
public:
    eLookForSoldierAttackGodAct(eGameBoard& board, const eTeamId team) :
        eGodAct(board, eGodActType::lookForSoldierAttack),
        mGodTeam(team) {}
    eLookForSoldierAttackGodAct(eGameBoard& board) :
        eLookForSoldierAttackGodAct(board, eTeamId::neutralFriendly) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto& chars = t->characters();
        if(chars.empty()) return null;
        for(const auto& cc : chars) {
            if(cc->dead()) continue;
            const bool is = cc->isSoldier();
            if(!is) continue;
            const auto cctid = cc->teamId();
            if(!eTeamIdHelpers::isEnemy(cctid, mGodTeam)) continue;
            mTarget = cc;
            return cc.get();
        }
        return null;
    }

    void act() {
        if(mTarget && !mTarget->dead()) {
            mTarget->killWithCorpse();
        }
    }

    void read(eReadStream& src) {
        src >> mGodTeam;
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mTarget = c;
        });
    }

    void write(eWriteStream& dst) const {
        dst << mGodTeam;
        dst.writeCharacter(mTarget);
    }
private:
    eTeamId mGodTeam;
    stdptr<eCharacter> mTarget;
};

class eLookForTargetedAttackGodAct : public eGodAct {
public:
    eLookForTargetedAttackGodAct(eGameBoard& board,
                                 const eGodType type) :
        eGodAct(board, eGodActType::lookForTargetedAttack),
        mType(type) {}
    eLookForTargetedAttackGodAct(eGameBoard& board) :
        eLookForTargetedAttackGodAct(board, eGodType::zeus) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        const auto b = t->underBuilding();
        if(!b) return null;
        const auto type = b->type();
        if(!eBuilding::sAttackable(type)) return null;
        const bool target = eGod::sTarget(mType, type);
        if(!target) return null;
        return b->centerTile();
    }

    void act() {
        if(mBTarget) {
            mBTarget->collapse();
            eSounds::playCollapseSound();
        }
    }

    void read(eReadStream& src) {
        src >> mType;
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mBTarget = b;
        });
    }

    void write(eWriteStream& dst) const {
        dst << mType;
        dst.writeBuilding(mBTarget);
    }
private:
    eGodType mType;
    stdptr<eBuilding> mBTarget;
};

class eLookForAttackGodAct : public eGodAct {
public:
    eLookForAttackGodAct(eGameBoard& board) :
        eGodAct(board, eGodActType::lookForAttack) {}
    eLookForAttackGodAct(eGameBoard& board, eCharacter* const c) :
        eGodAct(board, eGodActType::lookForAttack),
        mCptr(c) {}

    eMissileTarget find(eTile* const t) {
        const auto null = static_cast<eTile*>(nullptr);
        if(!mCptr) return null;
        if(mCptr->tile() == t) return null;
        const auto b = t->underBuilding();
        if(b && eBuilding::sAttackable(b->type())) {
            mBTarget = b;
            return b->centerTile();
        } else {
            const auto& chars = t->characters();
            if(chars.empty()) return null;
            for(const auto& cc : chars) {
                if(mCptr == cc.get()) continue;
                bool isGod = false;
                eGod::sCharacterToGodType(cc->type(), &isGod);
                if(isGod) continue;
                bool isMonster = false;
                eMonster::sCharacterToMonsterType(cc->type(), &isMonster);
                if(isMonster) continue;
                bool isHero = false;
                eHero::sCharacterToHeroType(cc->type(), &isHero);
                if(isHero) continue;
                mCTarget = cc;
                return cc.get();
            }
            return null;
        }
    }

    void act() {
        if(!mCptr) return;
        if(mBTarget) {
            const auto type = mCptr->type();
            if(type == eCharacterType::hephaestus) {
                mBTarget->setOnFire(true);
            } else {
                mBTarget->collapse();
                eSounds::playCollapseSound();
            }
        } else if(mCTarget && !mCTarget->dead()) {
            mCTarget->killWithCorpse();
        }
    }

    void read(eReadStream& src) {
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mCptr = c;
        });
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mCTarget = c;
        });
        src.readBuilding(&board(), [this](eBuilding* const b) {
            mBTarget = b;
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacter(mCptr);
        dst.writeCharacter(mCTarget);
        dst.writeBuilding(mBTarget);
    }
private:
    stdptr<eCharacter> mCptr;
    stdptr<eCharacter> mCTarget;
    stdptr<eBuilding> mBTarget;
};

class eGodAction : public eGodMonsterAction {
public:
    eGodAction(eCharacter* const c, const eCharActionType type);

    using eTexPtr = eTextureCollection eDestructionTextures::*;
    bool lookForRangeAction(const int dtime,
                            int& time, const int freq,
                            const int range,
                            const eCharacterActionType at,
                            const stdsptr<eGodAct>& act,
                            const eCharacterType chart,
                            const eGodSound missileSound,
                            const int nMissiles = 1);
    bool lookForBlessCurse(const int dtime, int& time,
                           const int freq, const int range,
                           const double bless);
    bool lookForTargetedBlessCurse(const int dtime, int& time,
                                   const int freq, const int range,
                                   const double bless);

    bool lookForSoldierAttack(const int dtime, int& time,
                              const int freq, const int range);

    void spawnGodMissile(const eCharacterActionType at,
                         const eCharacterType chart,
                         const eMissileTarget& target,
                         const eGodSound sound,
                         const stdsptr<eGodAct>& act,
                         const stdsptr<eCharActFunc>& finishAttackA = nullptr);

    void goBackToSanctuary();
    void goToTarget();

    eGodType type() const { return mType; }
private:
    const eGodType mType;
};

class eGA_lookForSoldierAttackFinish : public eCharActFunc {
public:
    eGA_lookForSoldierAttackFinish(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::GA_lookForSoldierAttackFinish) {}
    eGA_lookForSoldierAttackFinish(eGameBoard& board, eGodAction* const ca,
                                   eTile* const tile) :
        eCharActFunc(board, eCharActFuncType::GA_lookForSoldierAttackFinish),
        mTptr(ca), mTile(tile) {}

    void call() override {
        if(!mTptr) return;
        const auto t = mTptr.get();
        const auto c = t->character();
        c->changeTile(mTile);
        t->appear();
    }

    void read(eReadStream& src) override {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eGodAction*>(ca);
        });
        mTile = src.readTile(board());
    }

    void write(eWriteStream& dst) const override {
        dst.writeCharacterAction(mTptr);
        dst.writeTile(mTile);
    }
private:
    stdptr<eGodAction> mTptr;
    eTile* mTile = nullptr;
};

class eGA_lookForRangeActionFinish : public eCharActFunc {
public:
    eGA_lookForRangeActionFinish(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::GA_lookForRangeActionFinish) {}
    eGA_lookForRangeActionFinish(eGameBoard& board, eGodAction* const ca) :
        eCharActFunc(board, eCharActFuncType::GA_lookForRangeActionFinish),
        mTptr(ca) {}

    void call() override {
        if(!mTptr) return;
        mTptr->resumeAction();
    }

    void read(eReadStream& src) override {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eGodAction*>(ca);
        });
    }

    void write(eWriteStream& dst) const override {
        dst.writeCharacterAction(mTptr);
    }
private:
    stdptr<eGodAction> mTptr;
};

class eGA_teleportFinish : public eCharActFunc {
public:
    eGA_teleportFinish(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::GA_teleportFinish) {}
    eGA_teleportFinish(eGameBoard& board, eGodMonsterAction* const ca,
                       eTile* const tile) :
        eCharActFunc(board, eCharActFuncType::GA_teleportFinish),
        mTptr(ca), mTile(tile) {}

    void call() override {
        if(!mTptr) return;
        const auto t = mTptr.get();
        const auto c = t->character();
        c->changeTile(mTile);
        t->appear();
    }

    void read(eReadStream& src) override {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eGodMonsterAction*>(ca);
        });
        mTile = src.readTile(board());
    }

    void write(eWriteStream& dst) const override {
        dst.writeCharacterAction(mTptr);
        dst.writeTile(mTile);
    }
private:
    stdptr<eGodMonsterAction> mTptr;
    eTile* mTile = nullptr;
};

class eGA_hermesRunFinish : public eCharActFunc {
public:
    eGA_hermesRunFinish(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::GA_hermesRunFinish) {}
    eGA_hermesRunFinish(eGameBoard& board, eGodMonsterAction* const ca,
                        eCharacter* const c, const bool appear) :
        eCharActFunc(board, eCharActFuncType::GA_hermesRunFinish),
        mTptr(ca), mCptr(c), mAppear(appear) {}

    void call() override {
        if(!mTptr || !mCptr) return;
        const auto t = mTptr.get();
        mCptr->setSpeed(1.);
        if(!mAppear) t->playDisappearSound();
    }

    void read(eReadStream& src) override {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eGodMonsterAction*>(ca);
        });
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mCptr = c;
        });

        src >> mAppear;
    }

    void write(eWriteStream& dst) const override {
        dst.writeCharacterAction(mTptr);
        dst.writeCharacter(mCptr);

        dst << mAppear;
    }
private:
    stdptr<eGodMonsterAction> mTptr;
    stdptr<eCharacter> mCptr;
    bool mAppear;
};

class eGA_spawnGodMissilePlaySound : public eCharActFunc {
public:
    eGA_spawnGodMissilePlaySound(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::GA_spawnGodMissilePlaySound) {}
    eGA_spawnGodMissilePlaySound(eGameBoard& board, const eGodSound sound,
                                 eCharacter* const c) :
        eCharActFunc(board, eCharActFuncType::GA_spawnGodMissilePlaySound),
        mCptr(c), mSound(sound) {}

    void call() override {
        if(!mCptr) return;
        const auto c = mCptr.get();
        board().ifVisible(c->tile(), [&]() {
            const auto ct = c->type();
            const auto gt = eGod::sCharacterToGodType(ct);
            eSounds::playGodSound(gt, mSound);
        });
    }

    void read(eReadStream& src) override {
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mCptr = c;
        });

        src >> mSound;
    }

    void write(eWriteStream& dst) const override {
        dst.writeCharacter(mCptr);

        dst << mSound;
    }
private:
    stdptr<eCharacter> mCptr;
    eGodSound mSound;
};

class eGoToTargetTeleport : public eFindFailFunc {
public:
    eGoToTargetTeleport(eGameBoard& board) :
        eFindFailFunc(board, eFindFailFuncType::teleport2) {}
    eGoToTargetTeleport(eGameBoard& board, eGodMonsterAction* const ca) :
        eFindFailFunc(board, eFindFailFuncType::teleport2),
        mTptr(ca) {}

    void call(eTile* const tile) {
        if(!mTptr) return;
        mTptr->teleport(tile);
    }

    void read(eReadStream& src) {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eGodMonsterAction*>(ca);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacterAction(mTptr);
    }
private:
    stdptr<eGodMonsterAction> mTptr;
};

#endif // EGODACTION_H
