// Oleg Kotov

#pragma once

#include "entityfactorydictionary.h"

class Entity;

class ENGINE_API IEntityFactory
{
public:

    ~IEntityFactory() {}
    virtual Entity* create() = 0;
};

template <class EntityClass>
class EntityFactory : public IEntityFactory
{
public:

    EntityFactory( const std::string& classname )
    {
        m_classname = classname;
        EntityFactoryDictionary::instance()->installFactory( classname, this );
    }

    Entity* create()
    {
        Entity* entity = new EntityClass();
        entity->m_classname = m_classname;
        return entity;
    }

private:

    std::string m_classname;
};

