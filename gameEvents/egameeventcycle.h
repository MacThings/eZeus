#ifndef EGAMEEVENTCYCLE_H
#define EGAMEEVENTCYCLE_H

#include "pointers/eobject.h"

#include "egameevent.h"
#include "engine/edate.h"

class eGameEventCycle : public eObject {
public:
    eGameEventCycle(const stdsptr<eGameEvent>& event,
                    const eDate& startDate,
                    const int cycleDays,
                    const int nRuns,
                    eGameBoard& board);
    eGameEventCycle(const stdsptr<eGameEvent>& event,
                    const eDate& date,
                    eGameBoard& board);
    eGameEventCycle(eGameBoard& board);

    std::string longName() const;

    const eDate& startDate() const { return mStartDate; }
    void setStartDate(const eDate& d);

    int period() const { return mPeriodDays; }
    void setPeriod(const int p);

    int repeat() const { return mTotNRuns; }
    void setRepeat(const int r);

    const stdsptr<eGameEvent>& event() const { return mEvent; }

    void handleNewDate(const eDate& date);
    void rewind(const eDate& date);
    void trigger();
    bool finished() const { return mRemNRuns <= 0; }

    void write(eWriteStream& dst) const;
    void read(eReadStream& src);
private:
    stdsptr<eGameEvent> mEvent;

    eDate mStartDate;
    int mPeriodDays;
    int mTotNRuns;

    int mRemNRuns;
    eDate mNextDate;
};

#endif // EGAMEEVENTCYCLE_H
