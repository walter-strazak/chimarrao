#include "SettingsStateUIConfigBuilder.h"

#include "GetProjectPath.h"
#include "SettingsState.h"
#include "ui/DefaultUIManager.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{25, 5};
const auto sectionTextFontSize{37};
const auto displayModeFontSize{12};
const auto displayModeButtonSize = utils::Vector2f{8, 3};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const std::string backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";

const auto settingsTitlePosition = utils::Vector2f{32, 6};
const auto displayModeSectionPosition = utils::Vector2f{38, 18};
const auto windowModeButtonPosition = utils::Vector2f{
    displayModeSectionPosition.x + 15.8f + displayModeButtonSize.x, displayModeSectionPosition.y + 0.5f};
const auto fullscreenModeButtonPosition =
    utils::Vector2f{windowModeButtonPosition.x + displayModeButtonSize.x + 0.5f, windowModeButtonPosition.y};
const auto resolutionSectionPosition = utils::Vector2f{38, 24};
const auto resolutionTextFieldPosition = utils::Vector2f{
    resolutionSectionPosition.x + 18.50f + displayModeButtonSize.x, resolutionSectionPosition.y + 0.8f};
const auto changeResolutionButtonSize = utils::Vector2f{2.5, 2};
const auto resolutionDecreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x - 2.8f, resolutionSectionPosition.y + 1.15f};
const auto resolutionIncreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x + 11.2f, resolutionSectionPosition.y + 1.15f};
const auto vsyncSectionPosition = utils::Vector2f{38, 30};
const auto vsyncButtonPosition =
    utils::Vector2f{vsyncSectionPosition.x + 31.0f, vsyncSectionPosition.y + 0.5f};
const auto vsyncButtonSize = utils::Vector2f{3, 3};
const auto frameLimitSectionPosition = utils::Vector2f{38, 36};
const auto frameLimitTextFieldPosition = utils::Vector2f{
    frameLimitSectionPosition.x + 22.5f + displayModeButtonSize.x, frameLimitSectionPosition.y + 0.8f};
const auto frameLimitDecreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x - 4.0f, frameLimitSectionPosition.y + 1.15f};
const auto frameLimitIncreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x + 4.5f, frameLimitSectionPosition.y + 1.15f};
const auto backToMenuButtonPosition = utils::Vector2f{34.5, 48};
const auto applyChangesButtonPosition = utils::Vector2f{55, 48};
}


