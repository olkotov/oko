// Oleg Kotov

#pragma once

#include "okobase.h"

class Entity;
class IEntityFactory;

class ENGINE_API EntityFactoryDictionary
{    
public:
    
    static EntityFactoryDictionary* instance();
    
    void installFactory( const std::string& classname, IEntityFactory* factory );
    IEntityFactory* findFactory( const std::string& classname );
    Entity* create( const std::string& classname );

    const std::map<std::string, IEntityFactory*>& getList() const;

private:

    std::map<std::string, IEntityFactory*> m_dictionary;
    static EntityFactoryDictionary* m_instance;
};

