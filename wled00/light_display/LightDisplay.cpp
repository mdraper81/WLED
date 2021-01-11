#include "LightDisplay.h"

#include "lighted_objects/LightedObjectFactory.h"

#include "const.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#define USE_GET_MILLISECOND_TIMER
#include "FastLED.h"

//gamma 2.8 lookup table used for color correction
const byte LightDisplay::sGammaTable[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/*
** ============================================================================
** Constructor
** ============================================================================
*/
LightDisplay::LightDisplay()
    : mMaxPixelsInDisplay( 0 )
    , mCurrentBrightness( DEFAULT_BRIGHTNESS_SETTING )
    , mSupportsWhiteChannel( false )
    , mReverseModeEnabled( false )
    , mRgbwMode( RGBW_MODE_DUAL )
    , mGammaCorrectBrightness( false )
    , mGammaCorrectColor( true )
    , mMaxMilliamps( DEFAULT_MAX_MILLIAMPS )
    , mMilliampsPerLed( DEFAULT_MILLIAMP_PER_LED )
    , mCurrentMilliamps( 0 )
    , mNeoPixelWrapper( nullptr )
    , mCurrentTimestamp( 0 )
    , mLastShowTimestamp( 0 )
{
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
LightDisplay::~LightDisplay()
{
    if (mNeoPixelWrapper != nullptr)
    {
        delete mNeoPixelWrapper;
        mNeoPixelWrapper = nullptr;
    }
}

/*
** ============================================================================
** Initializes the light display
** ============================================================================
*/
void LightDisplay::init(bool supportsWhite, uint16_t totalPixels)
{
    Serial.printf("MDR DEBUG: Calling LightDisplay::init()");
    mNeoPixelWrapper = new NeoPixelWrapper();
    mLastShowTimestamp = mCurrentTimestamp = 0;
    mMaxPixelsInDisplay = totalPixels;
    mSupportsWhiteChannel = supportsWhite;
}

/*
** ============================================================================
** Sets up and displays the next 'frame' for each lighted object
** ============================================================================
*/
void LightDisplay::runEffect()
{
    uint32_t mCurrentTimestamp = millis(); // Be aware, millis() rolls over every 49 days
    
    // Early exit if it is too soon to setup the next frame
    if (mCurrentTimestamp - mLastShowTimestamp < MIN_FRAME_TIME_IN_MS)
    {
        return;
    }

    bool isShowRequired = false;
    // MDR DEBUG - TODO - go through all lighted objects and setup the next frame
    // if one or more are active then set isShowRequired to true.

    if (isShowRequired)
    {
        yield();
        show();
    }
}

/*
** ============================================================================
** Creates a new lighted object and adds it to the list of lighted objects for
** this display.
**
**  param   objectType - string specifying which type of lighted object to add.
** ============================================================================
*/
void LightDisplay::createLightedObject(std::string objectType)
{
    ILightedObject* newObject = LightedObjectFactory::get().createLightedObject(objectType, mNeoPixelWrapper);
    mLightedObjects.push_back(newObject);
    Serial.printf("MDR DEBUG: There are now %d objects in our list of lighted objects\n", mLightedObjects.size());
}

#if 0
/*
** ============================================================================
** Parse the given JsonObject and update the state of this object to reflect the
** settings in JSON.  This is used to apply changes to the light display from the
** web interface.
** ============================================================================
*/
void LightDisplay::deserializeAndApplyStateFromJson(JsonObject newState)
{
    JsonVariant displayObject = newState["display"];
    if (displayObject.is<JsonObject>())
    {
        // MDR DEBUG - TODO - Should we be setting the values here?  It seems like we might
        // get Json Objects from the cfg.json file OR from the web server.

        // Deserialize all lighted objects
        JsonArray lightedObjectsArray = displayObject["lighted_objects"].as<JsonArray>();
        for (JsonObject lightedObject : lightedObjectsArray)
        {
            // MDR DEBUG - TODO look up the correct object in the array of lighted objects and
            // call deserialize on it
        }
    }
}

/*
** ============================================================================
** Populates the given JsonObject with the current state of this light display.
** Used to populate the web UI with the current values for the display.
** ============================================================================
*/
void LightDisplay::serializeCurrentStateToJson(JsonObject &currentState) const
{
    currentState[F("led_count")] = getNumberOfLEDs();
    currentState[F("rgbw_mode")] = getSupportsWhiteChannel();
    currentState[F("use_white_channel")] = useWhiteChannel();

    JsonObject powerStats = currentState.createNestedObject("power_stats");
    powerStats[F("current")] = mCurrentMilliamps;
    powerStats[F("max_current")] = mMaxMilliamps;

    JsonArray ledPinsArray = currentState.createNestedArray("led_pins");
    ledPinsArray.add(LEDPIN);

    JsonArray lightedObjectsArray = currentState.createNestedArray("lighted_objects");
    for (ILightedObject* lightedObject : mLightedObjects)
    {
        JsonObject currentLightedObject = lightedObjectsArray.createNestedObject();
        lightedObject->serializeCurrentStateToJson(currentLightedObject);
    }
}
#endif

/*
** ============================================================================
** Sets the new desired brightness value (applies gamma correction if necessary)
** ============================================================================
*/
void LightDisplay::setBrightness(uint8_t newBrightness)
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return;
    }

    static bool sShouldStartBus = false;

    uint8_t correctBrightness = newBrightness;
    if (mGammaCorrectBrightness)
    {
        correctBrightness = sGammaTable[newBrightness];
    }

    // If the brightness needs to be updated
    if (mCurrentBrightness != correctBrightness)
    {
        mCurrentBrightness = correctBrightness;

        if (mCurrentBrightness == 0)
        {
            #if LEDPIN == LED_BUILTIN
                sShouldStartBus = true;
            #endif
        }
        else
        {
            #if LEDPIN == LED_BUILTIN
                if (sShouldStartBus)
                {
                    sShouldStartBus = false;

                    const NeoPixelType pixelType = mSupportsWhiteChannel ? NeoPixelType_Grbw : NeoPixelType_Grb;
                    mNeoPixelWrapper->Begin(pixelType, mMaxPixelsInDisplay);
                }
            #endif
        }
        
    }

    // Apply the brightness change immediately if we have not just updated the LEDs
    uint32_t currentTime = millis();
    if (currentTime - mLastShowTimestamp > MIN_FRAME_TIME_IN_MS)
    {
        show();
    }
}

