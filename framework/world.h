// Oleg Kotov

#pragma once

#include "baseapi.h"
#include "vector.h"
#include "quaternion.h"

#include <vector>
#include <string>

class Entity;
struct HitResult;
class TimerManager;

class ENGINE_API Level
{
public:

    Level();
    virtual ~Level();

public:
    
    virtual void beginPlay();
    virtual void tick( float deltaTime );

    const std::vector<Entity*>& getEntities() const;
    Entity* getEntityByName( const std::string& name ) const;

    Entity* spawnEntity( Entity* m_entity );
	void removeEntity( Entity* m_entity );

    // Entity* spawnEntity( const std::string& classname, const Vector& position = Vector::Zero, const Quaternion& rotation = Quaternion::Identity );
    Entity* spawnEntity( const std::string& classname, const Vector& position = Vector::zero, const Vector& rotation = Vector::zero );

    bool raycast( HitResult& raycastResult, const Vector& source, const Vector& target ); // direction?

    float deltaTime() const;

    void setGravity( const Vector& gravity );
    Vector getGravity() const;

    // drawSkybox() ?

    void setPlayerEntity( Entity* entity );
    Entity* getPlayerEntity() const;

    TimerManager* timerManager() const;

private:

	std::vector<Entity*> m_entities;

    bool m_isBeginPlay = false;
    Entity* m_player = nullptr;

    TimerManager* m_timerManager = nullptr;
};

