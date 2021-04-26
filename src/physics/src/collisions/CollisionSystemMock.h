#pragma once

#include "gmock/gmock.h"

#include "CollisionSystem.h"

namespace input
{
class CollisionSystemMock : public CollisionSystem
{
public:
    MOCK_METHOD(void, add, (std::vector<std::shared_ptr<components::core::Component>>&), (override));
    MOCK_METHOD(void, processRemovals, (), (override));
    MOCK_METHOD(void, update, (), (override));
};
}