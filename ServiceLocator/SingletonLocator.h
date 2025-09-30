#pragma once
#include "../Singleton/Singleton.h"
#include "Locator.h"

namespace stn
{
    template<class T>
    class SingletonLocator : public Singleton<Locator<T>>
    {
        friend class Singleton<Locator<T>>;
    };
}
#define STN_GET_LOCATOR(c) stn::SingletonLocator<c>::getInstance()
