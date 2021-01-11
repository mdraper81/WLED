#include "SnowFlake.h"

const char* SnowFlake::LIGHTED_OBJECT_TYPE_NAME = "Snow Flake";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
SnowFlake::SnowFlake()
    : BaseLightedObject()
{

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
** Deserialize the given newState object and apply those values to this 
** lighted object.  This applies changes from the web
** ============================================================================
*/
void SnowFlake::deserializeAndApplyStateFromJson(JsonObject newState)
{

}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void SnowFlake::serializeCurrentStateToJson(JsonObject& currentState) const
{
    serializeCommonState(currentState);
}