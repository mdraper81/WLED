#ifndef __LIGHT_STRAND_H
#define __LIGHT_STRAND_H

#include "BaseLightedObject.h"

class LightStrand : BaseLightedObject
{
    public:
        LightStrand(NeoPixelWrapper *neoPixelWrapper);
        virtual ~LightStrand();

    // ILightedObject Interface
    public:
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

#endif
