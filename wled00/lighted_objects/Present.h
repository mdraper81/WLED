#pragma once

#include "BaseLightedObject.h"

/*
**-----------------------------------------------------------------------------
** Implementation of a wrapped christmas present that has led lights on it
**-----------------------------------------------------------------------------
*/ 
class Present : public BaseLightedObject
{
    public:
        Present();
        virtual ~Present();

        static const char* LIGHTED_OBJECT_TYPE_NAME;

    // ILightedObject Interface
    public:
        /// Returns the name of this object type
        virtual std::string getObjectType() const { return LIGHTED_OBJECT_TYPE_NAME; }

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual std::list<const char*> getSupportedEffects() const;

        /// This is called to run another 'frame' of the current effect
        virtual bool runEffect(uint32_t delta);

    // BaseLightedObject overrides
    protected:
        virtual void deserializeSpecializedData(const JsonObject& stateObject) {}
        virtual void serializeSepecializedData(JsonObject& currentState) const {}
        virtual void onParametersUpdated() {}
        
    private:
        static std::initializer_list<const char*> SUPPORTED_EFFECTS;
};

// Auto-register this lighted object
namespace LightedObjectRegistration 
{
    LightedObjectFactoryRegistration<Present> _Present(Present::LIGHTED_OBJECT_TYPE_NAME);
}
