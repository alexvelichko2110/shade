
#include "ui.h"

#include "popup_menu.h"

namespace uiwidget {

void PopupMenu::on_event(Event* event)
{
    if (event->_name == "mouse_button_right_down")
    {
        std::cout << "click right down" << event->x << " " << event->y << std::endl;

        state = 1;
    }
    else
    if (event->_name == "mouse_button_right_up")
    {
        std::cout << "click right up" << event->x << " " << event->y << std::endl;

        state = 0;
    }
    else
    if(event->_name == "mouse_move")
    {
        //std::cout << "move " << event->x << " " << event->y << std::endl;

        if (state)
        {
            _x = event->x;
            _y = event->y;
        }

    }
    
}

void PopupMenu::draw(Render* render)
{
    Widget::draw(render);

    // render->set_color(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    // render->draw_rect(_x + _width*0.5, _y - _height*0.5, _width, _height);


    // render->draw_line(_x, _y, _x + _width, _y);    
    // render->draw_line(_x, _y - _height, _x + _width, _y - _height);
    // render->draw_line(_x, _y, _x, _y -_height);
    // render->draw_line(_x + _width, _y, _x + _width, _y - _height);

    // Render* render = instance()->get_render();

    float hline = 30.0;
    float yline = _y; 

    lines_type::iterator i = _lines.begin(), e = _lines.end();

    Font* font = rm()->fonts("font2").get();
    render->set_color(glm::vec4(170.9/255, 242.2/255, 0.0, 1.0));
    render->set_font(font);

    int item = 0;
    int select_item = 1;

    for (; i != e; ++i)
    {
        std::string str = *i;

        float len = compute_line_length(str, font, 1.0);

        if (select_item == item) {
            render->set_color(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
            render->draw_rect(_x + _width*0.5, yline - hline*0.5 - 5, _width - 2, hline);
        }

        render->set_color(glm::vec4(170.9/255, 242.2/255, 0.0, 1.0));
        render->draw_text(str, _x + _width*0.5 - len*0.5, yline - hline);

        yline -= hline;

        item++;
    }

}

}