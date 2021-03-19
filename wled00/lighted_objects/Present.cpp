#include "Present.h"

const char* Present::LIGHTED_OBJECT_TYPE_NAME = "Present";
std::initializer_list<const char*> Present::SUPPORTED_EFFECTS = {"Solid", "Unwrap"};

/*
** ============================================================================
** Constructor
** ============================================================================
*/
Present::Present()
    : BaseLightedObject()
{
    mNumberOfLEDs = 300;
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
Present::~Present()
{
}

/*
** ============================================================================
** Returns a list of supported effects for this lighted object
** ============================================================================
*/
std::list<const char*> Present::getSupportedEffects() const
{
    std::list<const char*> supportedEffects(SUPPORTED_EFFECTS);
    return supportedEffects;
}

/*
** ============================================================================
** Run the currently selected effect
** ============================================================================
*/
uint16_t Present::runEffect()
{
    for (int address = mStartingAddress; address < mStartingAddress + mNumberOfLEDs; ++address)
    {
        setPixelColor(address, 0x00FF0085);
    }
}
