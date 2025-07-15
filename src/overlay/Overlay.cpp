#include "Overlay.h"

#include <utils/Logger.h>

#include <imgui.h>
#include <imgui_backend/imgui_impl_gx2.h>
#include <imgui_backend/imgui_impl_wiiu.h>

#include <gx2/state.h>

#include <memory/mappedmemory.h>

Overlay::Overlay() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplWiiU_Init();
    ImGui_ImplGX2_Init();
}

Overlay::~Overlay() {
    ImGui_ImplGX2_Shutdown();
    ImGui_ImplWiiU_Shutdown();
}

void Overlay::update() {}

void Overlay::draw(uint32_t width, uint32_t height) {
    constexpr float LOGICAL_WIDTH  = 854.0f;
    constexpr float LOGICAL_HEIGHT = 480.0f;

    ImGuiIO &io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(LOGICAL_WIDTH, LOGICAL_HEIGHT);
    io.DisplayFramebufferScale =
            ImVec2(width / LOGICAL_WIDTH, height / LOGICAL_HEIGHT);

    ImGui_ImplGX2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();

    ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());

    GX2Flush();
}