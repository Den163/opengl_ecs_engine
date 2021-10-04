#include "render_tick_system.h"

#include <chrono>

void RenderTickSystem::init(RendererState& rendererState)
{
    rendererState.shouldUpdate = false;
    rendererState.updateTimestamp.lastUpdateTime = std::chrono::steady_clock::now();
    rendererState.updateTimestamp.deltaTime = std::chrono::duration<float> { 0 };
}

void RenderTickSystem::update(
    RendererState& rendererState, const RendererConfig& rendererConfig, const UpdateTimeState& loopUpdateTimestamp)
{
    auto& updateTimeStamp = rendererState.updateTimestamp;
    const auto& deltaTimeToUpdate = std::chrono::duration<float> { 1 / rendererConfig.targetFps };
    const auto& now = loopUpdateTimestamp.lastUpdateTime;
    const auto& deltaTime = now - updateTimeStamp.lastUpdateTime;

    auto shouldUpdate = deltaTime >= deltaTimeToUpdate;
    rendererState.shouldUpdate = shouldUpdate;

    if (shouldUpdate)
    {
        updateTimeStamp.lastUpdateTime = now;
        updateTimeStamp.deltaTime = deltaTime;
    }
}