std::unique_ptr<components::ui::UIConfig> SettingsStateUIConfigBuilder::createSettingsUIConfig(SettingsState* settingsState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "settingsBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, backgroundPath);

    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsTitleLabel", settingsTitlePosition, graphics::Color::Black, "Settings", 37, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    const auto backToMenuButtonSize = utils::Vector2f{13, 5};
    const auto backToMenuButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsBackToMenuButton",
                          buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { settingsState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, sectionTextFontSize, fontPath, utils::Vector2f{2, 0}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////

    const auto applyChangesButtonSize = utils::Vector2f{13, 5};
    const auto applyChangesButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsApplyChangesButton",
                          buttonHoverColor);
    };
    const auto applyChangesButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsApplyChangesButton",
                          buttonColor);
    };
    auto applyChangesButtonMouseOverActions =
        components::ui::MouseOverActions{applyChangesButtonOnMouseOver, applyChangesButtonOnMouseOut};
    auto applyChangesClickAction = [=] { settingsState->applyWindowSettingsChanges(); };
    auto applyChangesButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsApplyChangesButton", applyChangesButtonPosition, applyChangesButtonSize, buttonColor,
        "Apply", graphics::Color::Black, sectionTextFontSize, fontPath, utils::Vector2f{1, 0},
        applyChangesClickAction, applyChangesButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(applyChangesButtonConfig));
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto displayModeLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsDisplayModeLabel", displayModeSectionPosition,
                                                      graphics::Color::Black, "Display mode:", 30, fontPath);
    labelsConfig.emplace_back(std::move(displayModeLabelConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////
    auto windowModeClickAction = [=] { settingsState->setWindowMode(); };
    auto windowModeButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsWindowModeButton", windowModeButtonPosition, displayModeButtonSize, buttonColor, "Window",
        graphics::Color::Black, displayModeFontSize, fontPath, utils::Vector2f{2, 0.9}, windowModeClickAction);
    buttonsConfig.emplace_back(std::move(windowModeButtonConfig));

    ////////////////////////////////////////////////////////////////////

    auto fullscreenModeClickAction = [=] { settingsState->setFullscreenMode(); };
    auto fullscreenModeButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFullscreenModeButton", fullscreenModeButtonPosition, displayModeButtonSize, buttonColor,
        "Fullscreen", graphics::Color::Black, displayModeFontSize, fontPath, utils::Vector2f{0.5, 0.9},
        fullscreenModeClickAction);
    buttonsConfig.emplace_back(std::move(fullscreenModeButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    auto resolutionLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsResolutionLabel", resolutionSectionPosition,
                                                      graphics::Color::Black, "Resolution:", 30, fontPath);
    labelsConfig.emplace_back(std::move(resolutionLabelConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////

    auto resolutionValueLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsResolutionValueLabel", resolutionTextFieldPosition, graphics::Color::Black, "", 20,
        fontPath);
    labelsConfig.emplace_back(std::move(resolutionValueLabelConfig));

    //////////////////////////////////////////////////////////////////////////////////////////////

    const auto resolutionDecreaseButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsResolutionDecreaseButton",
                          buttonHoverColor);
    };
    const auto resolutionDecreaseButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsResolutionDecreaseButton",
                          buttonColor);
    };
    auto resolutionDecreaseButtonMouseOverActions = components::ui::MouseOverActions{
        resolutionDecreaseButtonOnMouseOver, resolutionDecreaseButtonOnMouseOut};
    auto resolutionDecreaseButtonClickAction = [=] { settingsState->decreaseResolution(); };
    auto resolutionDecreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsResolutionDecreaseButton", resolutionDecreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, "<", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6, -0.3},
        resolutionDecreaseButtonClickAction, resolutionDecreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(resolutionDecreaseButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto resolutionIncreaseButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsResolutionIncreaseButton",
                          buttonHoverColor);
    };
    const auto resolutionIncreaseButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsResolutionIncreaseButton",
                          buttonColor);
    };
    auto resolutionIncreaseButtonMouseOverActions = components::ui::MouseOverActions{
        resolutionIncreaseButtonOnMouseOver, resolutionIncreaseButtonOnMouseOut};
    auto resolutionIncreaseButtonClickAction = [=] { settingsState->increaseResolution(); };
    auto resolutionIncreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsResolutionIncreaseButton", resolutionIncreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, ">", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6, -0.3},
        resolutionIncreaseButtonClickAction, resolutionIncreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(resolutionIncreaseButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    auto vsyncLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsVsyncLabel", vsyncSectionPosition, graphics::Color::Black, "Vsync:", 30, fontPath);
    labelsConfig.emplace_back(std::move(vsyncLabelConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto vsyncCheckBoxOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::CheckBox, "settingsVsyncCheckBox",
                          buttonHoverColor);
    };
    const auto vsyncCheckBoxOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::CheckBox, "settingsVsyncCheckBox", buttonColor);
    };
    auto vsyncCheckBoxMouseOverActions =
        components::ui::MouseOverActions{vsyncCheckBoxOnMouseOver, vsyncCheckBoxOnMouseOut};
    auto vsyncCheckBoxClickAction = [=] { settingsState->switchVsync(); };
    auto vsyncCheckBoxConfig = std::make_unique<components::ui::CheckBoxConfig>(
        "settingsVsyncCheckBox", vsyncButtonPosition, vsyncButtonSize, buttonColor, false, 25, fontPath,
        utils::Vector2f{0.8, 0.0}, vsyncCheckBoxClickAction, vsyncCheckBoxMouseOverActions);
    checkBoxesConfig.emplace_back(std::move(vsyncCheckBoxConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    auto frameLimitLabelConfig =
        std::make_unique<components::ui::LabelConfig>("settingsFrameLimitLabel", frameLimitSectionPosition,
                                                      graphics::Color::Black, "Frame limit:", 30, fontPath);
    labelsConfig.emplace_back(std::move(frameLimitLabelConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    auto frameLimitValueLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "settingsFrameLimitValueLabel", frameLimitTextFieldPosition, graphics::Color::Black, "", 20,
        fontPath);
    labelsConfig.emplace_back(std::move(frameLimitValueLabelConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto frameLimitDecreaseButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsFrameLimitDecreaseButton",
                          buttonHoverColor);
    };
    const auto frameLimitDecreaseButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsFrameLimitDecreaseButton",
                          buttonColor);
    };
    auto frameLimitDecreaseButtonMouseOverActions = components::ui::MouseOverActions{
        frameLimitDecreaseButtonOnMouseOver, frameLimitDecreaseButtonOnMouseOut};
    auto frameLimitDecreaseButtonClickAction = [=] { settingsState->decreaseFrameLimit(); };
    auto frameLimitDecreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFrameLimitDecreaseButton", frameLimitDecreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, "<", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6, -0.3},
        frameLimitDecreaseButtonClickAction, frameLimitDecreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(frameLimitDecreaseButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto frameLimitIncreaseButtonOnMouseOver = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsFrameLimitIncreaseButton",
                          buttonHoverColor);
    };
    const auto frameLimitIncreaseButtonOnMouseOut = [=] {
      settingsState->uiManager->setColor(components::ui::UIComponentType::Button, "settingsFrameLimitIncreaseButton",
                          buttonColor);
    };
    auto frameLimitIncreaseButtonMouseOverActions = components::ui::MouseOverActions{
        frameLimitIncreaseButtonOnMouseOver, frameLimitIncreaseButtonOnMouseOut};
    auto frameLimitIncreaseButtonClickAction = [=] { settingsState->increaseFrameLimit(); };
    auto frameLimitIncreaseButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "settingsFrameLimitIncreaseButton", frameLimitIncreaseButtonPosition, changeResolutionButtonSize,
        buttonColor, ">", graphics::Color::Black, 20, fontPath, utils::Vector2f{0.6, -0.3},
        frameLimitIncreaseButtonClickAction, frameLimitIncreaseButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(frameLimitIncreaseButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}
}