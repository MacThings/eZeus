#ifndef EAPHRODITEHELPACTION_H
#define EAPHRODITEHELPACTION_H

#include "characters/actions/egodaction.h"

#include "buildings/ehousebase.h"

enum class eAphroditeHelpStage {
    none, appear, goTo, populate, disappear
};

class eAphroditeHelpAction : public eGodAction {
public:
    eAphroditeHelpAction(eCharacter* const c);

    bool decide() override;

    void read(eReadStream& src) override;
    void write(eWriteStream& dst) const override;

    static bool sHelpNeeded(const eCityId cid,
                            const eGameBoard& board);
private:
    eHouseBase* nearestHouseWithVacancies();

    void goToTarget();
    void populate();

    eAphroditeHelpStage mStage{eAphroditeHelpStage::none};
};

class eAphroditeHelpAct : public eGodAct {
public:
    eAphroditeHelpAct(eGameBoard& board,
                      eHouseBase* const target) :
        eGodAct(board, eGodActType::aphroditeHelp),
        mTarget(target) {}

    eAphroditeHelpAct(eGameBoard& board) :
        eAphroditeHelpAct(board, nullptr) {}

    eMissileTarget find(eTile* const t) {
        (void)t;
        const auto null = static_cast<eTile*>(nullptr);
        return null;
    }

    void act() {
        if(!mTarget) return;
        const int v = mTarget->vacancies();
        mTarget->moveIn(v);
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

#endif // EAPHRODITEHELPACTION_H
