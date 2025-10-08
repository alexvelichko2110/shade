#ifndef WIDGET_H_
#define WIDGET_H_

#include <list>

#include "render.h"

#include "event.h"

namespace uiwidget {


class Widget
{
public:

    virtual ~Widget()
    {

    }

    Widget()
    {
        
    }

    // typedef std::list<Widget*> WidgetList;
    
    virtual void on_event(Event* e)
    {

    }

    virtual void draw(Render* render);

    void set_rect(float x, float y, float width, float height)
    {
        _x = x;
        _y = y;

        _width = width;
        _height = height;
    }

// private:

	// perent node
	// Widget *_parent;

	// list of children node
	// WidgetList _children;

    float _x;
    float _y;

    float _width;
    float _height;
};


}

#endif // WIDGET_H_ 