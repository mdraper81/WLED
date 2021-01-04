#ifndef __I_LIGHTED_OBJECT_H
#define __I_LIGHTED_OBJECT_H

#include "src/dependencies/json/ArduinoJson-v6.h"

#include <stdint.h>

/*
**-----------------------------------------------------------------------------
** Interface that all lighted objects inherit from so that we can easily add
** new lighted objects in the future and the calling code does not need to 
** concern itself with the details of the lighted object.
**-----------------------------------------------------------------------------
*/ 
class ILightedObject
{
    public:
        /// All connected LEDs have a unique address, this is the address of the
        /// first LED in this object
        virtual uint16_t getNumberOfLEDs() const = 0;

        /// This is the number of LEDs that make up this object
        virtual uint16_t getStartingLEDNumber() const = 0;

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual const char* getSupportedEffects() const = 0;

        /// This is called to run another 'frame' of the current effect
        virtual uint16_t runEffect() = 0;

        /// This will change the current effect for this object
        virtual void setCurrentEffect(uint16_t effectId) = 0;

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState) = 0;

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const = 0;
};

#endif
