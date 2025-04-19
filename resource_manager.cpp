
#include "resource_manager.h"

ResourceManager* rm()
{
    static ResourceManager* _InstanceRM = nullptr;

    if (_InstanceRM == nullptr)
    {
        _InstanceRM = new ResourceManager();
    }

    return _InstanceRM;
}
