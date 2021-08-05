#ifndef ECONTEXTMENU_H
#define ECONTEXTMENU_H

#include "eframedwidget.h"

#include "etexture.h"
#include "textures/egametextures.h"

class eContextMenu : public eFramedWidget {
public:
    eContextMenu(eMainWindow* const window);

    void exec(const int x, const int y,
              eWidget* const w);

    void addAction(const std::string& text, const eAction& a);
protected:
    void sizeHint2(int& w, int& h);

    void paintEvent(ePainter& p);
    bool mousePressEvent(const eMouseEvent& e);
    bool mouseMoveEvent(const eMouseEvent& e);
private:
    using eWidget::setHeight;
    using eWidget::setWidth;
    using eWidget::resize;

    int yToActionId(const int y) const;
    eAction yToAction(const int y) const;

    int mHoverId = -1;

    std::vector<eTexture> mTextures;
    std::vector<eAction> mActions;
};

#endif // ECONTEXTMENU_H
