#include "EditorState.h"

#include <editor/TileMapSerializerJson.h>
#include <memory>

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorStateTest_Base : public Test
{
public:
    EditorStateTest_Base()
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
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    TileMap tileMap{
        "", {0, 0}, std::make_unique<TileMapSerializerJson>(), std::make_shared<utils::FileAccessMock>()};
};

class EditorStateTest : public EditorStateTest_Base
{
public:
    EditorState editorState{window,
                            rendererPool,
                            fileAccess,
                            states,
                            std::move(uiManagerInit),
                            std::make_shared<TileMap>(std::move(tileMap))};
};

TEST_F(EditorStateTest, xxx) {}