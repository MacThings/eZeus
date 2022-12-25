#ifndef EHEROACTION_H
#define EHEROACTION_H

#include "eactionwithcomeback.h"

#include "characters/monsters/emonster.h"

class eMonster;

enum class eHeroActionStage {
    none, patrol, hunt, fight, goBack
};

class eHeroAction : public eActionWithComeback {
public:
    eHeroAction(eCharacter* const c);

    bool decide() override;
    void increment(const int by) override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;
private:
    void lookForMonster();
    void lookForMonsterFight();
    bool fightMonster(eMonster* const m);
    void huntMonster(eMonster* const m);

    eHeroActionStage mStage = eHeroActionStage::none;
    int mLookForMonster = 0;
};

class eHA_patrolFail : public eCharActFunc {
public:
    eHA_patrolFail(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::HA_patrolFail) {}
    eHA_patrolFail(eGameBoard& board, eHeroAction* const ca) :
        eCharActFunc(board, eCharActFuncType::HA_patrolFail),
        mTptr(ca) {}

    void call() {
        if(!mTptr) return;
        const auto t = mTptr.get();
        const auto c = t->character();
        c->setActionType(eCharacterActionType::walk);
        t->goBack(eWalkableObject::sCreateDefault());
    }

    void read(eReadStream& src) {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eHeroAction*>(ca);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacterAction(mTptr);
    }
private:
    stdptr<eHeroAction> mTptr;
};

class eHA_huntMonsterFail : public eCharActFunc {
public:
    eHA_huntMonsterFail(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::HA_huntMonsterFail) {}
    eHA_huntMonsterFail(eGameBoard& board, eHeroAction* const ca) :
        eCharActFunc(board, eCharActFuncType::HA_huntMonsterFail),
        mTptr(ca) {}

    void call() {
        if(!mTptr) return;
        const auto t = mTptr.get();
        const auto c = t->character();
        c->setActionType(eCharacterActionType::walk);
        t->goBack(eWalkableObject::sCreateDefault());
    }

    void read(eReadStream& src) {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eHeroAction*>(ca);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacterAction(mTptr);
    }
private:
    stdptr<eHeroAction> mTptr;
};

class eHA_huntMonsterFinish : public eCharActFunc {
public:
    eHA_huntMonsterFinish(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::HA_huntMonsterFinish) {}
    eHA_huntMonsterFinish(eGameBoard& board, eHeroAction* const ca,
                          eMonster* const m) :
        eCharActFunc(board, eCharActFuncType::HA_huntMonsterFinish),
        mTptr(ca), mMptr(m) {}

    void call() {
        if(!mTptr) return;
        const auto t = mTptr.get();
        const auto c = t->character();
        c->setActionType(eCharacterActionType::walk);
        t->goBack(eWalkableObject::sCreateDefault());
    }

    void read(eReadStream& src) {
        src.readCharacterAction(&board(), [this](eCharacterAction* const ca) {
            mTptr = static_cast<eHeroAction*>(ca);
        });
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mMptr = static_cast<eMonster*>(c);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacterAction(mTptr);
        dst.writeCharacter(mMptr);
    }
private:
    stdptr<eHeroAction> mTptr;
    stdptr<eMonster> mMptr;
};

class eHA_fightMonsterDie : public eCharActFunc {
public:
    eHA_fightMonsterDie(eGameBoard& board) :
        eCharActFunc(board, eCharActFuncType::HA_fightMonsterDie) {}
    eHA_fightMonsterDie(eGameBoard& board, eMonster* const m) :
        eCharActFunc(board, eCharActFuncType::HA_fightMonsterDie),
        mMptr(m) {}

    void call() {
        if(!mMptr) return;
        mMptr->killWithCorpse();
    }

    void read(eReadStream& src) {
        src.readCharacter(&board(), [this](eCharacter* const c) {
            mMptr = static_cast<eMonster*>(c);
        });
    }

    void write(eWriteStream& dst) const {
        dst.writeCharacter(mMptr);
    }
private:
    stdptr<eMonster> mMptr;
};

#endif // EHEROACTION_H