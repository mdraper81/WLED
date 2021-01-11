#ifndef __LIGHT_DISPLAY_H
#define __LIGHT_DISPLAY_H

#include "Arduino.h"
#include "NpbWrapper.h"

#include <list>
#include <string>

// Forward Declarations
class ILightedObject;

/*
**-----------------------------------------------------------------------------
** Each WLED microcontroller will have one LightDisplay object that contains the
** list of all lighted objects that are included in that display.  This object
** owns all of those lighted objects and can add/remove them and tell each object
** what its starting address is.  This also contains some configuration settings
** for the display as a whole.
**-----------------------------------------------------------------------------
*/ 
class LightDisplay
{
    public:
        typedef std::list<ILightedObject*> LightedObjectList;

        LightDisplay();
        virtual ~LightDisplay();

        void init(bool supportWhite, uint16_t totalPixels);
        void runEffect();

        void createLightedObject(std::string objectType);

        const LightedObjectList getLightedObjects() const { return mLightedObjects; }
        //void deserializeAndApplyStateFromJson(JsonObject newState);
        //void serializeCurrentStateToJson(JsonObject &currentState) const;

    // Accessors / Modfiiers
    public:
        void setBrightness(uint8_t newBrightness);
        uint8_t getBrightness() const { return mCurrentBrightness; }

        void setColorOrder(uint8_t newColorOrder);
        uint8_t getColorOrder();

        void setCurrentPerLED(uint8_t newCurrent) { mMilliampsPerLed = newCurrent; }
        uint8_t getCurrentPerLED() const { return mMilliampsPerLed; }

        uint32_t getLastShowTimestamp() const { return mLastShowTimestamp; }

        void setMaximumAllowedCurrent(uint16_t newCurrent) { mMaxMilliamps = newCurrent; }
        uint16_t getMaximumAllowedCurrent() const { return mMaxMilliamps; }

        uint16_t getNumberOfLEDs() const;

        uint8_t getNumberOfLightedObjects() const { return mLightedObjects.size(); }

        uint32_t getPixelColor(uint16_t address) const;

        void setRgbwMode(uint8_t newMode) { mRgbwMode = newMode; }
        uint8_t getRgbwMode() const { return mRgbwMode; }

        bool getSupportsWhiteChannel() const { return mSupportsWhiteChannel; }

        void enableBrightnessGammaCorrection(bool enabled) { mGammaCorrectBrightness = enabled; }
        bool isBrightnessGammaCorrectionEnabled() const { return mGammaCorrectBrightness; }

        void enableColorGammaCorrection(bool enabled) { mGammaCorrectColor = enabled; }
        bool isColorGammaCorrectionEnabled() const { return mGammaCorrectColor; }

        void enableReverseMode(bool enabled) { mReverseModeEnabled = enabled; }
        bool isReverseModeEnabled() const { return mReverseModeEnabled; }

        bool useWhiteChannel() const; // MDR DEBUG - TODO - this was private

    // Private functions
    private:
        void show();

        uint32_t getGammaCorrectedColor(uint32_t color) const;

        // Power Limiting Utility Functions
        uint32_t calculatePowerBudget(uint8_t milliampsPerLed);
        uint32_t calculatePowerConsumption();
        uint8_t getPowerBudgetAllowedBrightness(uint32_t powerBudget, uint32_t basePowerConsumption);
        bool useWS2815PowerModel() const;

        uint32_t max(uint32_t value1, uint32_t value2) const { return value1 > value2 ? value1 : value2; }
        uint32_t min(uint32_t value1, uint32_t value2) const { return value1 < value2 ? value1 : value2; }

    // Private constants
    private:
        static const int MAX_NUM_LIGHTED_OBJECTS = 12;
        static const int MAX_LIGHTED_OBJECT_DATA = 2048;

        static const int MIN_FRAME_TIME_IN_MS = 15;

        static const int POWER_UNITS_PER_LED = 195075; // each LED can draw up 195075 "power units" (approx. 53mA)
        static const int DEFAULT_MILLIAMP_PER_LED = 55;
        static const int WS2815_POWER_MODEL_MILLIAMP_PER_LED = 12; // from testing an actual strip
        static const int MILLIAMP_PER_MICROCONTROLLER = 100;
        static const int BRIGHTNESS_SCALING_MILLIAMP_THRESHOLD = 149; // Max Allowed Current must be greater than this value

        static const int DEFAULT_BRIGHTNESS_SETTING = 128;
        static const int DEFAULT_MAX_MILLIAMPS = 850;

        static const byte sGammaTable[];

        enum ObjectOptionsE
        {
            NoOptions       = 0x00,
            LightsOn        = 0x01,
            IsSelected      = 0x02,
            AllOptions      = 0x03, // This should be the sum of all previous options
        };

    // Private members
    private:
        uint16_t            mMaxPixelsInDisplay;
        uint8_t             mCurrentBrightness;
        bool                mSupportsWhiteChannel;
        bool                mReverseModeEnabled;
        uint8_t             mRgbwMode;

        bool                mGammaCorrectBrightness;
        bool                mGammaCorrectColor;

        uint16_t            mMaxMilliamps;
        uint8_t             mMilliampsPerLed;
        uint16_t            mCurrentMilliamps;

        NeoPixelWrapper*    mNeoPixelWrapper;

        uint32_t            mCurrentTimestamp;
        uint32_t            mLastShowTimestamp;

        LightedObjectList   mLightedObjects;
};

#endif
