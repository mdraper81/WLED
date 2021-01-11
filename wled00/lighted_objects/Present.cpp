#include "Present.h"

const char* Present::LIGHTED_OBJECT_TYPE_NAME = "Present";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
Present::Present()
    : BaseLightedObject()
{

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

/*
** ============================================================================
** Deserialize the given newState object and apply those values to this 
** lighted object.  This applies changes from the web
** ============================================================================
*/
void Present::deserializeAndApplyStateFromJson(JsonObject newState)
{

}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void Present::serializeCurrentStateToJson(JsonObject& currentState) const
{
    serializeCommonState(currentState);
}