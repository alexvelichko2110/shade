
#ifndef POPUP_MENU_H_
#define POPUP_MENU_H_

#include <string>
#include <vector>

#include "render.h"
#include "widget.h"

namespace uiwidget {

class PopupMenu : public Widget
{
public:

    virtual void draw(Render* render);
    virtual void on_event(Event *event);

    void add_line(const std::string& line)
    {
        _lines.push_back(line);
    }

private:

    typedef std::vector<std::string> lines_type;

    lines_type _lines;


    int state = 0;
};

}

#endif //
