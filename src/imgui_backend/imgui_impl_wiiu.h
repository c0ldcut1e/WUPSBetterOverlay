#pragma once

#include <imgui.h>

#include <padscore/wpad.h>
#include <vpad/input.h>

IMGUI_IMPL_API bool ImGui_ImplWiiU_Init();
IMGUI_IMPL_API void ImGui_ImplWiiU_Shutdown();
IMGUI_IMPL_API bool ImGui_ImplWiiU_ProcessVPADInput(VPADStatus *input);
IMGUI_IMPL_API bool
ImGui_ImplWiiU_ProcessWPADInput(WPADStatusProController *input);