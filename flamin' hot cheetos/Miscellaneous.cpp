#include "Miscellaneous.h"

// windows mouse sensitivity (usually 1 is default)
#define systemSensitivity 1.0f

Misc miscellaneous;

void Misc::doBunnyhop( CBaseEntity* local, CUserCmd* cmd )
{
	if ( !cvar::misc_bunnyhop )
		return;

	if ( ( cmd->buttons & IN_JUMP ) && !( local->GetFlags( ) & FL_ONGROUND ) )
		cmd->buttons &= ~IN_JUMP;
}

void Misc::doRecoilControl( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd )
{
	if ( !cvar::misc_recoilcontrol || cvar::misc_recoilcontrol_scale <= 0.0f )
		return;

	if ( weapon->IsOther( ) || weapon->IsKnife( ) )
		return;

	static QAngle oldPunchAngles = QAngle( 0.0f, 0.0f, 0.0f );

	QAngle punchAngles = local->GetPunchAngles( );
	if ( punchAngles.x == 0.0f || punchAngles.y == 0.0f )
		return;

	if ( punchAngles.Length2D( ) < 0.0f || punchAngles.Length2D( ) > 6.0f )
	{
		oldPunchAngles.Zero( );
		return;
	}

	int shotsFired = local->GetShotsFired( ), oldShotsFired = 0;

	if ( cmd->buttons & IN_ATTACK && shotsFired > 1 && shotsFired > oldShotsFired )
	{
		QAngle compensatedAngles = punchAngles - oldPunchAngles;
		compensatedAngles *= cvar::misc_recoilcontrol_scale;

		QAngle deltaAngles = compensatedAngles;
		tools.normalizeAngles( deltaAngles );
		tools.clampAngles( deltaAngles );

		static float gameSensitivity = interfaces::convar->FindVar( charenc( "sensitivity" ) )->GetFloat( );

		float pixels = 0.022f * gameSensitivity * systemSensitivity;

		deltaAngles.x /= pixels * -1.0f;
		deltaAngles.y /= pixels;

		tools.moveMouse( deltaAngles.y, deltaAngles.x );

		oldShotsFired = shotsFired;
	}

	oldPunchAngles = punchAngles;
}