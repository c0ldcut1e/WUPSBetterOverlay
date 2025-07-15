#pragma once

#include <memory>

#include <overlay/Overlay.h>
#include <utils/CThread.h>

#include <coreinit/memheap.h>
#include <gx2/context.h>

extern GX2ContextState *gCtxState;
extern bool gDrawAllowed;

extern std::shared_ptr<Overlay> gOverlay;
extern CThread *gOverlayUpdateThread;

extern MEMHeapHandle gMEM2;