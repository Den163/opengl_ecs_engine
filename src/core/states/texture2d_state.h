#pragma once

#include <cstdint>
#include "../configs/color_mode.h"

struct Texture2dState
{
    uint8_t* ptr;
    int32_t width;
    int32_t height;
    ColorMode colorMode;
};