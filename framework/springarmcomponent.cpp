// Oleg Kotov

#include "springarmcomponent.h"

void SpringArmComponent::tick( float deltaTime )
{
    Quaternion desiredRotation = getWorldRotation();
    Vector backVector = desiredRotation.transformVector( Vector::back );
    
    Vector desiredLocation = getWorldPosition();
    desiredLocation += backVector * m_targetArmLength;
}

