#ifndef EAVAILABLEBUILDINGS_H
#define EAVAILABLEBUILDINGS_H

class eReadStream;
class eWriteStream;

enum class eBuildingType;

enum class eAvailable {
    notAvailable,
    available,
    built
};

struct eAvailableBuildings {
    void read(eReadStream& src);
    void write(eWriteStream& dst) const;

    bool available(const eBuildingType type,
                   const int id = -1) const;
    void built(const eBuildingType type,
               const int id = -1);
    void destroyed(const eBuildingType type,
                   const int id = -1);
    void allow(const eBuildingType type,
               const int id = -1);
    void disallow(const eBuildingType type,
                  const int id = -1);
private:
    bool* allowedPtr(const eBuildingType type);
    eAvailable* availablePtr(const eBuildingType type);
public:
    bool fWheatFarm = false;
    bool fCarrotsFarm = false;
    bool fOnionFarm = false;

    bool fVine = false;
    bool fOliveTree = false;
    bool fOrangeTree = false;

    bool fDairy = false;
    bool fCardingShed = false;

    bool fFishery = false;
    bool fUrchinQuay = false;
    bool fHuntingLodge = false;

    bool fMint = false;
    bool fFoundry = false;
    bool fTimberMill = false;
    bool fMasonryShop = false;

    bool fWinery = false;
    bool fOlivePress = false;
    bool fSculptureStudio = false;

    bool fArmory = false;

    eAvailable  fAphroditeSanctuary = eAvailable::notAvailable;
    eAvailable  fApolloSanctuary = eAvailable::notAvailable;
    eAvailable  fAresSanctuary = eAvailable::notAvailable;
    eAvailable  fArtemisSanctuary = eAvailable::notAvailable;
    eAvailable  fAthenaSanctuary = eAvailable::notAvailable;
    eAvailable  fAtlasSanctuary = eAvailable::notAvailable;
    eAvailable  fDemeterSanctuary = eAvailable::notAvailable;
    eAvailable  fDionysusSanctuary = eAvailable::notAvailable;
    eAvailable  fHadesSanctuary = eAvailable::notAvailable;
    eAvailable  fHephaestusSanctuary = eAvailable::notAvailable;
    eAvailable  fHeraSanctuary = eAvailable::notAvailable;
    eAvailable  fHermesSanctuary = eAvailable::notAvailable;
    eAvailable  fPoseidonSanctuary = eAvailable::notAvailable;
    eAvailable  fZeusSanctuary = eAvailable::notAvailable;

    eAvailable  fAchillesHall = eAvailable::notAvailable;
    eAvailable  fAtalantaHall = eAvailable::notAvailable;
    eAvailable  fBellerophonHall = eAvailable::notAvailable;
    eAvailable  fHerculesHall = eAvailable::notAvailable;
    eAvailable  fJasonHall = eAvailable::notAvailable;
    eAvailable  fOdysseusHall = eAvailable::notAvailable;
    eAvailable  fPerseusHall = eAvailable::notAvailable;
    eAvailable  fTheseusHall = eAvailable::notAvailable;

    int fPopulationMonument = 0;
    int fVictoryMonument = 0;
    int fColonyMonument = 0;
    int fAthleteMonument = 0;
    int fConquestMonument = 0;
    int fHappinessMonument = 0;
    int f7Monument = 0;
    int f8Monument = 0;
    int fScholarMonument = 0;
};

#endif // EAVAILABLEBUILDINGS_H