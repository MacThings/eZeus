#ifndef EGODACTION_H
#define EGODACTION_H

#include "ecomplexaction.h"

#include "characters/gods/egod.h"

class eGodAction : public eComplexAction {
public:
    eGodAction(eCharacter* const c,
               const eAction& failAction,
               const eAction& finishAction);

    using eFunc = std::function<void()>;
    void appear(const eFunc& finish = nullptr);
    void disappear(const bool die = false,
                   const eFunc& finish = nullptr);
    void teleport(eTile* const tile);

    void moveAround(const eAction& finishAct = [](){},
                    const int time = 5000);
    void patrol(const eAction& finishAct = [](){},
                const int dist = 100);

    void randomPlaceOnBoard();
    eTile* closestRoad(const int rdx, const int rdy) const;

    eGodType type() const { return mType; }
private:
    void hermesRun(const bool appear);

    void playAppearSound();
    void playDisappearSound();

    const eGodType mType;
};

#endif // EGODACTION_H
