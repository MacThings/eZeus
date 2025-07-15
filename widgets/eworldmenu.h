#ifndef EWORLDMENU_H
#define EWORLDMENU_H

#include "elabel.h"

#include "ebutton.h"
#include "engine/eworldcity.h"
#include "pointers/estdselfref.h"

class eWorldGoodsWidget;
class eWorldTributeWidget;

class eWorldBoard;

class eWorldMenu : public eLabel {
public:
    using eLabel::eLabel;

    void initialize(const eAction& openRequest,
                    const eAction& openFulfill,
                    const eAction& openGift,
                    const eAction& openRaid,
                    const eAction& openConquer,
                    const bool showText = true);

    void setCity(const stdsptr<eWorldCity>& c);
    void setWorldBoard(eWorldBoard* const b);
    void setText(const std::string& text);
    void updateLabels() const;
    void updateButtonsEnabled() const;
private:
    eWorldBoard* mBoard = nullptr;

    eLabel* mTextLabel = nullptr;

    eLabel* mRelationshipLabel = nullptr;
    eLabel* mNameLabel = nullptr;
    eLabel* mLeaderLabel = nullptr;

    eLabel* mAttitudeLabel = nullptr;

    eWorldGoodsWidget* mGoodsWidget = nullptr;
    eWorldTributeWidget* mTributeWidget = nullptr;

    eButton* mRequestButton = nullptr;
    eButton* mFulfillButton = nullptr;
    eButton* mGiftButton = nullptr;
    eButton* mRaidButton = nullptr;
    eButton* mConquerButton = nullptr;

    stdsptr<eWorldCity> mCity;

    bool mShowText = true;
};

#endif // EWORLDMENU_H
