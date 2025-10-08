#include "ui.h"

#include "widget.h"

namespace uiwidget {

void Widget::draw(Render *render)
{
    // rect fill color
    render->set_color(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    render->draw_rect(_x + _width*0.5, _y - _height*0.5, _width, _height);

    // frame
    render->set_color(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    render->draw_line(_x, _y, _x + _width, _y);    
    render->draw_line(_x, _y - _height, _x + _width, _y - _height);
    render->draw_line(_x, _y, _x, _y -_height);
    render->draw_line(_x + _width, _y, _x + _width, _y - _height);
}

}