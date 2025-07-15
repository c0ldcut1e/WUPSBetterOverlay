#pragma once

#include <vector>

#include <utils/CThread.h>
#include <utils/Types.h>

#include <gx2/enum.h>
#include <whb/gfx.h>

class Overlay {
public:
    Overlay();
    ~Overlay();

    void update();
    void draw(uint32_t width, uint32_t height);

    bool setupKeyboard = false;

private:
    WHBGfxShaderGroup shader;

    void *vtxBuffer = nullptr;
    void *colBuffer = nullptr;
    void *idxBuffer = nullptr;

    std::vector<uint16_t> idxs;
};