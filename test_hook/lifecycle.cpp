#pragma once

#include "lifecycle.h"

#include <iostream>
#include <random>

#include "../src/core/components/gui_mesh.h"
#include "../src/core/primitives/gui/gui_primitive_factory.h"
#include "../src/core/utils/debug_utils.h"
#include "../src/core/utils/engine_state_helper.h"
#include "../src/core/utils/texture_utils.h"
#include "../src/core/utils/font_utils.h"
#include "../src/core/utils/color.h"

static inline void checkMeshCreateByKeyPress(GlobalState& state);
static inline void checkMeshDeleteByKeyPress(GlobalState& state);
static inline void checkPrintCameraInfoByKeyPress(GlobalState& state);
static inline glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max);

void Lifecycle::init(GlobalState& state)
{
    auto& engineState = state.engineState;
    auto& guiRegistry = engineState.guiRegistry;

    auto& tm = GuiPrimitiveFactory::makeEntityTriangle(
        engineState,
        guiRegistry.create(),
        Transform::makeDefault(),
        GuiTriangle::create(
            {glm::vec3 {100, 100, 0.f}, {100, 200, 0.f}, {200, 200, 0.f}},
            Color::red
        ));

    const auto& qe = guiRegistry.create();
    auto& qm = GuiPrimitiveFactory::makeEntityQuad(
        engineState,
        qe,
        Transform::makeDefault().withPosition({400, 400, 0}),
        GuiQuad::create(300, 400, Color::yellow)
    );
    qm.fragmentShaderHandle = EngineStateHelper::standardTextureFragmentShaderProgram(engineState).programHandle;
    GuiPrimitiveFactory::addTexture(engineState, qe, TextureUtils::loadFromFile("assets/images/img.jpg").handle);

    auto fontIndex = engineState.fonts.insert(FontUtils::makeFont("assets/fonts/Roboto-Black.ttf", { 0, 120 }));
    GuiPrimitiveFactory::makeTextQuads(engineState, "Hello world", fontIndex);
}

void Lifecycle::preRenderUpdate(GlobalState& state)
{
}

void Lifecycle::postRenderUpdate(GlobalState& state)
{
    checkPrintCameraInfoByKeyPress(state);
    checkMeshCreateByKeyPress(state);
    checkMeshDeleteByKeyPress(state);
}

void checkMeshCreateByKeyPress(GlobalState& state)
{
    auto& engineState = state.engineState;

    if (!engineState.input.keyboardState.keyHasFlag('C', InputState::KEY_STATE_PRESSED)) return;

    const auto& windowState = engineState.window;
    auto& mesh = GuiPrimitiveFactory::makeEntityMesh(
        engineState,
        engineState.guiRegistry.create(),
        Transform::makeDefault());

    const auto& minPos = glm::vec2 { 0 };
    const auto& maxPos = glm::vec2 { windowState.width, windowState.height };

    mesh.renderMode = RenderMode::Triangles;

    const auto& p0 = randomPos(minPos, maxPos);
    const auto& p1 = randomPos(minPos, maxPos);
    const auto& p2 = randomPos(minPos, maxPos);

    mesh.vertices = {
        {Color::blue, p0, { 0,0 } },
        {Color::blue, p1, { 0.5f, 1 } },
        {Color::blue, p2, { 1, 1 } },
    };
}

void checkMeshDeleteByKeyPress(GlobalState& state)
{
    auto& engineState = state.engineState;

    if (!engineState.input.keyboardState.keyHasFlag('D', InputState::KEY_STATE_PRESSED)) return;

    for (auto&& [e, mesh] : engineState.guiRegistry.view<GuiMesh>().each())
    {
        GuiPrimitiveFactory::deleteMeshEntity(engineState, e);
        return;
    }
}

glm::vec3 randomPos(const glm::vec2& min, const glm::vec2& max)
{
    std::uniform_real_distribution<float> dists[2] = {
        std::uniform_real_distribution { min.x, max.x },
        std::uniform_real_distribution { min.y, max.y },
    };

    glm::vec3 v { 0.0f };

    for (size_t i = 0; i < 2; i++)
    {
        auto randomDevice = std::random_device {};
        auto randomFunctor = std::mt19937 { randomDevice() };
        v[i] = dists[i](randomFunctor);
    }

    return v;
}

void checkPrintCameraInfoByKeyPress(GlobalState& state)
{
    auto& engineState = state.engineState;
    const auto& cameraState = engineState.camera;

    if (engineState.input.keyPressed('P'))
    {
       std::cout << "Camera position        : " << cameraState.position << std::endl
                 << "Camera rotation (euler): " << glm::degrees(glm::eulerAngles(cameraState.rotation)) << std::endl
                 << "Mouse screen position  : " << engineState.input.postRenderMousePosition << std::endl;
    }
}

void Lifecycle::cleanup(GlobalState& state)
{

}