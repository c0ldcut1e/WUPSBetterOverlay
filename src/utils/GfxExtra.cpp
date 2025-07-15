#include "GfxExtra.h"

#include <cstring>

#include <utils/Logger.h>

#include <gfd.h>
#include <gx2/mem.h>
#include <gx2/shaders.h>

#include <memory/mappedmemory.h>

GX2PixelShader *WHBGfxLoadGFDPixelShaderMappedMem(uint32_t idx,
                                                  const void *data) {
    uint32_t headerSize;
    uint32_t prgmSize;
    GX2PixelShader *shader = nullptr;
    void *prgm             = nullptr;

    if (idx >= GFDGetPixelShaderCount(data)) {
        DEBUG_FUNCTION_LINE_ERR("%s: idx %u >= %u GFDGetPixelShaderCount(data)",
                                __FUNCTION__, idx,
                                GFDGetPixelShaderCount(data));
        goto error;
    }

    headerSize = GFDGetPixelShaderHeaderSize(idx, data);
    if (!headerSize) {
        DEBUG_FUNCTION_LINE_ERR("%s: headerSize == 0", __FUNCTION__);
        goto error;
    }

    prgmSize = GFDGetPixelShaderProgramSize(idx, data);
    if (!prgmSize) {
        DEBUG_FUNCTION_LINE_ERR("%s: prgmSize == 0", __FUNCTION__);
        goto error;
    }

    shader = (GX2PixelShader *) MEMAllocFromMappedMemoryEx(headerSize, 64);
    if (!shader) {
        DEBUG_FUNCTION_LINE_ERR("%s: MEMAllocFromMappedMemoryEx(%u, 64) failed",
                                __FUNCTION__, headerSize);
        goto error;
    }

    prgm = MEMAllocFromMappedMemoryForGX2Ex(prgmSize,
                                            GX2_SHADER_PROGRAM_ALIGNMENT);
    if (!prgm) {
        DEBUG_FUNCTION_LINE_ERR("%s: MEMAllocFromMappedMemoryForGX2Ex failed",
                                __FUNCTION__);
        goto error;
    }

    if (!GFDGetPixelShader(shader, prgm, idx, data)) {
        DEBUG_FUNCTION_LINE_ERR("%s: GFDGetPixelShader failed", __FUNCTION__);
        GX2RUnlockBufferEx(&shader->gx2rBuffer,
                           GX2R_RESOURCE_DISABLE_CPU_INVALIDATE |
                                   GX2R_RESOURCE_DISABLE_GPU_INVALIDATE);
        goto error;
    }

    shader->program = prgm;
    shader->size    = prgmSize;

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, shader->program,
                  shader->size);

    return shader;

error:
    if (shader) {
        if (shader->program) MEMFreeToMappedMemory(shader->program);
        MEMFreeToMappedMemory(shader);
    }

    return FALSE;
}

void WHBGfxFreePixelShaderMappedMem(GX2PixelShader *shader) {
    if (shader->program) MEMFreeToMappedMemory(shader->program);
    MEMFreeToMappedMemory(shader);
}

