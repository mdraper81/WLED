#pragma once

#include "LightedObjectFactory.h"

#include <string>

namespace LightedObjectRegistration
{
    template <typename T>
    class LightedObjectFactoryRegistration
    {
        public:
            LightedObjectFactoryRegistration(std::string objectType)
            {
                LightedObjectFactory::get().registerLightedObjectType(objectType, []() { return static_cast<ILightedObject*>(new T()); });
            }
    };
}