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
	if ( !cvar::misc_rcs )
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
		compensatedAngles *= tools.random( cvar::misc_rcs_min, cvar::misc_rcs_max );
		tools.normalizeAngles( compensatedAngles );

		QAngle viewAngles = cmd->viewangles;
		tools.normalizeAngles( viewAngles );

		QAngle finalAngles = viewAngles - compensatedAngles;
		tools.normalizeAngles( finalAngles );
		tools.clampAngles( finalAngles );

		interfaces::engine->SetViewAngles( finalAngles );

		/*static float gameSensitivity = interfaces::convar->FindVar( charenc( "sensitivity" ) )->GetFloat( );

		float pixels = 0.022f * gameSensitivity * systemSensitivity;

		finalAngles.x /= pixels * -1.0f;
		finalAngles.y /= pixels;

		tools.moveMouse( finalAngles.y, finalAngles.x );*/

		oldShotsFired = shotsFired;
	}

	oldPunchAngles = punchAngles;
}