GX2VertexShader *WHBGfxLoadGFDVertexShaderMappedMem(uint32_t idx,
                                                    const void *data) {
    uint32_t headerSize;
    uint32_t prgmSize;
    GX2VertexShader *shader = NULL;
    void *prgm              = NULL;

    if (idx >= GFDGetVertexShaderCount(data)) {
        DEBUG_FUNCTION_LINE_ERR(
                "%s: idx %u >= %u GFDGetVertexShaderCount(file)", __FUNCTION__,
                idx, GFDGetVertexShaderCount(data));
        goto error;
    }

    headerSize = GFDGetVertexShaderHeaderSize(idx, data);
    if (!headerSize) {
        DEBUG_FUNCTION_LINE_ERR("%s: headerSize == 0", __FUNCTION__);
        goto error;
    }

    prgmSize = GFDGetVertexShaderProgramSize(idx, data);
    if (!prgmSize) {
        DEBUG_FUNCTION_LINE_ERR("%s: prgmSize == 0", __FUNCTION__);
        goto error;
    }

    shader = (GX2VertexShader *) MEMAllocFromMappedMemoryEx(headerSize, 64);
    if (!shader) {
        DEBUG_FUNCTION_LINE_ERR("%s: MEMAllocFromMappedMemoryEx(%u, 64) failed",
                                __FUNCTION__, headerSize);
        goto error;
    }

    prgm = MEMAllocFromMappedMemoryForGX2Ex(prgmSize,
                                            GX2_SHADER_PROGRAM_ALIGNMENT);
    if (!prgm) {
        DEBUG_FUNCTION_LINE_ERR("%s: MEMAllocFromMappedMemoryForGX2Ex failed",
                                __FUNCTION__);
        goto error;
    }

    if (!GFDGetVertexShader(shader, prgm, idx, data)) {
        DEBUG_FUNCTION_LINE_ERR("%s: GFDGetVertexShader failed", __FUNCTION__);
        GX2RUnlockBufferEx(&shader->gx2rBuffer,
                           GX2R_RESOURCE_DISABLE_CPU_INVALIDATE |
                                   GX2R_RESOURCE_DISABLE_GPU_INVALIDATE);
        goto error;
    }

    shader->program = prgm;
    shader->size    = prgmSize;

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, shader->program,
                  shader->size);

    return shader;

error:
    if (shader) {
        if (shader->program) MEMFreeToMappedMemory(shader->program);
        MEMFreeToMappedMemory(shader);
    }

    return FALSE;
}

void WHBGfxFreeVertexShaderMappedMem(GX2VertexShader *shader) {
    if (shader->program) MEMFreeToMappedMemory(shader->program);
    MEMFreeToMappedMemory(shader);
}

BOOL WHBGfxLoadGFDShaderGroupMappedMem(WHBGfxShaderGroup *grp, uint32_t idx,
                                       const void *data) {
    memset(grp, 0, sizeof(WHBGfxShaderGroup));
    grp->vertexShader = WHBGfxLoadGFDVertexShaderMappedMem(idx, data);
    grp->pixelShader  = WHBGfxLoadGFDPixelShaderMappedMem(idx, data);
    if (!grp->vertexShader || !grp->pixelShader) {
        WHBGfxFreeShaderGroupMappedMem(grp);
        return FALSE;
    }

    return TRUE;
}

BOOL WHBGfxInitFetchShaderMappedMem(WHBGfxShaderGroup *grp) {
    uint32_t size           = GX2CalcFetchShaderSizeEx(grp->numAttributes,
                                                       GX2_FETCH_SHADER_TESSELLATION_NONE,
                                                       GX2_TESSELLATION_MODE_DISCRETE);
    grp->fetchShaderProgram = MEMAllocFromMappedMemoryForGX2Ex(
            size, GX2_SHADER_PROGRAM_ALIGNMENT);

    GX2InitFetchShaderEx(&grp->fetchShader, (uint8_t *) grp->fetchShaderProgram,
                         grp->numAttributes, grp->attributes,
                         GX2_FETCH_SHADER_TESSELLATION_NONE,
                         GX2_TESSELLATION_MODE_DISCRETE);

    GX2Invalidate(GX2_INVALIDATE_MODE_CPU_SHADER, grp->fetchShaderProgram,
                  size);

    return TRUE;
}

BOOL WHBGfxFreeShaderGroupMappedMem(WHBGfxShaderGroup *grp) {
    if (grp->fetchShaderProgram) {
        MEMFreeToMappedMemory(grp->fetchShaderProgram);
        grp->fetchShaderProgram = NULL;
    }

    if (grp->pixelShader) {
        WHBGfxFreePixelShaderMappedMem(grp->pixelShader);
        grp->pixelShader = NULL;
    }

    if (grp->vertexShader) {
        WHBGfxFreeVertexShaderMappedMem(grp->vertexShader);
        grp->vertexShader = NULL;
    }

    return TRUE;
}