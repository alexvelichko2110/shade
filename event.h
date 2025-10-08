#ifndef EVENT_H_
#define EVENT_H_

#include <string>

namespace uiwidget {

class Event
{
public:

    std::string _name;
    int action;
    double x;
    double y;
};

}

#endif //  