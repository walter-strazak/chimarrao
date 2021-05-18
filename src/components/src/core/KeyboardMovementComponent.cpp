#include "KeyboardMovementComponent.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"
#include "BoxColliderComponent.h"

namespace components::core
{

KeyboardMovementComponent::KeyboardMovementComponent(ComponentOwner* ownerInit)
    : Component{ownerInit}, movementSpeed{7.f}
{
}

void KeyboardMovementComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (not animation)
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Animation component not found"};
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (not velocityComponent)
    {
        throw exceptions::DependentComponentNotFound{
            "KeyboardMovementComponent: Velocity component not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();

}

void KeyboardMovementComponent::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (input.isKeyPressed(input::InputKey::Left))
    {

        currentMovementSpeed.x = -movementSpeed;
        animation->setAnimationDirection(animations::AnimationDirection::Left);
    }
    else if (input.isKeyPressed(input::InputKey::Right))
    {
        currentMovementSpeed.x = movementSpeed;
        animation->setAnimationDirection(animations::AnimationDirection::Right);
    }
    else
    {
        currentMovementSpeed.x = 0.f;
    }

    if (input.isKeyPressed(input::InputKey::Up) && not canMoveDown && canMoveUp)
    {
        currentMovementSpeed.y = -2.89f * 5.f;
    }
    else
    {
        currentMovementSpeed.y += 20.f * deltaTime.count();
    }

    if (currentMovementSpeed.x == 0.f)
    {
        animation->setAnimation(animations::AnimationType::Idle);
    }
    else
    {
        animation->setAnimation(animations::AnimationType::Walk);
    }
    velocityComponent->setVelocity(currentMovementSpeed);
}

void KeyboardMovementComponent::lateUpdate(utils::DeltaTime deltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    auto currentMovementSpeed = velocityComponent->getVelocity();

    if (currentMovementSpeed.x < 0 and not canMoveLeft)
    {
        currentMovementSpeed.x = 0;
    }
    if (currentMovementSpeed.x > 0 and not canMoveRight)
    {
        currentMovementSpeed.x = 0;
    }
    if (currentMovementSpeed.y < 0 and not canMoveUp)
    {
        currentMovementSpeed.y = 0;
    }
    if (currentMovementSpeed.y > 0 and not canMoveDown)
    {
        currentMovementSpeed.y = 0;
    }

    velocityComponent->setVelocity(currentMovementSpeed);
    float xFrameMove = currentMovementSpeed.x * deltaTime.count();
    float yFrameMove = currentMovementSpeed.y * deltaTime.count();
    owner->transform->addPosition(xFrameMove, yFrameMove);
}

void KeyboardMovementComponent::setMovementSpeed(float speed)
{
    movementSpeed = speed;
}

float KeyboardMovementComponent::getMovementSpeed() const
{
    return movementSpeed;
}
}