#include "Overlay.h"

#include <utils/Logger.h>
#include <utils/Utils.h>

#include <imgui.h>
#include <imgui_backend/imgui_impl_gx2.h>
#include <imgui_backend/imgui_impl_wiiu.h>

#include <gx2/state.h>

#include <memory/mappedmemory.h>

Overlay::Overlay() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = FS_SD_CARD_PATH "BetterOverlay.ini";

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
    float32_t drcWidth  = (float32_t) DRC_WIDTH;
    float32_t drcHeight = (float32_t) DRC_HEIGHT;

    ImGuiIO &io                = ImGui::GetIO();
    io.DisplaySize             = ImVec2(drcWidth, drcHeight);
    io.DisplayFramebufferScale = ImVec2(width / drcWidth, height / drcHeight);

    ImGui_ImplGX2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplGX2_RenderDrawData(ImGui::GetDrawData());
}