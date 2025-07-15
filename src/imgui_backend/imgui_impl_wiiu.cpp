#include "imgui_impl_wiiu.h"

#include <cstring>

#include <utils/Logger.h>

struct ImGui_ImplWiiU_Data {
    ImGui_ImplWiiU_Data() { memset((void *) this, 0, sizeof(*this)); }

    bool wasTouched;
};

static ImGui_ImplWiiU_Data *ImGui_ImplWiiU_GetBackendData() {
    return ImGui::GetCurrentContext() ? (ImGui_ImplWiiU_Data *) ImGui::GetIO()
                                                .BackendPlatformUserData
                                      : NULL;
}

bool ImGui_ImplWiiU_Init() {
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == NULL &&
              "Already initialized a platform backend!");

    ImGui_ImplWiiU_Data *data  = IM_NEW(ImGui_ImplWiiU_Data)();
    io.BackendPlatformUserData = data;
    io.BackendPlatformName     = "imgui_impl_wiiu";
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

    return true;
}

void ImGui_ImplWiiU_Shutdown() {
    ImGui_ImplWiiU_Data *data = ImGui_ImplWiiU_GetBackendData();
    IM_ASSERT(data != NULL &&
              "No platform backend to shutdown, or already shutdown?");

    ImGuiIO &io                = ImGui::GetIO();
    io.BackendPlatformName     = NULL;
    io.BackendPlatformUserData = NULL;

    IM_DELETE(data);
}

static bool ImGui_ImplWiiU_WantsInput() {
    ImGuiIO &io = ImGui::GetIO();
    return io.WantCaptureMouse || io.WantCaptureKeyboard || io.WantTextInput ||
           (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) &&
                   io.NavActive &&
                   ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow);
}

bool ImGui_ImplWiiU_ProcessVPADInput(VPADStatus *input) {
    ImGui_ImplWiiU_Data *data = ImGui_ImplWiiU_GetBackendData();
    IM_ASSERT(data != NULL && "Did you call ImGui_ImplWiiU_Init() ?");

    ImGuiIO &io = ImGui::GetIO();

    VPADTouchData touch;
    VPADGetTPCalibratedPoint(VPAD_CHAN_0, &touch, &input->tpNormal);
    if (touch.touched) {
        float width  = (io.DisplaySize.x > 0.0f) ? io.DisplaySize.x : 854.0f;
        float height = (io.DisplaySize.y > 0.0f) ? io.DisplaySize.y : 480.0f;
        float x      = touch.x * (width / 1280.0f);
        float y      = touch.y * (height / 720.0f);
        io.AddMousePosEvent(x, y);
    }

    if (touch.touched != data->wasTouched) {
        io.AddMouseButtonEvent(ImGuiMouseButton_Left, touch.touched);
        data->wasTouched = touch.touched;
    }

    if (ImGui_ImplWiiU_WantsInput()) {
        uint32_t held = input->hold;

        io.AddKeyEvent(ImGuiKey_GamepadDpadLeft, held & VPAD_BUTTON_LEFT);
        io.AddKeyEvent(ImGuiKey_GamepadDpadRight, held & VPAD_BUTTON_RIGHT);
        io.AddKeyEvent(ImGuiKey_GamepadDpadUp, held & VPAD_BUTTON_UP);
        io.AddKeyEvent(ImGuiKey_GamepadDpadDown, held & VPAD_BUTTON_DOWN);

        io.AddKeyEvent(ImGuiKey_GamepadFaceLeft, held & VPAD_BUTTON_X);
        io.AddKeyEvent(ImGuiKey_GamepadFaceRight, held & VPAD_BUTTON_B);
        io.AddKeyEvent(ImGuiKey_GamepadFaceUp, held & VPAD_BUTTON_Y);
        io.AddKeyEvent(ImGuiKey_GamepadFaceDown, held & VPAD_BUTTON_A);
        io.AddKeyEvent(ImGuiKey_GamepadStart, held & VPAD_BUTTON_PLUS);
        io.AddKeyEvent(ImGuiKey_GamepadLStickLeft,
                       held & VPAD_STICK_L_EMULATION_LEFT);
        io.AddKeyEvent(ImGuiKey_GamepadLStickRight,
                       held & VPAD_STICK_L_EMULATION_RIGHT);
        io.AddKeyEvent(ImGuiKey_GamepadLStickUp,
                       held & VPAD_STICK_L_EMULATION_UP);
        io.AddKeyEvent(ImGuiKey_GamepadLStickDown,
                       held & VPAD_STICK_L_EMULATION_DOWN);

        io.AddKeyEvent(ImGuiKey_GamepadL1, held & VPAD_BUTTON_L);
        io.AddKeyEvent(ImGuiKey_GamepadR1, held & VPAD_BUTTON_R);
    }

    return ImGui_ImplWiiU_WantsInput();
}

bool ImGui_ImplWiiU_ProcessWPADInput(WPADStatusProController *input) {
    return true; // TODO
}