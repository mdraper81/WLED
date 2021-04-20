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
        virtual std::list<const char*> getSupportedEffects() const;

    // BaseLightedObject overrides
    protected:
        virtual void deserializeSpecializedData(const JsonObject& stateObject) {}
        virtual void serializeSepecializedData(JsonObject& currentState) const;        
        virtual void onParametersUpdated();

        // Handles the specialized effect logic for snowflakes
        virtual bool runSpecializedEffect();        

    private:
        static std::initializer_list<const char*> SUPPORTED_EFFECTS;

        void updateTotalNumberOfLeds();

        static const char* ARM_LEN_KEY;
        static const char* LG_CHEVRON_LEN_KEY;
        static const char* SM_CHEVRON_LEN_KEY;
        static const char* NUM_ARM_KEY;
};

// Auto-register this lighted object
namespace LightedObjectRegistration 
{
    LightedObjectFactoryRegistration<SnowFlake> _SnowFlake(SnowFlake::LIGHTED_OBJECT_TYPE_NAME);
}
