#include "HeadsUpDisplay.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "core/ItemEffectMock.h"

#include "CommonUIConfigElements.h"
#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "GraphicsIdGenerator.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "core/HealthComponent.h"
#include "core/ItemCollectorComponent.h"

using namespace components::core;
using namespace game;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto changedSize = utils::Vector2f{10.f, 5.f};
const auto dummyPosition = utils::Vector2f{0.f, 5.f};
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";
}

class HeadsUpDisplayTest : public Test
{
public:
    HeadsUpDisplayTest()
    {
        player->addComponent<HealthComponent>(initialHealthPoints);
        const auto velocity = player->addComponent<VelocityComponent>();
        velocity->setVelocity(5, 0);
        player->addComponent<DirectionComponent>();
        player->addComponent<AnimationComponent>(animator);
        boxColliderComponent = player->addComponent<BoxColliderComponent>(size);
        itemCollector = player->addComponent<ItemCollectorComponent>(quadtree, rayCast, capacity);

        EXPECT_CALL(*rendererPool,
                    acquire(dummySize, dummyPosition, imagePath, graphics::VisibilityLayer::First, _))
            .WillOnce(Return(itemId));

        boxColliderComponent1 = itemOwner1.addComponent<BoxColliderComponent>(size);
        itemOwner1.addComponent<CollectableItemComponent>(itemName1, itemType, itemEffect);
        itemOwner1.addGraphicsComponent(rendererPool, dummySize, dummyPosition, imagePath,
                                        graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);
        itemOwner1.loadDependentComponents();
    }

    void expectCreateGraphicsComponents()
    {
        EXPECT_CALL(*rendererPool, acquire(healthPointsBarSize, healthPointsBarPosition, graphics::Color::Red,
                                           graphics::VisibilityLayer::First, _))
            .WillOnce(Return(graphicsId1));
        EXPECT_CALL(*rendererPool, acquire(healthPointsBarSize, healthPointsBarPosition,
                                           graphics::Color::Transparent, graphics::VisibilityLayer::First, _))
            .WillOnce(Return(graphicsId3));
        EXPECT_CALL(*rendererPool, acquire(slotSize, _, graphics::Color(152, 152, 152, 152),
                                           graphics::VisibilityLayer::First, _))
            .Times(8)
            .WillRepeatedly(Return(graphics::GraphicsIdGenerator::generateId()));
        EXPECT_CALL(*rendererPool,
                    acquire(slotSize, _, graphics::Color::Transparent, graphics::VisibilityLayer::First, _))
            .Times(8)
            .WillRepeatedly(Return(graphics::GraphicsIdGenerator::generateId()));

        EXPECT_CALL(*rendererPool, acquireText(healthPointsLabelPosition, "HP", fontPath, characterSize,
                                               graphics::VisibilityLayer::First, graphics::Color::Red, _))
            .WillOnce(Return(graphicsId2));
        EXPECT_CALL(*rendererPool, acquireText(slotsLabelPosition, "ITEMS", fontPath, characterSize,
                                               graphics::VisibilityLayer::First, graphics::Color::Black, _))
            .WillOnce(Return(graphicsId4));
    }

    void expectReleaseGraphicsIds()
    {
        EXPECT_CALL(*rendererPool, release(_)).Times(21);
    }

    const std::string itemName1{"item1"};
    const ItemType itemType{ItemType::Apple};
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    const unsigned capacity{8};
    const utils::Vector2f size{4, 4};
    const unsigned int initialHealthPoints{100};
    const unsigned characterSize{20};
    const utils::Vector2f healthPointsLabelPosition{0.5, 0.5};
    const utils::Vector2f slotsLabelPosition{0.5, 3};
    const utils::Vector2f healthPointsBarPosition{4, 1.2};
    const utils::Vector2f healthPointsBarSize{7, 1.3};
    const utils::Vector2f slotSize{2, 2};
    const utils::Vector2f healthPointsBarSizeAfterChange{3.5f, 1.3};
    const utils::Vector2f position{0.0, 0.0};
    const utils::Vector2f position1{2.0, 0.0};
    const graphics::GraphicsId graphicsId1 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId2 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId3 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId4 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId itemId = graphics::GraphicsIdGenerator::generateId();
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1;
    ComponentOwner itemOwner1{position1, "ItemCollectorComponentTest2"};
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    std::shared_ptr<physics::DefaultQuadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::DefaultRayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
    std::shared_ptr<ComponentOwner> player{std::make_shared<ComponentOwner>(position, "headsUpDisplayTest")};
    std::shared_ptr<components::core::ItemCollectorComponent> itemCollector;
};

TEST_F(HeadsUpDisplayTest, healthNotChanged_barShouldNotBeUpdated)
{
    expectCreateGraphicsComponents();
    player->addComponent<HealthComponent>(100);
    player->loadDependentComponents();
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    EXPECT_CALL(*rendererPool, setOutline(_, 0.1, graphics::Color::Black)).Times(8);
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));

    hud.update(deltaTime, input);

    expectReleaseGraphicsIds();
}

TEST_F(HeadsUpDisplayTest, healthChanged_barShouldBeUpdated)
{
    expectCreateGraphicsComponents();
    auto health = player->addComponent<HealthComponent>(100);
    player->loadDependentComponents();
    health->loseHealthPoints(50);
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    EXPECT_CALL(*rendererPool, setOutline(_, 0.1, graphics::Color::Black)).Times(8);
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));
    EXPECT_CALL(*rendererPool, setSize(graphicsId1, healthPointsBarSizeAfterChange));

    hud.update(deltaTime, input);

    expectReleaseGraphicsIds();
}

TEST_F(HeadsUpDisplayTest, collectedItemsChanged_shouldLoadItemTexturesOnUpdate)
{
    expectCreateGraphicsComponents();
    player->loadDependentComponents();
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    EXPECT_CALL(*rendererPool, setOutline(_, 0.1, graphics::Color::Black)).Times(8);
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::First));
    EXPECT_CALL(*rendererPool, setTexture(_, graphics::TextureRect{imagePath}, utils::Vector2f{1, 1}));
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent1);
    itemCollector->collectNearestItem();

    hud.update(deltaTime, input);

    expectReleaseGraphicsIds();
}

TEST_F(HeadsUpDisplayTest, givenNoItems_shouldNotLoadAnyTexturesOnUpdate)
{
    expectCreateGraphicsComponents();
    player->loadDependentComponents();
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    EXPECT_CALL(*rendererPool, setOutline(_, 0.1, graphics::Color::Black)).Times(8);
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));

    hud.update(deltaTime, input);

    expectReleaseGraphicsIds();
}

TEST_F(HeadsUpDisplayTest, collectedItemsNotChanged_shouldNotLoadItemTexturesOnSecondUpdate)
{
    expectCreateGraphicsComponents();
    player->loadDependentComponents();
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    EXPECT_CALL(*rendererPool, setOutline(_, 0.1, graphics::Color::Black)).Times(8);
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillRepeatedly(Return(healthPointsBarSize));
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::First));
    EXPECT_CALL(*rendererPool, setTexture(_, graphics::TextureRect{imagePath}, utils::Vector2f{1, 1}));
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(8);
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent1);
    itemCollector->collectNearestItem();
    hud.update(deltaTime, input);

    hud.update(deltaTime, input);

    expectReleaseGraphicsIds();
}
