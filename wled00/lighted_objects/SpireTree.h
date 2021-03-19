#pragma once

#include "BaseLightedObject.h"

/*
**-----------------------------------------------------------------------------
** Implementation of a three sided spire christmas tree with leds on it
**-----------------------------------------------------------------------------
*/ 
class SpireTree : public BaseLightedObject
{
    public:
        SpireTree();
        virtual ~SpireTree();

        static const char* LIGHTED_OBJECT_TYPE_NAME;

    // ILightedObject Interface
    public:
        /// Returns the name of this object type
        virtual std::string getObjectType() const { return LIGHTED_OBJECT_TYPE_NAME; }

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual std::list<const char*> getSupportedEffects() const;

        /// This is called to run another 'frame' of the current effect
        virtual uint16_t runEffect();

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
    LightedObjectFactoryRegistration<SpireTree> _SpireTree(SpireTree::LIGHTED_OBJECT_TYPE_NAME);
}
