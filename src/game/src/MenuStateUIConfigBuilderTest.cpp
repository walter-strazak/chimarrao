#include "MenuStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "ButtonsNavigatorMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "MenuState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedButtonNames{
    "menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton", "menuExitButton"};
const std::vector<std::string> expectedImagesNames{"menuIcon1Image", "menuIcon2Image", "menuIcon3Image",
                                                   "menuIcon4Image", "menuIcon5Image"};
}
class MenuStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<NiceMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<NiceMock<utils::FileAccessMock>>();
    NiceMock<StatesMock> states;
    std::shared_ptr<components::ui::UIManagerMock> uiManager{
        std::make_shared<NiceMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<NiceMock<ButtonsNavigatorMock>> buttonsNavigatorInit{
        std::make_unique<NiceMock<ButtonsNavigatorMock>>()};
    NiceMock<ButtonsNavigatorMock>* buttonsNavigator = buttonsNavigatorInit.get();
    MenuState menuState{window, rendererPool, fileAccess, states, uiManager, std::move(buttonsNavigatorInit)};
};

TEST_F(MenuStateUIConfigBuilderTest, createMenuUI)
{
    const auto menuUI = MenuStateUIConfigBuilder::createMenuUIConfig(&menuState);

    std::vector<std::string> actualButtonsNames;
    std::transform(menuUI->buttonsConfig.begin(), menuUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(menuUI->imagesConfig.begin(), menuUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(menuUI->backgroundConfig->uniqueName, "menuBackground");
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(menuUI->labelsConfig.empty());
    ASSERT_TRUE(menuUI->checkBoxesConfig.empty());
    ASSERT_TRUE(menuUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}

TEST_F(MenuStateUIConfigBuilderTest, getGridButtonNames)
{
    const auto actualButtonNames = MenuStateUIConfigBuilder::getButtonNames();

    ASSERT_EQ(actualButtonNames, expectedButtonNames);
}

TEST_F(MenuStateUIConfigBuilderTest, getIconNames)
{
    const auto actualIconNames = MenuStateUIConfigBuilder::getIconNames();

    ASSERT_EQ(actualIconNames, expectedImagesNames);
}