#include "Control Variables.h"

namespace cvar
{
	int general_key_aimbot = 1;
	int general_key_triggerbot = 18;

	bool aimbot_enabled = true;
	float aimbot_fov = 10.0f;
	float aimbot_smoothing = 25.0f;
	float aimbot_rcs_min = 1.8f;
	float aimbot_rcs_max = 2.0f;
	float aimbot_randomize_hitbox = 0.0f;
	float aimbot_randomize_angle = 0.0f;

	bool esp_enabled = true;
	bool esp_draw_box = true;
	bool esp_draw_name = true;
	bool esp_draw_weapon = true;
	bool esp_draw_callout = false;
	bool esp_draw_health = true;
	bool esp_draw_health_text = false;
	bool esp_draw_armor = false;
	bool esp_draw_armor_text = false;
	bool esp_draw_world = true;
	bool esp_draw_glow = true;

	bool misc_bunnyhop = true;
	bool misc_triggerbot = true;
	bool misc_skinchanger = true;
	bool misc_knifechanger = true;
	int misc_knifechanger_model = 0;
	int misc_overridefov = 22;
	bool misc_rcs = false;
	float misc_rcs_min = 1.8f;
	float misc_rcs_max = 2.0f;
	bool misc_scoreboard = true;
}