#include "LightStrand.h"

const char* LightStrand::LIGHTED_OBJECT_TYPE_NAME = "Strand";
std::initializer_list<const char*> LightStrand::SUPPORTED_EFFECTS = {"Solid", "Multi-Color Solid", "Chase"};

const char* LightStrand::STRAND_LENGTH_KEY = "strandLength";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
LightStrand::LightStrand()
    : BaseLightedObject()
{
    mNumericValues[STRAND_LENGTH_KEY] = 50;
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
LightStrand::~LightStrand()
{
}

/*
** ============================================================================
** Returns a list of supported effects for this lighted object
** ============================================================================
*/
std::list<const char*> LightStrand::getSupportedEffects() const
{
    std::list<const char*> supportedEffects(SUPPORTED_EFFECTS);
    return supportedEffects;
}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void LightStrand::serializeSepecializedData(JsonObject& currentState) const
{
    JsonArray uiElementsArray = currentState[UI_ELEMENTS_ARRAY_ELEMENT];
    appendNumericElement(uiElementsArray, "Strand Length", 0, 1000, mNumericValues.at(STRAND_LENGTH_KEY), STRAND_LENGTH_KEY);
}

/*
** ============================================================================
** Update the total number of LEDs for this object
** ============================================================================
*/
void LightStrand::onParametersUpdated()
{
    mNumberOfLEDs = mNumericValues[STRAND_LENGTH_KEY];
}

/*
** ============================================================================
** Run the currently selected effect
** ============================================================================
*/
bool LightStrand::runSpecializedEffect()
{
    for (int address = mStartingAddress; address < mStartingAddress + mNumberOfLEDs; ++address)
    {
        if (address % 4 == 0)
        {
            setPixelColor(address, 0x00FF0000); // RED
        }
        else if (address % 4 == 1)
        {
            setPixelColor(address, 0x0003D000); // GREEN
        }
        else if (address % 4 == 2)
        {
            setPixelColor(address, 0x000200FF); // BLUE
        }
        else
        {
            setPixelColor(address, 0x00FF0085); // PURPLE
        }
    }

    return true;
}