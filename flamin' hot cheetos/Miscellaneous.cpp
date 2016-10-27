#include "Miscellaneous.h"

Misc miscellaneous;

void Misc::doBunnyhop( CBaseEntity* local, CUserCmd* cmd )
{
	if ( !cvar::misc_bunnyhop )
		return;

	if ( ( cmd->buttons & IN_JUMP ) && !( local->getFlags( ) & FL_ONGROUND ) )
		cmd->buttons &= ~IN_JUMP;
}

void Misc::doRecoilControl( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd )
{
	if ( !cvar::misc_rcs )
		return;

	if ( weapon->isOther( ) || weapon->isKnife( ) )
		return;

	static QAngle oldPunchAngles = QAngle( 0.0f, 0.0f, 0.0f );

	QAngle punchAngles = local->getPunchAngles( );
	if ( punchAngles.x == 0.0f || punchAngles.y == 0.0f )
		return;

	if ( punchAngles.Length2D( ) < 0.0f || punchAngles.Length2D( ) > 6.0f )
	{
		oldPunchAngles.Zero( );
		return;
	}

	int shotsFired = local->getShotsFired( ), oldShotsFired = 0;

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

		interfaces::engine->setViewAngles( finalAngles );

		/*static float gameSensitivity = interfaces::convar->findVar( charenc( "sensitivity" ) )->getFloat( );

		float pixels = 0.022f * gameSensitivity * systemSensitivity;

		finalAngles.x /= pixels * -1.0f;
		finalAngles.y /= pixels;

		tools.moveMouse( finalAngles.y, finalAngles.x );*/

		oldShotsFired = shotsFired;
	}

	oldPunchAngles = punchAngles;
}