
#include "node.h"
#include "camera.h"

#include "shader.h"
#include "buffer.h"
#include "texture.h"

#include "font.h"

#include "render.h"

#include "resource_manager.h"

#include "ui.h"

namespace uiwidget {

UI::~UI()
{
}

UI::UI()
{
}

void UI::mouse_move_event_up(Event *event)
{
    std::vector<Widget*>::iterator i = _widgets.begin(), e = _widgets.end();

    for (; i != e; ++i)
    {
        (*i)->on_event(event);
    }
}

void UI::mouse_click_event_up(Event *event)
{
    std::vector<Widget*>::iterator i = _widgets.begin(), e = _widgets.end();

    for (; i != e; ++i)
    {
        (*i)->on_event(event);
    }
}

void UI::draw(Render* render)
{
    std::vector<Widget*>::iterator i = _widgets.begin(), e = _widgets.end();

    for (; i != e; ++i)
    {
        (*i)->draw(render);
    }

    // menu.draw(render);
}

// UI* instance()
// {
//     static UI* _InstanceUI = nullptr;

//     if (_InstanceUI == nullptr)
//     {
//         _InstanceUI = new UI();
//     }

//     return _InstanceUI;
// }


}
