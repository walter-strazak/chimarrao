#pragma once

#include "AnimationComponent.h"
#include "AttackStrategy.h"
#include "ComponentOwner.h"

namespace components::core
{
class ArtificialIntelligenceAttackComponent : public Component
{
public:
    ArtificialIntelligenceAttackComponent(ComponentOwner* owner, ComponentOwner* target, std::shared_ptr<AttackStrategy>);

    void loadDependentComponents() override;
    void update(utils::DeltaTime, const input::Input&) override;

private:
    ComponentOwner* target;
    bool attemptToAttack{false};
    std::shared_ptr<AnimationComponent> animation;
    std::shared_ptr<AttackStrategy> attackStrategy;
};
}
