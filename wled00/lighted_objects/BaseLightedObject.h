#pragma once

#include "ILightedObject.h"

#include "Arduino.h"

/*
**-----------------------------------------------------------------------------
** Base class that all lighted objects should inherit from.  This defines default
** behavior for the ILightedObject interface but, more importantly, includes a 
** number of protected functions that can be used by all lighted objects.
**-----------------------------------------------------------------------------
*/ 
class BaseLightedObject : public ILightedObject
{
    public:
        BaseLightedObject();
        virtual ~BaseLightedObject();

    // ILightedObject Interface
    public:
        /// Returns the name of this object type
        virtual std::string getObjectType() const { return "BaseLightedObject"; }
        
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

        // This will pass in the pointer to the Neo Pixel wrapper for the lighted object to interact with
        virtual void setNeoPixelWrapper(NeoPixelWrapper* neoPixelWrapper) { mPixelWrapper = neoPixelWrapper; }      

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState) { }

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const { }

    protected:
        void setPixelColor(uint16_t address, uint32_t color);

        void serializeCommonState(JsonObject& currentState) const;

    private:
        void setPixelColor(uint16_t address, byte red, byte green, byte blue, byte white);

    protected:
        NeoPixelWrapper *mPixelWrapper;

        uint16_t mStartingAddress;
        uint16_t mNumberOfLEDs;
        uint16_t mSelectedEffectId;
};
