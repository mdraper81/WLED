#include "BaseLightedObject.h"

#include "const.h"

std::initializer_list<const char*> BaseLightedObject::SUPPORTED_EFFECTS = {"None"};
const char* BaseLightedObject::EFFECT_KEY = "effect";
const char* ILightedObject::TYPE_ELEMENT = "type";
const char* BaseLightedObject::SELECTED_EFFECT_ELEMENT = "selectedEffect";
const char* BaseLightedObject::BRIGHTNESS_PCT_ELEMENT = "brightnessPct";
const char* BaseLightedObject::POWERED_ON_ELEMENT = "poweredOn";  
const char* BaseLightedObject::UI_ELEMENTS_ARRAY_ELEMENT = "UIElements";

/*
** ============================================================================
** Constructor
** ============================================================================
*/
BaseLightedObject::BaseLightedObject()
    : mPixelWrapper( nullptr )
    , mTotalTimeRunning( 0 )
    , mStartingAddress( 0 )
    , mNumberOfLEDs( 50 )
    , mPoweredOn( true )
{
    mDropDownSelections[EFFECT_KEY] = 0;
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
** Returns a list of supported effects for this lighted object
** ============================================================================
*/
std::list<const char*> BaseLightedObject::getSupportedEffects() const
{
    std::list<const char*> supportedEffects(SUPPORTED_EFFECTS);
    return supportedEffects;
}

/*
** ============================================================================
** Updates the color for every pixel for this object to the value that it should
** have for this 'frame'.  Note that this handles deciding if the pixels should
** be updated by the specialized effect function or turned off.
**
**  param delta - the number of milliseconds since the last update
** ============================================================================
*/
bool BaseLightedObject::runEffect(uint32_t delta)
{
    mTotalTimeRunning += delta;

    if (mPoweredOn)
    {
        return runSpecializedEffect();
    }
    else
    {
        turnOffPixelsInRange(mStartingAddress, mNumberOfLEDs);
        return true;
    }
}

/*
** ============================================================================
** Updates the parameter values for this object using the values provided in
** the incoming userInputValues
**
**  param userInputValues - JsonArray that contains user input objects defining
**        the values to set each parameter to (coming from the user interface)
** ============================================================================
*/
void BaseLightedObject::update(const char* userInputValues)
{
    StaticJsonDocument<1024> jsonDoc;
    deserializeJson(jsonDoc, userInputValues);

    JsonArray userInputValueArray = jsonDoc.as<JsonArray>();
    deserializeUiElements(userInputValueArray);

    onParametersUpdated();
}

/*
** ============================================================================
** Populates the base state variables using the passed in Json Object
**
**  param newState - Json Object representing the desired state of this ILightedObject
** ============================================================================
*/
void BaseLightedObject::deserializeAndApplyStateFromJson(JsonObject newState)
{
    // Deserialize state variables
    POPULATE_FROM_JSON(mPoweredOn, newState[POWERED_ON_ELEMENT]);
    
    // Deserialize all UI Elements
    JsonArray uiElementsArray = newState[UI_ELEMENTS_ARRAY_ELEMENT];
    deserializeUiElements(uiElementsArray);

    // Hand off to derived class to deserialize any specialized data
    deserializeSpecializedData(newState);

    // Fire the hook to let the derived class know that parameters have been updated
    onParametersUpdated();
}

/*
** ============================================================================
** Serializes the common state info for a lighted object into the currentState
** Json object.
**
**  param   currentState - JSON object to serialize into
** ============================================================================
*/
void BaseLightedObject::serializeCurrentStateToJson(JsonObject& currentState) const
{
    // Serialize state variables
    currentState[TYPE_ELEMENT] = String(getObjectType().c_str());
    currentState[POWERED_ON_ELEMENT] = mPoweredOn;

    // Serialize all UI Elements
    JsonArray uiElementsArray = currentState.createNestedArray(UI_ELEMENTS_ARRAY_ELEMENT);
    appendCommonUiElements(uiElementsArray);    

    // Hand off to derived class to serialize any specialized data
    serializeSepecializedData(currentState);
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
** Set the pixels in the given address range to the given color
** ============================================================================
*/
void BaseLightedObject::setPixelColorForRange(uint16_t startingAddress, uint16_t numPixels, uint32_t color)
{
    for (int address = startingAddress; address < startingAddress + numPixels; ++address)
    {
        setPixelColor(address, color);
    }
}

/*
** ============================================================================
** Turn off the pixels in the given address range
** ============================================================================
*/
void BaseLightedObject::turnOffPixelsInRange(uint16_t startingAddress, uint16_t numPixels)
{
    // Set all pixels in this range to black to turn them off
    setPixelColorForRange(startingAddress, numPixels, 0x00000000);
}


/*
** ============================================================================
** Append common ui elements that all Lighted Objects share
**
**  param uiElementsArray - ui elments array to append to
** ============================================================================
*/
void BaseLightedObject::appendCommonUiElements(JsonArray& uiElementsArray) const
{
    appendTitleElement(uiElementsArray, "%s", getObjectType().c_str());

    int lastAddress = mStartingAddress + mNumberOfLEDs - 1;
    appendStringElement(uiElementsArray, TextTypeSmall, "Address range %d to %d (%d LEDs)", mStartingAddress, lastAddress, mNumberOfLEDs);
    
    appendDropDownElement(uiElementsArray, getSupportedEffects(), mDropDownSelections.at(EFFECT_KEY), "Effect:", EFFECT_KEY);
}

/*
** ============================================================================
** Appends a Drop Down UI Element to the given uiElementsArray.  
**
**  param   uiElementsArray - ui elements array to append to
**  param   options - Json array of options to populate the drop down with
**  param   selectedIndex - Which index to select
**  param   inputKey - the key that identifies this input when parsing the save object JSON request
** ============================================================================
*/
void BaseLightedObject::appendDropDownElement(JsonArray& uiElementsArray, std::list<const char*> optionsList, int selectedIndex, const char* label, const char* inputKey) const
{
    JsonObject dropDownElement = uiElementsArray.createNestedObject();
    dropDownElement["elementType"] = "dropdown";
    dropDownElement["selectedIndex"] = selectedIndex;
    dropDownElement["label"] = String(label);
    dropDownElement["inputKey"] = inputKey;

    JsonArray dropDownOptions = dropDownElement.createNestedArray("options");
    for (const char* option : optionsList)
    {
        dropDownOptions.add(option);
    }    
}

/*
** ============================================================================
** Appends a numeric UI element to the given uiElementsArray
**
**  param   uiElementsArray - ui elements array to append to
**  param   label - text to use as the label for the numeric element
**  param   minValue - minimum value to allow for the numeric
**  param   maxValue - maximum value to allow for the numeric
**  param   currentValue - current value to populate the numeric with
**  param   inputKey - the key that identifies this input when parsing the save object JSON request
** ============================================================================
*/
void BaseLightedObject::appendNumericElement(JsonArray& uiElementsArray, const char* label, int minValue, int maxValue, const int currentValue, const char* inputKey) const
{
    JsonObject numericElement = uiElementsArray.createNestedObject();
    numericElement["elementType"] = "numeric";
    numericElement["label"] = label;
    numericElement["minValue"] = minValue;
    numericElement["maxValue"] = maxValue;
    numericElement["value"] = currentValue;
    numericElement["inputKey"] = inputKey;
}

/*
** ============================================================================
** Appends a string UI element to the given uiElementsArray
**
**  param   uiElementsArray - ui elements array to append to
**  param   format - printf style formatting string
**  param   ... - printf style arguments for formatting
** ============================================================================
*/
void BaseLightedObject::appendStringElement(JsonArray& uiElementsArray, TextTypeE textType, const char* format, ...) const
{
    char newUIString[MAX_UI_STRING_LENGTH];

    // Format the incoming string properly
    va_list args;
    va_start(args, format);
    vsnprintf(newUIString, MAX_UI_STRING_LENGTH, format, args);
    va_end(args);

    // Add the string to the JSON UI Elements array
    JsonObject stringElement = uiElementsArray.createNestedObject();
    stringElement["elementType"] = "string";
    stringElement["value"] = String(newUIString);

    switch (textType)
    {
        case TextTypeSmall:
            stringElement["class"] = "lighted_object_small_text";
            break;

        case TextTypeLarge:
            stringElement["class"] = "lighted_object_large_text";
            break;
    }
}

/*
** ============================================================================
** Deserializes the ui elements in the provided Json Array to set the correct
** values for each parameter in this object
**
**  param uiElementsArray - JSON array containing ui element information
** ============================================================================
*/
void BaseLightedObject::deserializeUiElements(const JsonArray& uiElementsArray)
{
    for (JsonObject uiParameter : uiElementsArray)
    {
        String elementType = uiParameter["elementType"];
        if (elementType.equalsIgnoreCase("numeric"))
        {
            String key = uiParameter["inputKey"];
            int value = uiParameter["value"];
            mNumericValues[key.c_str()] = value;
        }
        else if (elementType.equalsIgnoreCase("dropdown"))
        {
            String key = uiParameter["inputKey"];
            String value = uiParameter["value"];
            int selectedIndex = uiParameter["selectedIndex"];
            mDropDownSelections[key.c_str()] = selectedIndex;
        }
    }
}

/*
** ============================================================================
** Appends a Title element to the given uiElementsArray
**
**  param   uiElementsArray - ui elements array to append to
**  param   format - printf style formatting string
**  param   ... - printf style arguments for formatting
** ============================================================================
*/
void BaseLightedObject::appendTitleElement(JsonArray& uiElementsArray, const char* format, ...) const
{
   char newUIString[MAX_UI_STRING_LENGTH];

    // Format the incoming string properly
    va_list args;
    va_start(args, format);
    vsnprintf(newUIString, MAX_UI_STRING_LENGTH, format, args);
    va_end(args);

    // Add the string to the JSON UI Elements array
    JsonObject stringElement = uiElementsArray.createNestedObject();
    stringElement["elementType"] = "title";
    stringElement["value"] = String(newUIString);
    stringElement["poweredOn"] = mPoweredOn;
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

// MDR DEBUG TODO - Add hardcoded colorsets
//                  Apply color set onto a lighted object
//                  Allow color set offset adjustment
//                  Bug: Adjust total number of leds in display and then try to turn leds on/off.  The display is in a messed up state, probably need to clear all lighted objects when reloading.
//                  Bug: After clearing all objects I have LEDs still on, maybe need to turn the LEDs off before clearing the object.
//                  Implement some basic effects
// MDR DEBUG TODO - Investigate overlap between cfg.cpp settings and lightDisplay.cpp settings
//                  see https://www.toptal.com/designers/htmlarrows/arrows/)
//                  Add numeric slider control
//                  Add effect intensity & speed controllers to snowflake
//                  Create Scene objects
//                  Move scenes up/down
//                  Remove scenes
//                  Save/Load Scenes
//                  Use a json to load color sets (create it with default sets if it doesn't exist)
//                  Implement functionality to edit color sets
// MDR DEBUG TODO - update NodeMCU LED to blink instead of remaining on (seems to be a problem with NeoPixelWrapper)
// MDR DEBUG TODO - clean up bri, briT, briLast and the stuff related to the night dimming mode
