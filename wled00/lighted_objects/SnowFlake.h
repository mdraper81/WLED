#pragma once

#include "BaseLightedObject.h"

/*
**-----------------------------------------------------------------------------
** Implementation of a snow flake that has led lights on it
**-----------------------------------------------------------------------------
*/ 
class SnowFlake : public BaseLightedObject
{
    public:
        SnowFlake();
        virtual ~SnowFlake();

        static const char* LIGHTED_OBJECT_TYPE_NAME;

    // ILightedObject Interface
    public:
        /// Returns the name of this object type
        virtual std::string getObjectType() const { return LIGHTED_OBJECT_TYPE_NAME; }

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual const char* getSupportedEffects() const { return "[\"Solid\"]"; }

        /// This is called to run another 'frame' of the current effect
        virtual uint16_t runEffect();

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState);

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const;
};

// Auto-register this lighted object
namespace LightedObjectRegistration 
{
    LightedObjectFactoryRegistration<SnowFlake> _SnowFlake(SnowFlake::LIGHTED_OBJECT_TYPE_NAME);
}
