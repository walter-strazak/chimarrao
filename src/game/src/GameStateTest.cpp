#include "GameState.h"

#include "gtest/gtest.h"

#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "ProjectPathReader.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const auto brickTexturePath = utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png";
}

class GameStateTest_Base : public Test
{
public:
    GameStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*rendererPool, acquire(utils::Vector2f{5, 5}, utils::Vector2f{10, 10},
                                           graphics::Color{128, 91, 50}, graphics::VisibilityLayer::Second));
        EXPECT_CALL(*rendererPool, acquire(utils::Vector2f{5, 5}, utils::Vector2f{30, 30}, brickTexturePath,
                                           graphics::VisibilityLayer::Second));
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(2);
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
        EXPECT_CALL(*rendererPool, release(_)).Times(2);
        EXPECT_CALL(*rendererPool, setTexture(_, _, _));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
    StrictMock<StatesMock> states;
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class GameStateTest : public GameStateTest_Base
{
public:
    GameState gameState{window,
                        rendererPool,
                        fileAccess,
                        states,
                        std::move(uiManagerInit),
                        std::move(componentOwnersManagerInit)};
};

TEST_F(GameStateTest, activate_shouldActivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, activate());
    EXPECT_CALL(*uiManager, activate());

    gameState.activate();
}

TEST_F(GameStateTest, deactivate_shouldDeactivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, deactivate());
    EXPECT_CALL(*uiManager, deactivate());

    gameState.deactivate();
}

TEST_F(GameStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(gameState.getType(), StateType::Game);
}

TEST_F(GameStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    gameState.render();
}

TEST_F(GameStateTest, update_shouldUpdateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, update(deltaTime, Ref(input)));
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    gameState.update(deltaTime, input);
}