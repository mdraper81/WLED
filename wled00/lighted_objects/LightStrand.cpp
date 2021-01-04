#include "LightStrand.h"

/*
** ============================================================================
** Constructor
** ============================================================================
*/
LightStrand::LightStrand(NeoPixelWrapper *neoPixelWrapper)
    : BaseLightedObject(neoPixelWrapper)
{

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
** Deserialize the given newState object and apply those values to this 
** lighted object.  This applies changes from the web
** ============================================================================
*/
void LightStrand::deserializeAndApplyStateFromJson(JsonObject newState)
{

}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void LightStrand::serializeCurrentStateToJson(JsonObject& currentState) const
{

}