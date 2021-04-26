#include "Component.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components::core;

class ComponentTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    const std::string ownerName{"componentTest"};
    ComponentOwner componentOwner{initialPosition, ownerName};
    Component component{&componentOwner};
};

TEST_F(ComponentTest, initialComponent_shouldBeEnabled)
{
    ASSERT_TRUE(component.isEnabled());
}

TEST_F(ComponentTest, disableComponent)
{
    component.disable();

    ASSERT_FALSE(component.isEnabled());
}

TEST_F(ComponentTest, enableComponent)
{
    component.disable();

    component.enable();

    ASSERT_TRUE(component.isEnabled());
}

TEST_F(ComponentTest, shouldReturnOwnerName)
{
    const auto actualOwnerName = component.getOwnerName();

    ASSERT_EQ(actualOwnerName, ownerName);
}

