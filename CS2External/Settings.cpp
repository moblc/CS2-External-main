#include "Settings.h";
#include <ImGui/imgui.h>;

namespace Settings
{
	bool showMenu = true;
	namespace Util
	{
		bool Bhop = false;
		bool Radar = false;
		bool NoFlash = false;
		bool ThirdPerson = false;
	}
	namespace ESP
	{
		bool ESP = true;
		bool Team = true;
		bool Health = true;
		bool Name = true;
		bool Weapon = true;
		bool Location = true;
		bool Box = true;
		bool Lines = true;
		bool bone = true;
		bool distance = true;
		bool VisionProjection = true;
	}
	
	namespace Misc
	{
		bool AllEntity = false;
		bool Item = false;
		bool Chiken = false;
		bool Glow = false;
		bool Bomb = false;
	}

	namespace Aimbot
	{
		const char* items[15] = {
			"盆骨",
			"脖子",
			"头",
			"左肩",
			"左手肘",
			"左手",
			"右肩",
			"右手肘",
			"右手",
			"左腿",
			"左膝",
			"左脚",
			"右腿",
			"右膝",
			"右脚"
		};
		bool Aimbot = false;
		int Key = 0x10;
		bool RCS = false;
		bool Slient = false;
		bool Smooth = false;
		float SmoothValue = 0.1f;
		bool Team = false;
		bool VisibilityCheck = false;
		bool Trigger = false;
		float TriggerDelay = 0;
		float BetweenShotDelay = 0.01;
		int SelectedItem = 2;
		bool distance_angle_switch = false;
		bool FOVCheck = false;
		float FOV = 180.0f;
	}
}