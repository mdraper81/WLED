#include "BaseLightedObject.h"

/*
** ============================================================================
** Constructor
** ============================================================================
*/
BaseLightedObject::BaseLightedObject()
    : mPixelWrapper( nullptr )
    , mStartingAddress( 0 )
    , mNumberOfLEDs( 0 )
    , mSelectedEffectId( 0 )
{
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
BaseLightedObject::~BaseLightedObject()
{
}

/*
** ============================================================================
** Set the pixel at the given address to the given color
** ============================================================================
*/
void BaseLightedObject::setPixelColor(uint16_t address, uint32_t color)
{
    byte white  = (color >> 24);
    byte red    = (color >> 16);
    byte green  = (color >>  8);
    byte blue   =  color;
    setPixelColor(address, red, green, blue, white);
}

/*
** ============================================================================
** Serialize common state variables that all Lighted Objects share
** ============================================================================
*/
void BaseLightedObject::serializeCommonState(JsonObject& currentState) const
{
    currentState["object_type"] = String(getObjectType().c_str());
    currentState["startingAddress"] = mStartingAddress;
    currentState["numLEDs"] = mNumberOfLEDs;
    currentState["selectedEffectId"] = mSelectedEffectId;
}

/*
** ============================================================================
** Set the pixel at the given adress to show the colors specified by red, green,
** blue, and white
** ============================================================================
*/
void BaseLightedObject::setPixelColor(uint16_t address, byte red, byte green, byte blue, byte white)
{
    if (nullptr != mPixelWrapper)
    {
        // TODO: Consider taking functionality from FX_fcn.cpp to support reverse mode, skip first, etc
        RgbwColor color;
        color.R = red; 
        color.G = green; 
        color.B = blue; 
        color.W = white;
        mPixelWrapper->SetPixelColor(address, color);
    }
}

// MDR DEBUG TODO - update lighted objects to return HTML for their own settings
// MDR DEBUG TODO - Add dropdown to select effect for lighted object
// MDR DEBUG TODO - Add functionality to edit lighted object
// MDR DEBUG TODO - Define JSON object to specify the light strand settings and the lighted object settings
// MDR DEBUG TODO - Test that we can create a strand and set the solid color and that it is purple
// MDR DEBUG TODO - Add support to set a color set onto a lighted object
// MDR DEBUG TODO - Add support to add a color set offset onto a lighted object
// MDR DEBUG TODO - Replace "strip" global variable with light display.  May require adding functionality to LightDisplay
// MDR DEBUG TODO - update NodeMCU LED to blink instead of remaining on (seems to be a problem with NeoPixelWrapper)

