// Oleg Kotov

#include "world.h"
#include "entity.h"
#include "physicsengine.h"
#include "clock.h"
#include "gameplay.h"
#include "okoengine.h"
#include "timermanager.h"

Level::Level()
{
	m_timerManager = new TimerManager();
}

Level::~Level()
{
	for ( auto& entity : m_entities )
	{
		SafeDelete( entity );
	}

	SafeDelete( m_timerManager );
}

void Level::beginPlay()
{
	m_isBeginPlay = true;

	// awake?

	std::vector<Entity*> entitiesToInit = m_entities;

	for ( auto& entity : entitiesToInit )
	{
		entity->beginPlay();
	}
}

void Level::tick( float deltaTime )
{
	std::vector<Entity*> entitiesToTick = m_entities;

	for ( const auto& entity : entitiesToTick )
    {
		if ( !entity->isEnabled() ) continue;
        entity->tick( deltaTime );        
    }

	m_timerManager->update( deltaTime );
}

const std::vector<Entity*>& Level::getEntities() const
{
    return m_entities;
}

Entity* Level::getEntityByName( const std::string& name ) const
{
	for ( auto& entity : m_entities )
	{
		if ( entity->getName() == name )
		{
			return entity;
		}
	}

	return nullptr;
}

Entity* Level::spawnEntity( Entity* entity )
{
	if ( entity->isInWorld() ) return entity;

	m_entities.push_back( entity );

	entity->m_world = this;

	if ( m_isBeginPlay == true )
	{
		entity->beginPlay();
	}

	return entity;
}

void Level::removeEntity( Entity* entity )
{
	for ( int32_t i = 0; i < m_entities.size(); ++i )
	{
		if ( m_entities[i] == entity )
		{
			std::swap( m_entities[i], m_entities.back() );
			m_entities.pop_back();
		}
	}

	entity->m_world = nullptr;
	// entity->EndPlay(); ?
}

//Entity* World::spawnEntity( const std::string& classname, const Vector& position /* = Vector::Zero */, const Quaternion& rotation /* = Quaternion::Identity */ )
//{
//	Entity* entity = createEntityByName( classname );
//	assert( entity != nullptr );
//
//	entity->setWorldPosition( position );
//	entity->setWorldRotation( rotation );
//
//	spawnEntity( entity );
//
//	return entity;
//}

Entity* Level::spawnEntity( const std::string& classname, const Vector& position /* = Vector::Zero */, const Vector& rotation /* = Vector::Zero */ )
{
	Entity* entity = createEntityByName( classname );
	assert( entity );

	entity->setWorldPosition( position );
	entity->setWorldEulerAngles( rotation );

	spawnEntity( entity );

	return entity;
}

bool Level::raycast( HitResult& raycastResult, const Vector& source, const Vector& target )
{
	auto physicsEngine = OkoEngine::instance()->physics();
	return physicsEngine->raycast( raycastResult, source, target );
}

float Level::deltaTime() const
{
	return OkoEngine::instance()->gameClock()->getDeltaTime();
}

void Level::setGravity( const Vector& gravity )
{
	OkoEngine::instance()->physics()->setGravity( gravity );
}

Vector Level::getGravity() const
{
	return OkoEngine::instance()->physics()->getGravity();
}

void Level::setPlayerEntity( Entity* entity )
{
	m_player = entity;
}

Entity* Level::getPlayerEntity() const
{
	return m_player;
}

TimerManager* Level::timerManager() const
{
	return m_timerManager;
}

