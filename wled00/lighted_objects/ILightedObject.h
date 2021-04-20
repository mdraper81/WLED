#pragma once

#include "LightedObjectFactoryRegistration.h"

#include "NpbWrapper.h"

#include "wled.h"

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
        /// Returns the name of this object type
        virtual std::string getObjectType() const = 0;

        /// This is the number of LEDs that make up this object
        virtual uint16_t getNumberOfLEDs() const = 0;

        /// All connected LEDs have a unique address, this is the address of the
        /// first LED in this object
        virtual uint16_t getStartingLEDNumber() const = 0;
        virtual void setStartingLEDNumber(uint16_t startingAddress) = 0;

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual std::list<const char*> getSupportedEffects() const = 0;

        /// This is called to run another 'frame' of the current effect
        virtual bool runEffect(uint32_t delta) = 0;

        // This will pass in the pointer to the Neo Pixel wrapper for the lighted object to interact with
        virtual void setNeoPixelWrapper(NeoPixelWrapper* neoPixelWrapper) = 0;

        /// Allows you to toggle the power for this lighted object
        virtual void togglePower() = 0;      

        /// Updates the parameters of this lighted object using the user input values taken from the UI
        virtual void update(const char* userInputValues) = 0;

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState) = 0;

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const = 0;

        /// This one JSON element tag is defined in the interface so that we can read it in Light Display
        /// when trying to decide which type of ILightedObject to create in order to deserialize the 
        /// array of lighted objects
        static const char* TYPE_ELEMENT;            
};
