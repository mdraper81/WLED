#include "LightedObjectFactory.h"

#include "ILightedObject.h"
#include "NpbWrapper.h"

/*
** ============================================================================
** Get the instance of this singleton
** ============================================================================
*/
LightedObjectFactory& LightedObjectFactory::get()
{
    static LightedObjectFactory instance;
    return instance;
}

/*
** ============================================================================
** Constructor
** ============================================================================
*/
LightedObjectFactory::LightedObjectFactory()
{
}

/*
** ============================================================================
** Copy Constructor
** ============================================================================
*/
LightedObjectFactory::LightedObjectFactory(const LightedObjectFactory& other)
{
    for (auto generatorEntry : other.mGeneratorFunctions)
    {
        registerLightedObjectType(generatorEntry.first, generatorEntry.second);
    }   
}

/*
** ============================================================================
** Destructor
** ============================================================================
*/
LightedObjectFactory::~LightedObjectFactory()
{
    mGeneratorFunctions.clear();
}

/*
** ============================================================================
** Registers a new lighted object type with this factory.
**
** param    objectType - The string that is used to identify the lighted object 
**          being registered
** param    funcCreate - The function that is used to create an instance of the
**          new lighted object
** returns  true if the lighted object type was successfully registered
** ============================================================================
*/
bool LightedObjectFactory::registerLightedObjectType(std::string objectType, const lightedObjectGenerator& funcCreate)
{
    // This will only insert the lightedObjectGenerator if it is not already registered.
    // False will be returned if the lightedObjectGenerator was already registered.
    return mGeneratorFunctions.insert(std::make_pair(objectType, funcCreate)).second;
}

/*
** ============================================================================
** Returns a list of all supported Lighted Object Types.  The strings in the 
** returned list are the identifiers used to create objects of that type.
** ============================================================================
*/
StringList LightedObjectFactory::getListOfLightedObjectTypes() const
{
    StringList availableObjectTypes;

    for (auto generatorEntry : mGeneratorFunctions)
    {
        availableObjectTypes.push_back(generatorEntry.first);
    }

    return availableObjectTypes;
}

/*
** ============================================================================
** Call this function to create a new instance of the given type of lighted object
**
** param    objectType - the object to generate
** returns  new instance of the given lighted object
** ============================================================================
*/
ILightedObject* LightedObjectFactory::createLightedObject(std::string objectType, NeoPixelWrapper* neoPixelWrapper)
{
    ILightedObject* newObject = nullptr;

    auto findIter = mGeneratorFunctions.find(objectType);
    if (findIter != mGeneratorFunctions.end())
    {
        // Call the registered lighted object generator function
        newObject = findIter->second();
        newObject->setNeoPixelWrapper(neoPixelWrapper);
    }

    return newObject;
}
