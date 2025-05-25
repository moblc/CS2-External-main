#include "Menu.h"
#include "Settings.h"
#include <ImGui/imgui.h>
#include <GLFW/glfw3.h>
#include "Hack.h"
#include <Windows.h>

bool isListeningForKey = false;

int GetPressedKey() {
    for (int key = 0x01; key < 0xFE; key++) {
        if (GetAsyncKeyState(key) & 0x8000) {
            return key;
        }
    }
    return 0;
}

void Menu::ToggleMenu()
{
	Settings::showMenu = !Settings::showMenu;
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = Settings::showMenu;
	io.WantCaptureKeyboard = Settings::showMenu;
	io.MouseDrawCursor = Settings::showMenu;
}

void Menu::Aimbot()
{
    static bool distanceSelected = Settings::Aimbot::distance_angle_switch;
    static bool angleSelected = !Settings::Aimbot::distance_angle_switch;
	if (ImGui::BeginTabItem("自瞄"))
	{
		ImGui::Checkbox("自瞄", &Settings::Aimbot::Aimbot);
        ImGui::Text(isListeningForKey ? "监听热键..." : "自瞄键: %d", Settings::Aimbot::Key);
		if (ImGui::Button("设置自瞄按键"))
            isListeningForKey = true;
        if (isListeningForKey) {
            int pressedKey = GetPressedKey();
            if (pressedKey != 0) {
                Settings::Aimbot::Key = pressedKey;
                isListeningForKey = false;
            }
        }
		ImGui::Checkbox("RCS", &Settings::Aimbot::RCS);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),"待更新");
        if (ImGui::Checkbox("静默", &Settings::Aimbot::Slient))
                Settings::Aimbot::Slient = false;
		ImGui::Checkbox("平滑", &Settings::Aimbot::Smooth);
		ImGui::SliderFloat("平滑程度", &Settings::Aimbot::SmoothValue, 0.0f, 1.0f);
		ImGui::Checkbox("队伍", &Settings::Aimbot::Team);
		ImGui::Checkbox("可视判断", &Settings::Aimbot::VisibilityCheck);
        if (ImGui::Checkbox("准星最近", &angleSelected))
        {
            Settings::Aimbot::distance_angle_switch = !angleSelected;
            distanceSelected = !angleSelected;
        }
        if (ImGui::Checkbox("距离最近", &distanceSelected))
        {
            Settings::Aimbot::distance_angle_switch = distanceSelected;
			angleSelected = !distanceSelected;
        }
		ImGui::Checkbox("自动扳机", &Settings::Aimbot::Trigger);
		ImGui::SliderFloat("扳机延迟", &Settings::Aimbot::TriggerDelay, 0.0f, 1.0f);
		ImGui::Checkbox("开启视距", &Settings::Aimbot::FOVCheck);
		ImGui::SliderFloat("视距", &Settings::Aimbot::FOV, 0.0f, 360.0f);
        ImGui::ListBox("自瞄位置", &Settings::Aimbot::SelectedItem, Settings::Aimbot::items, IM_ARRAYSIZE(Settings::Aimbot::items));
      	ImGui::EndTabItem();
	}
}

void Menu::MISC()
{
	if (ImGui::BeginTabItem("其他"))
	{
        ImGui::Checkbox("所有实体", &Settings::Misc::AllEntity);
        ImGui::Checkbox("物品", &Settings::Misc::Item);
        ImGui::Checkbox("发光", &Settings::Misc::Glow);
		ImGui::Checkbox("炸弹", &Settings::Misc::Bomb);
		ImGui::EndTabItem();
	}

}

void Menu::Utility()
{
    if (ImGui::BeginTabItem("常用"))
    {
        ImGui::Checkbox("兔子跳", &Settings::Util::Bhop);
		ImGui::Checkbox("雷达", &Settings::Util::Radar);
		ImGui::Checkbox("无视闪光", &Settings::Util::NoFlash);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "关闭辅助前关闭第三人称");
        if (ImGui::Checkbox("第三人称", &Settings::Util::ThirdPerson))
        {
            if (!Settings::Util::ThirdPerson)
                Hack::UnpatchThird();
            else
                Hack::PatchThird();
        }
        ImGui::EndTabItem();
    }
}

void Menu::ESP()
{
    if (ImGui::BeginTabItem("绘制"))
    {
        ImGui::Checkbox("透视", &Settings::ESP::ESP);
		ImGui::Checkbox("队伍", &Settings::ESP::Team);
		ImGui::Checkbox("名字", &Settings::ESP::Name);
		ImGui::Checkbox("武器", &Settings::ESP::Weapon);
        ImGui::Checkbox("血量", &Settings::ESP::Health);
        ImGui::Checkbox("方框", &Settings::ESP::Box);
        ImGui::Checkbox("射线", &Settings::ESP::Lines);
		ImGui::Checkbox("骨骼", &Settings::ESP::bone);
		ImGui::Checkbox("距离", &Settings::ESP::Location);
		ImGui::Checkbox("视角朝向", &Settings::ESP::VisionProjection);
        ImGui::EndTabItem();
    }
}

void Menu::MainMenu()
{
    if (Settings::showMenu)
    {
        ImGui::Begin("菜单", &Settings::showMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::SetWindowSize(ImVec2(500, 600));

        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MainTabBar", tab_bar_flags))
        {
            ESP();
            MISC();
            Utility();
            Aimbot();

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}