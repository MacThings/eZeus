#ifndef EEPISODESWIDGET_H
#define EEPISODESWIDGET_H

#include "ewidget.h"

class eWidgetColumn;
class eCampaign;
class eParentCityEpisode;
class eEpisodeWidget;

class eEpisodesWidget : public eWidget {
public:
    using eWidget::eWidget;

    using eSetTextAction = std::function<void(int)>;
    void intialize(const std::shared_ptr<eCampaign>& c,
                   const eSetTextAction& sta);

    void deselectText(const int skipId = -1);
private:
    void update();
    void add(eCampaign* const c,
             const std::shared_ptr<eParentCityEpisode>& e,
             const bool last);
    int columnWidth() const { return width()/sNCols; }

    static const int sNCols = 4;

    eSetTextAction mSta;
    std::shared_ptr<eCampaign> mC;
    eWidget* mEpisodesW = nullptr;
    std::vector<eEpisodeWidget*> mWs;
};

#endif // EEPISODESWIDGET_H