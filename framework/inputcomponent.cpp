// Oleg Kotov

#include "inputcomponent.h"
#include "inputsystem.h"
#include "okoengine.h"

InputComponent::InputComponent()
{
	engine()->inputSystem()->addInputComponent( this );
}

InputComponent::~InputComponent()
{
	engine()->inputSystem()->removeInputComponent( this );
}

