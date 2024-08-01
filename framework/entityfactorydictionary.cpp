// Oleg Kotov

#include "entityfactorydictionary.h"
#include "entityfactory.h"

EntityFactoryDictionary* EntityFactoryDictionary::m_instance = nullptr;

EntityFactoryDictionary* EntityFactoryDictionary::instance()
{
    if ( !m_instance )
    {
        m_instance = new EntityFactoryDictionary();
    }
    
    return m_instance;
}

void EntityFactoryDictionary::installFactory( const std::string& classname, IEntityFactory* factory )
{
    m_dictionary[classname] = factory;
}

IEntityFactory* EntityFactoryDictionary::findFactory( const std::string& classname )
{
    return m_dictionary[classname];
}

Entity* EntityFactoryDictionary::create( const std::string& classname )
{
    return m_dictionary[classname]->create();
}

const std::map<std::string, IEntityFactory*>& EntityFactoryDictionary::getList() const
{
    return m_dictionary;
}

