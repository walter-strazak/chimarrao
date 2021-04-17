#include "EditorMenuState.h"

#include "EditorMenuStateUIConfigBuilder.h"
#include "GetProjectPath.h"
#include "ui/DefaultUIManager.h"

namespace game
{

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::stack<std::unique_ptr<State>>& statesInit,
                                 std::unique_ptr<components::ui::UIManager> uiManagerInit, TileMap& tileMap)
    : State{windowInit, rendererPoolInit, statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMap}
{
    uiManager->createUI(EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(this));
    possibleLeaveFromStateTimer.start();
}

NextState EditorMenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        shouldBackToEditor = true;
    }

    if (shouldBackToEditor)
    {
        return NextState::Previous;
    }

    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void EditorMenuState::lateUpdate(const utils::DeltaTime&) {}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

std::string EditorMenuState::getName() const
{
    return "Editor menu state";
}

void EditorMenuState::activate()
{
    active = true;
    uiManager->activate();
    possibleLeaveFromStateTimer.restart();
}

void EditorMenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

}