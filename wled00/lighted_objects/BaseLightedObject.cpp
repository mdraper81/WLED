#include "BaseLightedObject.h"

/*
** ============================================================================
** Constructor
** ============================================================================
*/
BaseLightedObject::BaseLightedObject(NeoPixelWrapper *neoPixelWrapper)
    : mPixelWrapper( neoPixelWrapper )
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
** Set the pixel at the given adress to show the colors specified by red, green,
** blue, and white
** ============================================================================
*/
void BaseLightedObject::setPixelColor(uint16_t address, byte red, byte green, byte blue, byte white)
{
    // TODO: Consider taking functionality from FX_fcn.cpp to support reverse mode, skip first, etc
    RgbwColor color;
    color.R = red; 
    color.G = green; 
    color.B = blue; 
    color.W = white;
    mPixelWrapper->SetPixelColor(address, color);
}

// MDR DEBUG TODO - Check changes into git so that we have tracking and can roll back
// MDR DEBUG TODO - Hook up new LightDisplay (see wled.h) to actually run in place of WS2812FX.  replace "strip" global variable.  May require adding functionality to LightDisplay
// MDR DEBUG TODO - Define JSON object to specify the light strand settings and the lighted object settings
// MDR DEBUG TODO - Create a LightStrand object and hack it into Light Display by default to see if we can test it (WIP)
// MDR DEBUG TODO - Test that we can create a strand and set the solid color and that it is purple
// MDR DEBUG TODO - Add support to set a color set onto a lighted object
// MDR DEBUG TODO - Add support to add a color set offset onto a lighted object
// MDR DEBUG TODO - update lighted objects to return HTML for their own settings
// MDR DEBUG TODO - update NodeMCU LED to blink instead of remaining on

