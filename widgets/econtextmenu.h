#ifndef ECONTEXTMENU_H
#define ECONTEXTMENU_H

#include "eactionlistwidget.h"

#include "etexture.h"
#include "textures/egametextures.h"

class eContextMenu : public eActionListWidget {
public:
    eContextMenu(eMainWindow* const window);

    void exec(const int x, const int y,
              eWidget* const w);

protected:
    bool mouseReleaseEvent(const eMouseEvent& e);
private:
    using eWidget::setHeight;
    using eWidget::setWidth;
    using eWidget::resize;
};

#endif // ECONTEXTMENU_H
