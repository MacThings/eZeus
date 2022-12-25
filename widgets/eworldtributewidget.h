#ifndef EWORLDTRIBUTEWIDGET_H
#define EWORLDTRIBUTEWIDGET_H

#include "ewidget.h"

#include "elabel.h"
#include "engine/eworldcity.h"

class eWorldTributeWidget : public eWidget {
public:
    using eWidget::eWidget;

    void initialize();
    void setCity(const stdsptr<eWorldCity>& c);
private:
    eLabel* mTypeIcon = nullptr;
    eLabel* mTextLabel = nullptr;
};

#endif // EWORLDTRIBUTEWIDGET_H