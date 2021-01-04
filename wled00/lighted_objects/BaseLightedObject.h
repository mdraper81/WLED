#ifndef __BASE_LIGHTED_OBJECT
#define __BASE_LIGHTED_OBJECT

#include "ILightedObject.h"

#include "Arduino.h"
#include "NpbWrapper.h"

/*
**-----------------------------------------------------------------------------
** Base class that all lighted objects should inherit from.  This defines default
** behavior for the ILightedObject interface but, more importantly, includes a 
** number of protected functions that can be used by all lighted objects.
**-----------------------------------------------------------------------------
*/ 
class BaseLightedObject : ILightedObject
{
    public:
        BaseLightedObject(NeoPixelWrapper *neoPixelWrapper);
        virtual ~BaseLightedObject();

    // ILightedObject Interface
    public:
        /// All connected LEDs have a unique address, this is the address of the
        /// first LED in this object
        virtual uint16_t getNumberOfLEDs() const { return mNumberOfLEDs; }

        /// This is the number of LEDs that make up this object
        virtual uint16_t getStartingLEDNumber() const { return mStartingAddress; }

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual const char* getSupportedEffects() const { return "[\"None\"]"; }

        /// This is called to run another 'frame' of the current effect
        virtual uint16_t runEffect() { return 0; }

        /// This will change the current effect for this object
        virtual void setCurrentEffect(uint16_t effectId) { mSelectedEffectId = 0; }

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState) { }

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const { }

    protected:
        void setPixelColor(uint16_t address, uint32_t color);

    private:
        void setPixelColor(uint16_t address, byte red, byte green, byte blue, byte white);

    protected:
        NeoPixelWrapper *mPixelWrapper;

        uint16_t mStartingAddress;
        uint16_t mNumberOfLEDs;
        uint16_t mSelectedEffectId;
};

#endif
