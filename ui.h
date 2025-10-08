#ifndef UI_H_
#define UI_H_

#include "event.h"
#include "render.h"

#include "popup_menu.h"
#include "widget.h"

namespace uiwidget {

class UI
{
public:
    
    virtual ~UI ();
    UI ();

    // void set_window(int width, int height)
    // {
    //     _window_width = width;
    //     _window_height = height;

    // }

    void mouse_move_event_up(Event *event);
    void mouse_click_event_up(Event *event);

    void draw(Render* render);

    void add_widget(Widget *widget) {
        _widgets.push_back(widget);
    }
    
private:

    int _window_width;
    int _window_height;

    std::vector<Widget*> _widgets;
};

// UI* instance();

}

#endif // UI_H_ 