/*
** ============================================================================
** Sets the RGB color order for the NeoPixelWrapper
** ============================================================================
*/
void LightDisplay::setColorOrder(uint8_t newColorOrder)
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return;
    }

    mNeoPixelWrapper->SetColorOrder(newColorOrder);
}

/*
** ============================================================================
** Returns the RGB color order for the NeoPixelWrapper
** ============================================================================
*/
uint8_t LightDisplay::getColorOrder()
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return 0;
    }

    return mNeoPixelWrapper->GetColorOrder();
}

/*
** ============================================================================
** Gets the color being displayed at a specific address
** ============================================================================
*/
uint32_t LightDisplay::getPixelColor(uint16_t address) const
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return 0;
    }

    if (address > getNumberOfLEDs())
    {
        return 0;
    }

    return mNeoPixelWrapper->GetPixelColorRgbw(address);
}

/*
** ============================================================================
** Shows the next 'frame' of the effect for each lighted object.  This is called
** to actually change the lights to the new values.
**
** DISCLAIMER
** The following function attemps to calculate the current LED power usage,
** and will limit the brightness to stay below a set amperage threshold.
** It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
** Stay safe with high amperage and have a reasonable safety margin!
** I am NOT to be held liable for burned down garages!
** ============================================================================
*/
void LightDisplay::show()
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return;
    }

    //power limit calculation
    byte actualMilliampsPerLed = useWS2815PowerModel() ? WS2815_POWER_MODEL_MILLIAMP_PER_LED : mMilliampsPerLed;

    // 0 mA per LED or too low numbers turn off calculation
    if (mMaxMilliamps > BRIGHTNESS_SCALING_MILLIAMP_THRESHOLD && actualMilliampsPerLed > 0)
    {
        uint32_t puPerMilliamp = POWER_UNITS_PER_LED / actualMilliampsPerLed;
        uint32_t powerBudget = calculatePowerBudget(puPerMilliamp);
        uint32_t basePowerConsumption = calculatePowerConsumption();
    
        // This will either be the current brightness selected, or a downscaled
        // brightness that remains within the power budget
        uint8_t brightness = getPowerBudgetAllowedBrightness(powerBudget, basePowerConsumption);

        // Set the new brightness and calculate the current milliamps being used
        mNeoPixelWrapper->SetBrightness(brightness);

        mCurrentMilliamps = (basePowerConsumption * brightness) / puPerMilliamp;
        mCurrentMilliamps += MILLIAMP_PER_MICROCONTROLLER; // add power of ESP back to estimate
        mCurrentMilliamps += mMaxPixelsInDisplay; // add standby power back to estimate
    }
    else
    {
        mCurrentMilliamps = 0;
        mNeoPixelWrapper->SetBrightness(mCurrentBrightness);
    }
  
    // some buses send asynchronously and this method will return before
    // all of the data has been sent.
    // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
    mNeoPixelWrapper->Show();
    mLastShowTimestamp = mCurrentTimestamp;
}

