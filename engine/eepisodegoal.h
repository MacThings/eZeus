#ifndef EEPISODEGOAL_H
#define EEPISODEGOAL_H

#include "fileIO/ereadstream.h"
#include "fileIO/ewritestream.h"

#include "edate.h"

enum class eEpisodeGoalType {
    population,
    treasury,
    sanctuary,
    support,
    quest,
    slay,
    rule,
    housing,
    setAsideGoods,
    surviveUntil,
    completeBefore,
    tradingPartners
};

struct eEpisodeGoal {
    eEpisodeGoalType fType;
    int fEnumInt1 = 0;
    int fEnumInt2 = 0;
    int fRequiredCount = 0;
    int fStatusCount = 0;
    int fPreviewCount = 0;
    stdsptr<eWorldCity> fCity;

    stdsptr<eEpisodeGoal> makeCopy() const;

    void read(eWorldBoard* const board, eReadStream& src);
    void write(eWriteStream& dst) const;

    bool met() const;

    static std::string sText(const eEpisodeGoalType type);
    std::string text(const bool colonyEpisode) const;
    std::string statusText(const eGameBoard* const b) const;

    void update(const eGameBoard* const b);

    eDate date() const; // for surviveUntil and completeBefore
    void initializeDate(const eGameBoard* const b);
};

#endif // EEPISODEGOAL_H
