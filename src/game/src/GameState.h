#pragma once

#include "CollisionSystem.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "ui/UIManager.h"

namespace game
{
class GameState : public State
{
public:
    explicit GameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                       std::shared_ptr<utils::FileAccess>, States&,
                       std::unique_ptr<components::ui::UIManager>, std::unique_ptr<physics::CollisionSystem>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;
    void pause();

private:
    bool paused;
    utils::Timer timer;
    const float timeAfterStateCouldBePaused;
    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> obstacle;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> objects;
    std::unique_ptr<physics::CollisionSystem> collisionSystem;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}