#ifndef ETRADEPOSTINFOWIDGET_H
#define ETRADEPOSTINFOWIDGET_H

#include "eemployingbuildinginfowidget.h"

#include "../eswitchbutton.h"
#include "../espinbox.h"

#include "engine/eresourcetype.h"

#include "buildings/etradepost.h"

class eTradePostInfoWidget : public eEmployingBuildingInfoWidget {
public:
    eTradePostInfoWidget(eMainWindow* const window,
                         eMainWidget* const mw);

    void initialize(eTradePost* const stor);
    void get(eResourceType& imports,
             eResourceType& exports,
             std::map<eResourceType, int>& count) const;
private:
    std::map<eResourceType, eSwitchButton*> mImportButtons;
    std::map<eResourceType, eSwitchButton*> mExportButtons;
    std::map<eResourceType, eSpinBox*> mSpinBoxes;
};

#endif // ETRADEPOSTINFOWIDGET_H
