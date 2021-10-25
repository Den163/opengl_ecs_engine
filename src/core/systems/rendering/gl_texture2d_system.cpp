#include "gl_texture2d_system.h"

#include <stb_image.h>

#include "../../components/gl_mesh.h"
#include "../../components/gl_texture.h"

static inline ColorMode getColorMode(int channels);

void GlTexture2dSystem::init(EngineState& engineState, const EngineConfig& engineConfig)
{
    const auto& textureConfigs = engineConfig.textures;
    const auto texturesCount = textureConfigs.size();
    auto& textureStates = engineState.textures;

    textureStates.resize(texturesCount);

    stbi_set_flip_vertically_on_load(true);
    for (size_t i = 0; i < texturesCount; i++)
    {
        const auto& textureConfig = textureConfigs[i];
        const auto& filePath = textureConfig.filePath;
        auto& textureState = textureStates[i];

        int32_t channels;
        uint8_t* imageData = stbi_load(filePath.data(), &textureState.width, &textureState.height, &channels, 0);

        if (imageData == nullptr)
        {
            throw std::runtime_error { "Failed to load texture: " + filePath };
        }

        textureState.colorMode = getColorMode(channels);
        glCreateTextures(GL_TEXTURE_2D, 1, &textureState.handle);
        glTextureStorage2D(textureState.handle, 1, GL_RGB8, textureState.width, textureState.height);
        glTextureSubImage2D(
            textureState.handle, 0,
            0, 0,
            textureState.width, textureState.height,
            GL_RGB, GL_UNSIGNED_BYTE, imageData);

        stbi_image_free(imageData);
    }
}

ColorMode getColorMode(int channels)
{
    switch (channels)
    {
        case 3: return ColorMode::RGB;
        case 4: return ColorMode::RGBA;
        default:
            throw std::runtime_error { "Can't map channels count: `" + std::to_string(channels) + "` to ColorMode" };
    }
}


void GlTexture2dSystem::update(EngineState& engineState)
{
    auto& registry = engineState.guiRegistry;
    for (auto&& [e, glMesh, texture] : registry.view<const GlMesh, const GlTexture>().each())
    {
        glBindVertexArray(glMesh.vaoHandle);
        glBindTexture(GL_TEXTURE_2D, texture.handle);
    }
}