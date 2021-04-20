#pragma once

#include "ILightedObject.h"
#include <map>
#include <string>

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
        
        /// This is the number of LEDs that make up this object
        virtual uint16_t getNumberOfLEDs() const { return mNumberOfLEDs; }

        /// All connected LEDs have a unique address, this is the address of the
        /// first LED in this object
        virtual uint16_t getStartingLEDNumber() const { return mStartingAddress; }
        virtual void setStartingLEDNumber(uint16_t startingAddress) { mStartingAddress = startingAddress; }

        /// This will return a JSON list of all effects supported by this type
        /// of object
        virtual std::list<const char*> getSupportedEffects() const;

        /// This is called to run another 'frame' of the current effect.  Returns true if any pixels in this
        /// object have changed.
        virtual bool runEffect(uint32_t delta) final;

        // This will pass in the pointer to the Neo Pixel wrapper for the lighted object to interact with
        virtual void setNeoPixelWrapper(NeoPixelWrapper* neoPixelWrapper) { mPixelWrapper = neoPixelWrapper; }    

        /// Allows you to toggle the power for this lighted object
        virtual void togglePower() { mPoweredOn = !mPoweredOn; }         

        /// Updates the parameters of this lighted object using the user input values taken from the UI
        virtual void update(const char* userInputValues); 

        /// This will deserialize the given newState object and apply those values 
        /// to this lighted object.  This applies changes from the web
        virtual void deserializeAndApplyStateFromJson(JsonObject newState) final;

        /// This will populate the given currentState JSON object with the current 
        /// state of this lighted object.  This provides the current state to the web
        virtual void serializeCurrentStateToJson(JsonObject& currentState) const final;

    // Constants
    protected:
        static const int MAX_UI_STRING_LENGTH = 64;

        typedef std::map<std::string /* key */, int /* value */> NumericValues;
        typedef std::pair<std::string, int> NumericValueEntry;

        typedef std::map<std::string /* key */, int /* value */> DropDownSelections;
        typedef std::pair<std::string, int> DropDownSelectionEntry;

        enum TextTypeE
        {
            TextTypeSmall,
            TextTypeLarge
        };

    private:
        static std::initializer_list<const char*> SUPPORTED_EFFECTS;

    protected:
        void setPixelColor(uint16_t address, uint32_t color);
        void setPixelColorForRange(uint16_t startingAddress, uint16_t numPixels, uint32_t color);
        void turnOffPixelsInRange(uint16_t startingAddress, uint16_t numPixels);

        void appendCommonUiElements(JsonArray& uiElementsArray) const;
        void appendDropDownElement(JsonArray& uiElementsArray, std::list<const char*> optionsList, int selectedIndex, const char* label, const char* inputKey) const;
        void appendNumericElement(JsonArray& uiElementsArray, const char* name, int minValue, int maxValue, const int currentValue, const char* inputKey) const;
        void appendStringElement(JsonArray& uiElementsArray, TextTypeE textType, const char* format, ...) const;

    // Functions that are intended to be overriden by derived classes
    protected: 
        virtual void deserializeSpecializedData(const JsonObject& stateObject) = 0;
        virtual void serializeSepecializedData(JsonObject& currentState) const = 0;
        virtual void onParametersUpdated() = 0;

        virtual bool runSpecializedEffect() { return false; }

    private:
        void deserializeUiElements(const JsonArray& uiElementsArray);

        void appendTitleElement(JsonArray& uiElementsArray, const char* format, ...) const;

        void setPixelColor(uint16_t address, byte red, byte green, byte blue, byte white);

    protected:
        NeoPixelWrapper *mPixelWrapper;

        uint64_t mTotalTimeRunning;
        uint16_t mStartingAddress;
        uint16_t mNumberOfLEDs;
        bool     mPoweredOn;

        // Parameter storage that can be used by derived classes, these store
        // parameters in a map using a string key that corresponds to the key
        // used by the UI to refer to the parameter.
        NumericValues mNumericValues;
        DropDownSelections mDropDownSelections;

        static const char* EFFECT_KEY;     

        static const char* SELECTED_EFFECT_ELEMENT;
        static const char* BRIGHTNESS_PCT_ELEMENT;
        static const char* POWERED_ON_ELEMENT;
        static const char* UI_ELEMENTS_ARRAY_ELEMENT;
};
