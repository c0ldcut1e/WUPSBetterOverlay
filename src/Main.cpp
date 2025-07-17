#include <wups.h>

#include <Globals.h>
#include <overlay/Overlay.h>
#include <utils/CThread.h>
#include <utils/Logger.h>
#include <utils/PluginInfo.h>

#include <coreinit/debug.h>
#include <coreinit/memheap.h>
#include <gx2/enum.h>
#include <whb/proc.h>

#include <memory/mappedmemory.h>

WUPS_PLUGIN_NAME("BetterOverlay");
WUPS_PLUGIN_DESCRIPTION("");
WUPS_PLUGIN_VERSION("v0.1");
WUPS_PLUGIN_AUTHOR("c0ldcut1e");
WUPS_PLUGIN_LICENSE("MIT");

WUPS_USE_WUT_DEVOPTAB();
WUPS_USE_STORAGE("betteroverlay");

INITIALIZE_PLUGIN() {
    initLogging();

    DEBUG_FUNCTION_LINE("Hello from %s o/", getPluginName());

    gMEM2 = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2);

    gCtxState = (GX2ContextState *) MEMAllocFromMappedMemoryForGX2Ex(
            sizeof(GX2ContextState), GX2_CONTEXT_STATE_ALIGNMENT);
    if (!gCtxState) OSFatal("Failed to alloc gContextState");

    gOverlay = std::make_shared<Overlay>();
}

DEINITIALIZE_PLUGIN() {
    DEBUG_FUNCTION_LINE("Goodbye from %s o/", getPluginName());

    deinitLogging();
}

ON_APPLICATION_START() { gDrawAllowed = false; }