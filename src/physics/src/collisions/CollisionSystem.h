#pragma once

#include <memory>
#include <vector>

#include "core/ComponentOwner.h"

class CollisionSystem
{
public:
    ~CollisionSystem() = default;

    virtual void add(std::vector<std::shared_ptr<components::core::ComponentOwner>>&) = 0;
    virtual void processRemovals() = 0;
    virtual void update() = 0;
};