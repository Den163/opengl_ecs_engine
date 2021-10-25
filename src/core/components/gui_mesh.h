#pragma once

#include <cstdint>
#include <glm/mat4x4.hpp>
#include <vector>

#include "../primitives/gui/gui_vertex.h"
#include "../configs/render_mode.h"
#include "../utils/engine_invariants.h"

struct GuiMesh
{
    RenderMode renderMode;
    uint32_t shaderPipelineHandle;
    uint32_t vertexShaderHandle;
    uint32_t fragmentShaderHandle;

    std::vector<GuiVertex> vertices;
};