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
    //mNumberOfLEDs = 300;
    mNumberOfLEDs = 5;
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
bool Present::runEffect(uint32_t delta)
{
    for (int address = mStartingAddress; address < mStartingAddress + mNumberOfLEDs; ++address)
    {
        if (mPoweredOn)
        {
            setPixelColor(address, 0x00FF0000);
        }
        else
        {
            setPixelColor(address, 0x00000000);
        }
    }

    return true;
}
