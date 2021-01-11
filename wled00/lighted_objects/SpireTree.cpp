#include "SpireTree.h"

const char* SpireTree::LIGHTED_OBJECT_TYPE_NAME = "Spire Tree";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
SpireTree::SpireTree()
    : BaseLightedObject()
{

}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
SpireTree::~SpireTree()
{
}

/*
** ============================================================================
** Run the currently selected effect
** ============================================================================
*/
uint16_t SpireTree::runEffect()
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
void SpireTree::deserializeAndApplyStateFromJson(JsonObject newState)
{

}

/*
** ============================================================================
** Populate the given currentState JSON object with the current state of this 
** lighted object.  This provides the current state to the web
** ============================================================================
*/
void SpireTree::serializeCurrentStateToJson(JsonObject& currentState) const
{
    serializeCommonState(currentState);
}