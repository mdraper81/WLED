#pragma once

#include "BaseLightedObject.h"

/*
**-----------------------------------------------------------------------------
** Implementation of a string of led lights
**-----------------------------------------------------------------------------
*/ 
class LightStrand : public BaseLightedObject
{
    public:
        LightStrand();
        virtual ~LightStrand();

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

        // Handles the specialized effect logic for light strands
        virtual bool runSpecializedEffect();

    private:
        static std::initializer_list<const char*> SUPPORTED_EFFECTS;    

        static const char* STRAND_LENGTH_KEY;
};



// Auto-register this lighted object
namespace LightedObjectRegistration 
{
    LightedObjectFactoryRegistration<LightStrand> _LightStrand(LightStrand::LIGHTED_OBJECT_TYPE_NAME);
}
