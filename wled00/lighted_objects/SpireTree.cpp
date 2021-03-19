#include "SpireTree.h"

const char* SpireTree::LIGHTED_OBJECT_TYPE_NAME = "Spire Tree";
std::initializer_list<const char*> SpireTree::SUPPORTED_EFFECTS = {"Solid", "Multi-Color Solid", "Decorate"};

/*
** ============================================================================
** Constructor
** ============================================================================
*/
SpireTree::SpireTree()
    : BaseLightedObject()
{
    mNumberOfLEDs = 300;
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
** Returns a list of supported effects for this lighted object
** ============================================================================
*/
std::list<const char*> SpireTree::getSupportedEffects() const
{
    std::list<const char*> supportedEffects(SUPPORTED_EFFECTS);
    return supportedEffects;
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
