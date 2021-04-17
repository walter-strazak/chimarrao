#pragma once

#include "InputApi.h"
#include "InputKey.h"
#include "Vector.h"

namespace input
{
class INPUT_API Input
{
public:
    virtual ~Input() = default;

    virtual bool isKeyPressed(InputKey) const = 0;
    virtual void setKeyPressed(InputKey) = 0;
    virtual bool isKeyReleased(InputKey) const = 0;
    virtual void setReleasedKeys() = 0;
    virtual void setMousePosition(const utils::Vector2f&) = 0;
    virtual utils::Vector2f getMousePosition() const = 0;
    virtual void clearPressedKeys() = 0;
};
}