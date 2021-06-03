#include "EditorMenuState.h"

#include <memory>

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "ui/UIManagerMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorMenuStateTest_Base : public Test
{
public:
    EditorMenuStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class EditorMenuStateTest : public EditorMenuStateTest_Base
{
public:
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    EditorMenuState editorMenuState{window, rendererPool, fileAccess, states, uiManager, tileMap};
};

TEST_F(EditorMenuStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    editorMenuState.activate();
}

TEST_F(EditorMenuStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    editorMenuState.deactivate();
}

TEST_F(EditorMenuStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(editorMenuState.getType(), StateType::EditorMenu);
}

TEST_F(EditorMenuStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    editorMenuState.render();
}

TEST_F(EditorMenuStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    editorMenuState.update(deltaTime, input);
}