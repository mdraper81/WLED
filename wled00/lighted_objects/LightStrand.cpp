#include "LightStrand.h"

const char* LightStrand::LIGHTED_OBJECT_TYPE_NAME = "Strand";
std::initializer_list<const char*> LightStrand::SUPPORTED_EFFECTS = {"Solid", "Multi-Color Solid", "Chase"};

/*
** ============================================================================
** Constructor
** ============================================================================
*/
LightStrand::LightStrand()
    : BaseLightedObject()
{
    mNumberOfLEDs = 50;
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
** Run the currently selected effect
** ============================================================================
*/
uint16_t LightStrand::runEffect()
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
void LightStrand::serializeSepecializedData(JsonObject& currentState) const
{
    JsonArray uiElementsArray = currentState[UI_ELEMENTS_ARRAY_ELEMENT];
    appendNumericElement(uiElementsArray, "Strand Length", 0, 1000, 50, "numLights"); // MDR DEBUG - TODO - Get an appropriate max value
}