#include "RendererPoolSfml.h"

#include "SFML/Graphics/Texture.hpp"
#include "gtest/gtest.h"

#include "ContextRendererMock.h"
#include "TextureStorageMock.h"

#include "RectangleShape.h"
#include "exceptions/TextureNotAvailable.h"

using namespace ::testing;
using namespace graphics;

namespace
{
const utils::Vector2f size1{20, 30};
const utils::Vector2f size2{100, 100};
const utils::Vector2f position{0, 10};
const Color color{Color::Black};
const TexturePath validTexturePath{"validTexturePath"};
const TexturePath validTexturePath2{"validTexturePath2"};
const TexturePath invalidTexturePath{"invalidTexturePath"};
const auto invalidId = GraphicsIdGenerator::generateId();
}

class RendererPoolSfmlTest_Base : public Test
{
public:
    RendererPoolSfmlTest_Base()
    {
        EXPECT_CALL(*contextRenderer, initialize());
        EXPECT_CALL(*contextRenderer, setView());
    }

    sf::Texture texture;
    std::unique_ptr<ContextRendererMock> contextRendererInit{
        std::make_unique<StrictMock<ContextRendererMock>>()};
    ContextRendererMock* contextRenderer{contextRendererInit.get()};

    std::unique_ptr<TextureStorageMock> textureStorageInit{
        std::make_unique<StrictMock<TextureStorageMock>>()};
    TextureStorageMock* textureStorage{textureStorageInit.get()};
};

class RendererPoolSfmlTest : public RendererPoolSfmlTest_Base
{
public:
    RendererPoolSfml rendererPool{std::move(contextRendererInit), std::move(textureStorageInit)};
};

TEST_F(RendererPoolSfmlTest, acquireShapeWithColor_positionShouldMatch)
{
    const auto shapeId = rendererPool.acquire(size1, position, color);

    const auto actualShapePosition = rendererPool.getPosition(shapeId);

    ASSERT_EQ(*actualShapePosition, position);
}

TEST_F(RendererPoolSfmlTest, acquireShapeWithTexture_textureNotAvailable_shouldThrowTextureNotAvailable)
{
    EXPECT_CALL(*textureStorage, getTexture(invalidTexturePath))
        .WillOnce(Throw(exceptions::TextureNotAvailable{""}));

    ASSERT_THROW(rendererPool.acquire(size1, position, invalidTexturePath), exceptions::TextureNotAvailable);
}

TEST_F(RendererPoolSfmlTest, acquireShapeWithTexture_textureAvailable_positionShouldMatch)
{
    EXPECT_CALL(*textureStorage, getTexture(validTexturePath)).WillOnce(ReturnRef(texture));

    const auto shapeId = rendererPool.acquire(size1, position, validTexturePath);

    const auto actualShapePosition = rendererPool.getPosition(shapeId);
    ASSERT_EQ(actualShapePosition, position);
}

TEST_F(RendererPoolSfmlTest, getPositionWithInvalidId_shouldReturnNone)
{
    const auto actualPosition = rendererPool.getPosition(invalidId);

    ASSERT_EQ(actualPosition, boost::none);
}

TEST_F(RendererPoolSfmlTest, setPositionWithInvalidId_shouldDoNothingAndNoThrow)
{
    ASSERT_NO_THROW(rendererPool.setPosition(invalidId, position));
}

TEST_F(RendererPoolSfmlTest, renderShape)
{
    rendererPool.acquire(size1, position, color);
    EXPECT_CALL(*contextRenderer, clear(sf::Color::White));
    EXPECT_CALL(*contextRenderer, setView());
    EXPECT_CALL(*contextRenderer, draw(_));

    rendererPool.renderAll();
}

TEST_F(RendererPoolSfmlTest, renderShapes)
{
    rendererPool.acquire(size1, position, color);
    rendererPool.acquire(size1, position, color);
    EXPECT_CALL(*contextRenderer, clear(sf::Color::White));
    EXPECT_CALL(*contextRenderer, setView());
    EXPECT_CALL(*contextRenderer, draw(_)).Times(2);

    rendererPool.renderAll();
}

TEST_F(RendererPoolSfmlTest, releasedShapeShouldNotBeRendered)
{
    const auto id = rendererPool.acquire(size1, position, color);
    rendererPool.release(id);
    EXPECT_CALL(*contextRenderer, clear(sf::Color::White));
    EXPECT_CALL(*contextRenderer, setView());

    rendererPool.renderAll();
}

TEST_F(RendererPoolSfmlTest, setTextureWithValidTexturePath_shouldNoThrow)
{
    const auto shapeId = rendererPool.acquire(size1, position, color);
    EXPECT_CALL(*textureStorage, getTexture(validTexturePath2)).WillOnce(ReturnRef(texture));

    ASSERT_NO_THROW(rendererPool.setTexture(shapeId, validTexturePath2));
}

TEST_F(RendererPoolSfmlTest, setTextureWithInvalidTexturePath_shouldThrowTextureNotAvailable)
{
    const auto shapeId = rendererPool.acquire(size1, position, color);
    EXPECT_CALL(*textureStorage, getTexture(invalidTexturePath))
        .WillOnce(Throw(exceptions::TextureNotAvailable{""}));

    ASSERT_THROW(rendererPool.setTexture(shapeId, invalidTexturePath), exceptions::TextureNotAvailable);
}