/*
** ============================================================================
** Returns the gamma correct value of the given color (if gamma correction for
** color is enabled).
** ============================================================================
*/
uint32_t LightDisplay::getGammaCorrectedColor(uint32_t color) const
{
    uint32_t correctColor = color;

    if (mGammaCorrectColor)
    {
        uint8_t white = (color >> 24);
        uint8_t red =   (color >> 16);
        uint8_t green = (color >> 8);
        uint8_t blue =   color;

        white = sGammaTable[white];
        red = sGammaTable[red];
        green = sGammaTable[green];
        blue = sGammaTable[blue];

        correctColor = ((white << 24) | (red << 16) | (green << 8) | (blue));
    }

    return correctColor;
}

/*
** ============================================================================
** Calculates the power budget in power units (PU) that can be used to power all 
** of the LEDs and remain within the maximum allowed current (mMaxMilliamps). One
** PU is the power it takes to have 1 channel 1 step brighter per brightness step
** so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
**
**  param   milliampsPerLed - power consumption for an individual LED
**
**  returns maximum allowed Power Units to stay within current limitation
** ============================================================================
*/
uint32_t LightDisplay::calculatePowerBudget(uint8_t puPerMilliamp)
{
    uint32_t powerBudget = (mMaxMilliamps - MILLIAMP_PER_MICROCONTROLLER) * puPerMilliamp;
    if (powerBudget > puPerMilliamp * mMaxPixelsInDisplay)
    {
        // each LED uses about 1mA in standby, exclude that from power budget
        powerBudget -= puPerMilliamp * mMaxPixelsInDisplay;
    }
    else
    {
        // There is not enough current allowed in mMaxMilliamps to run all of the LEDs in
        // standby, set the power budget to 0
        powerBudget = 0;
    }

    return powerBudget;
}

/*
** ============================================================================
** Calculates the power consumption of the current display.  This will sum together
** the current being used by each pixel and return the total current usage in 
** milliamps.  This is a calculation that looks at the power for the colors being
** set on each pixel.  The brightness can be adjusted to affect the power usage.
**
**  returns base power consumption to show the current colors of every LED (in mA)
** ============================================================================
*/
uint32_t LightDisplay::calculatePowerConsumption()
{
    // Early exit if neo pixel wrapper is not yet setup
    if (nullptr == mNeoPixelWrapper)
    {
        return 0;
    }

    //sum up the usage of each LED
    uint32_t powerSum = 0;
    for (uint16_t i = 0; i < mMaxPixelsInDisplay; i++) 
    {
        RgbwColor color = mNeoPixelWrapper->GetPixelColorRaw(i);
        if(useWS2815PowerModel())
        {
            // ignore white component on WS2815 power calculation
            powerSum += (max(max(color.R,color.G),color.B)) * 3;
        }
        else 
        {
            powerSum += (color.R + color.G + color.B + color.W);
        }
    }

    // RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
    if (mSupportsWhiteChannel) 
    {
        powerSum *= 3;
        powerSum = powerSum >> 2; //same as /= 4
    }

    return powerSum;
}

/*
** ============================================================================
** Returns the brightness to apply to the LEDs to remain within the power budget.
** This will use the current brightness setting unless that would use too much
** power.
**
**  param   powerBudget - maximum power units allowed to stay within current limitation
**  param   basePowerConsumption - current required to display all LEDs with the current
**          pixel colors
**
**  returns brightness to set all LEDs to that stays within the power budget
** ============================================================================
*/
uint8_t LightDisplay::getPowerBudgetAllowedBrightness(uint32_t powerBudget, uint32_t basePowerConsumption)
{
    uint8_t allowedBrightness = mCurrentBrightness;
    uint32_t powerConsumption = basePowerConsumption * mCurrentBrightness;
    
    // scale brightness down to stay in current limit (if necessary)
    if (powerConsumption > powerBudget)
    {
        float percentToScale = (float)powerBudget / (float)basePowerConsumption;
        uint32_t integerToScale = percentToScale * 255;
        uint8_t byteToScale = (integerToScale > 255) ? 255 : integerToScale;
        allowedBrightness = scale8(mCurrentBrightness, byteToScale);
    }

    return allowedBrightness;
}

/*
** ============================================================================
** Returns true if we should be using the WS2815 power model
** ============================================================================
*/
bool LightDisplay::useWS2815PowerModel() const
{
    return mMilliampsPerLed == 255;
}

/*
** ============================================================================
** Returns true if the current display and selected RGBW mode support a white
** channel
** ============================================================================
*/
bool LightDisplay::useWhiteChannel() const
{
    bool rgbwModeUsesWhiteChannel = (RGBW_MODE_MANUAL_ONLY == mRgbwMode || RGBW_MODE_DUAL == mRgbwMode);
    return mSupportsWhiteChannel && rgbwModeUsesWhiteChannel;
}

/*
** ============================================================================
** Returns the number of LEDs in the current light strand
** ============================================================================
*/
uint16_t LightDisplay::getNumberOfLEDs() const
{
    // MDR DEBUG - TODO Loop through each lighted object and call getNumberOfLEDs
    return mMaxPixelsInDisplay;
}