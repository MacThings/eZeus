#include "emouseevent.h"

eMouseEvent::eMouseEvent(const int x, const int y,
                         const bool shift, const bool ctrl,
                         const eMouseButton buttons,
                         const eMouseButton button) :
    mX(x), mY(y), mShift(shift), mCtrl(ctrl),
    mButtons(buttons), mButton(button) {

}

eMouseEvent eMouseEvent::translated(const int x, const int y) const {
    auto r = *this;
    r.mX += x;
    r.mY += y;
    return r;
}

eMouseEvent eMouseEvent::withPosition(const int x, const int y) const {
    auto r = *this;
    r.mX = x;
    r.mY = y;
    return r;
}

eMouseWheelEvent::eMouseWheelEvent(const int x, const int y,
                                   const bool shift, const bool ctrl,
                                   const eMouseButton buttons,
                                   const int dy) :
    eMouseEvent(x, y, shift, ctrl, buttons), mDY(dy) {

}

eMouseWheelEvent eMouseWheelEvent::translated(const int x, const int y) const {
    auto r = *this;
    r.mX += x;
    r.mY += y;
    return r;
}

eMouseWheelEvent eMouseWheelEvent::withPosition(const int x, const int y) const {
    auto r = *this;
    r.mX = x;
    r.mY = y;
    return r;
}

eKeyPressEvent::eKeyPressEvent(const int x, const int y,
                               const bool shift, const bool ctrl,
                               const eMouseButton buttons,
                               const SDL_Scancode key) :
    eMouseEvent(x, y, shift, ctrl, buttons), mKey(key) {

}

eKeyPressEvent eKeyPressEvent::translated(const int x, const int y) const {
    auto r = *this;
    r.mX += x;
    r.mY += y;
    return r;
}

eKeyPressEvent eKeyPressEvent::withPosition(const int x, const int y) const {
    auto r = *this;
    r.mX = x;
    r.mY = y;
    return r;
}
