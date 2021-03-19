#include "SnowFlake.h"

const char* SnowFlake::LIGHTED_OBJECT_TYPE_NAME = "Snow Flake";
std::initializer_list<const char*> SnowFlake::SUPPORTED_EFFECTS = {"Solid", "Chase", "Twinkle"};

const char* SnowFlake::ARM_LEN_KEY = "armLength";
const char* SnowFlake::LG_CHEVRON_LEN_KEY = "largeChevronLength";
const char* SnowFlake::SM_CHEVRON_LEN_KEY = "smallChevronLength";
const char* SnowFlake::NUM_ARM_KEY = "numArms";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
SnowFlake::SnowFlake()
    : BaseLightedObject()
{
    mNumericValues[ARM_LEN_KEY] = 7;
    mNumericValues[NUM_ARM_KEY] = 8;
    mNumericValues[LG_CHEVRON_LEN_KEY] = 6;
    mNumericValues[SM_CHEVRON_LEN_KEY] = 4;

    updateTotalNumberOfLeds();
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
SnowFlake::~SnowFlake()
{
}

/*
** ============================================================================
** Returns a list of supported effects for this lighted object
** ============================================================================
*/
std::list<const char*> SnowFlake::getSupportedEffects() const
{
    std::list<const char*> supportedEffects(SUPPORTED_EFFECTS);
    return supportedEffects;
}

/*
** ============================================================================
** Run the currently selected effect
** ============================================================================
*/
uint16_t SnowFlake::runEffect()
{
    for (int address = mStartingAddress; address < mStartingAddress + mNumberOfLEDs; ++address)
    {
        setPixelColor(address, 0x00FF0085);
    }
}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void SnowFlake::serializeSepecializedData(JsonObject& currentState) const
{
    JsonArray uiElementsArray = currentState[UI_ELEMENTS_ARRAY_ELEMENT];
    appendNumericElement(uiElementsArray, "Arm length (LEDs)", 0, 1000, mNumericValues.at(ARM_LEN_KEY), ARM_LEN_KEY);
    appendNumericElement(uiElementsArray, "Large chevron length (LEDs)", 0, 1000, mNumericValues.at(LG_CHEVRON_LEN_KEY), LG_CHEVRON_LEN_KEY);
    appendNumericElement(uiElementsArray, "Small chevron length (LEDs)", 0, 1000, mNumericValues.at(SM_CHEVRON_LEN_KEY), SM_CHEVRON_LEN_KEY);
    appendNumericElement(uiElementsArray, "Number of arms", 0, 1000, mNumericValues.at(NUM_ARM_KEY), NUM_ARM_KEY);
}

void SnowFlake::onParametersUpdated()
{
    updateTotalNumberOfLeds();
}

/*
** ============================================================================
** Update the total number of LEDs for this object (call this any time the 
** parameters for number of arms, number of LEDs per arm, ot the number of LEDs
** in the large or small chevron change).
** ============================================================================
*/
void SnowFlake::updateTotalNumberOfLeds()
{
    int totalLedsPerArm = mNumericValues[ARM_LEN_KEY] + mNumericValues[LG_CHEVRON_LEN_KEY] + mNumericValues[SM_CHEVRON_LEN_KEY];
    mNumberOfLEDs = totalLedsPerArm * mNumericValues[NUM_ARM_KEY];
}