#ifndef EMESSAGES_H
#define EMESSAGES_H

#include "emessage.h"

#include <map>

enum class eGodType;
enum class eHeroType;

struct eQuestMessages {
    eEventMessageType fQuest;
    eEventMessageType fFulfilled;
};

struct eGodMessages {
    mutable int fLastMessage = -1;

    eMessageType fWooing0;
    eMessageType fJealousy1;
    eMessageType fJealousy2;
    eMessageType fInvades;
    eEventMessageType fHelps;

    eEventMessageType fMonster;

    eQuestMessages fQuest1;
    eQuestMessages fQuest2;

    eMessageType fSanctuaryComplete;
};

struct eHeroMessages {
    eEventMessageType fArrival;
    eConsequenceMessageType fHallAvailable;
};

class eMessages {
public:
    eMessages() {}

    static eMessages instance;

    static bool load();
    static bool loaded();

    static const std::string& message(const std::string& key);
    static eMessageType favorMessage(const eMessageType& reason);
    static eMessageType dfavorMessage(const eMessageType& reason);
    static eMessageType invasionMessage(const eMessageType& baseMsg,
                                        const std::string& reason,
                                        const int months);

    std::map<std::string, std::string> fMessages;

    eMessageType fPop100;
    eMessageType fPop500;
    eMessageType fPop1000;
    eMessageType fPop2000;
    eMessageType fPop3000;
    eMessageType fPop5000;
    eMessageType fPop10000;
    eMessageType fPop15000;
    eMessageType fPop20000;
    eMessageType fPop25000;

    eMessageType fHappinessMonument;

    eMessageType fFire;
    eMessageType fCollapse;
    eMessageType fPlagueStrikes;

    const eGodMessages* godMessages(const eGodType godType) const;

    eGodMessages fAphrodite;
    eGodMessages fApollo;
    eGodMessages fAres;
    eGodMessages fArtemis;
    eGodMessages fAthena;
    eGodMessages fAtlas;
    eGodMessages fDemeter;
    eGodMessages fDionysus;
    eGodMessages fHades;
    eGodMessages fHephaestus;
    eGodMessages fHera;
    eGodMessages fHermes;
    eGodMessages fPoseidon;
    eGodMessages fZeus;

    const eHeroMessages* heroMessages(const eHeroType heroType) const;

    eHeroMessages fAchilles;
    eHeroMessages fAtalanta;
    eHeroMessages fBellerophon;
    eHeroMessages fHercules;
    eHeroMessages fJason;
    eHeroMessages fOdysseus;
    eHeroMessages fPerseus;
    eHeroMessages fTheseus;

    eGameMessages fHomeGames;
    eGameMessages fIsthmianGames;
    eGameMessages fNemeanGames;
    eGameMessages fPythianGames;
    eGameMessages fOlympianGames;

    eMessageType fInvasion36;
    eMessageType fInvasion24;
    eMessageType fInvasion12;
    eMessageType fInvasion6;
    eMessageType fInvasion1;
    eMessageType fInvasion;
    std::string fInvasionNoReason;
    eMessageType fInvasionBribed;
    eMessageType fInvasionDefeat;
    eMessageType fInvasionVictory;

    eMessageType fTributePaidGranted;
    eMessageType fTributePaidAccepted;
    eMessageType fTributePaidRefused;
    eMessageType fTributePostponed;

    eMessageType fRequestGranted;
    eMessageType fRequestPartialSpace;
    eMessageType fRequestInsufficientSpace;
    eMessageType fRequestLastChance;
    eMessageType fRequestAccepted;
    eMessageType fRequestPostponed;
    eMessageType fRequestRefused;
    eMessageType fRequestForfeited;

    eMessageType fDemandRefused;

    eMessageType fGiftReceivedDrachmas;
    eMessageType fGiftReceivedNeeded;
    eMessageType fGiftReceivedSells;
    eMessageType fGiftReceivedNotNeeded;
    eMessageType fGiftReceivedRefuse;

    // ally
    eMessageType fGeneralRequestInitialS;
    eMessageType fGeneralRequestReminderS;
    eMessageType fGeneralRequestOverdueS;
    eMessageType fGeneralRequestWarningS;
    eMessageType fGeneralRequestComplyS;
    eMessageType fGeneralRequestTooLateS;
    eMessageType fGeneralRequestRefuseS;
    eReason fGeneralRequestEarlyReasonS;
    eReason fGeneralRequestComplyReasonS;
    eReason fGeneralRequestTooLateReasonS;
    eReason fGeneralRequestRefuseReasonS;

    // rival
    eMessageType fGeneralRequestInitialD;
    eMessageType fGeneralRequestReminderD;
    eMessageType fGeneralRequestOverdueD;
    eMessageType fGeneralRequestWarningD;
    eMessageType fGeneralRequestComplyD;
    eMessageType fGeneralRequestTooLateD;
    eMessageType fGeneralRequestRefuseD;
    eReason fGeneralRequestEarlyReasonD;
    eReason fGeneralRequestComplyReasonD;
    eReason fGeneralRequestTooLateReasonD;
    eReason fGeneralRequestRefuseReasonD;

    // subject
    eMessageType fGeneralRequestInitialP;
    eMessageType fGeneralRequestReminderP;
    eMessageType fGeneralRequestOverdueP;
    eMessageType fGeneralRequestWarningP;
    eMessageType fGeneralRequestComplyP;
    eMessageType fGeneralRequestTooLateP;
    eMessageType fGeneralRequestRefuseP;
    eReason fGeneralRequestEarlyReasonP;
    eReason fGeneralRequestComplyReasonP;
    eReason fGeneralRequestTooLateReasonP;
    eReason fGeneralRequestRefuseReasonP;

    // parent
    eMessageType fGeneralRequestInitialR;
    eMessageType fGeneralRequestReminderR;
    eMessageType fGeneralRequestOverdueR;
    eMessageType fGeneralRequestWarningR;
    eMessageType fGeneralRequestComplyR;
    eMessageType fGeneralRequestTooLateR;
    eMessageType fGeneralRequestRefuseR;
    eReason fGeneralRequestEarlyReasonR;
    eReason fGeneralRequestComplyReasonR;
    eReason fGeneralRequestTooLateReasonR;
    eReason fGeneralRequestRefuseReasonR;

    eMessageType fGiftGranted;
    eMessageType fGiftCashGranted;
    eMessageType fGiftPartialSpace;
    eMessageType fGiftInsufficientSpace;
    eMessageType fGiftLastChance;
    eMessageType fGiftForfeited;
    eMessageType fGiftAccepted;
    eMessageType fGiftCashAccepted;
    eMessageType fGiftPostponed;
    eMessageType fGiftRefused;

    eMessageType fIncreasedFavor;
    eMessageType fDecreasedFavor;

    eMessageType fUnemployment;
    eMessageType fEmployees;

    eMessageType fPlague;
private:
    bool loadImpl();
    void load(eGodMessages& god, const std::string& godName);
    void load(eHeroMessages& hero, const std::string& heroName);

    void loadGeneralRequest(eMessageType& initial,
                            eMessageType& reminder,
                            eMessageType& overdue,
                            eMessageType& warning,
                            eMessageType& comply,
                            eMessageType& tooLate,
                            eMessageType& refuse,
                            eReason& earlyReason,
                            eReason& complyReason,
                            eReason& tooLateReason,
                            eReason& refuseReason,
                            const std::string& letter);

    bool mLoaded = false;
};

#endif // EMESSAGES_H
