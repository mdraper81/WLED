#pragma once

#include <string>
#include <list>
#include <unordered_map>

// Forward Declarations
class ILightedObject;
class NeoPixelWrapper;

// Type Definitions
typedef ILightedObject* (*lightedObjectGenerator)();
typedef std::list<std::string> StringList;

/*
**-----------------------------------------------------------------------------
** LightedObjectFactory is a factory class that is used to create lighted objects.
** New lighted object types (which derive from ILightedObject via BaseLightedObject)
** can register themselves with this class.  The calling code can query the factory
** to get a complete list of supported lighted object types.
**-----------------------------------------------------------------------------
*/ 
class LightedObjectFactory
{
public:
    static LightedObjectFactory& get();

    bool registerLightedObjectType(std::string objectType, const lightedObjectGenerator& funcCreate);

    StringList getListOfLightedObjectTypes() const;

    ILightedObject* createLightedObject(std::string objectType, NeoPixelWrapper *neoPixelWrapper);

private:
    LightedObjectFactory();
    LightedObjectFactory(const LightedObjectFactory&);
    virtual ~LightedObjectFactory();

    std::unordered_map<std::string, lightedObjectGenerator> mGeneratorFunctions;
};
