#pragma once

#include <utils/Types.h>

#include <whb/gfx.h>

BOOL WHBGfxLoadGFDShaderGroupMappedMem(WHBGfxShaderGroup *grp, uint32_t idx,
                                       const void *data);
BOOL WHBGfxInitFetchShaderMappedMem(WHBGfxShaderGroup *grp);
BOOL WHBGfxFreeShaderGroupMappedMem(WHBGfxShaderGroup *grp);