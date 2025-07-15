#include "Globals.h"

GX2ContextState *gCtxState = nullptr;
bool gDrawAllowed          = false;

std::shared_ptr<Overlay> gOverlay = nullptr;
CThread *gOverlayUpdateThread     = nullptr;

MEMHeapHandle gMEM2 = NULL;