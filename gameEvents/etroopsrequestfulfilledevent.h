#ifndef ETROOPSREQUESTFULFILLEDEVENT_H
#define ETROOPSREQUESTFULFILLEDEVENT_H

#include "eplayerconquesteventbase.h"

class eTroopsRequestFulfilledEvent : public ePlayerConquestEventBase {
public:
    eTroopsRequestFulfilledEvent(const eGameEventBranch branch,
                                 eGameBoard& board);

    void initialize(const eEnlistedForces& forces,
                    const stdsptr<eWorldCity>& city,
                    const stdsptr<eWorldCity>& rivalCity);

    void trigger() override;
    std::string longName() const override;
private:
    stdsptr<eWorldCity> mRivalCity;
};

#endif // ETROOPSREQUESTFULFILLEDEVENT_H
