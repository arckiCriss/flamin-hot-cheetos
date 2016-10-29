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

	if ( !weapon->isValid( ) )
		return;

	static QAngle oldPunchAngles = QAngle( 0.0f, 0.0f, 0.0f );

	QAngle punchAngles = local->getPunchAngles( );
	if ( punchAngles.x == 0.0f || punchAngles.y == 0.0f )
		return;

	if ( punchAngles.Length2D( ) < 0.0f || punchAngles.Length2D( ) > 7.0f )
	{
		oldPunchAngles.Zero( );
		return;
	}

	int shotsFired = local->getShotsFired( ), oldShotsFired = 0;

	if ( cmd->buttons & IN_ATTACK && shotsFired > 1 && shotsFired > oldShotsFired )
	{
		QAngle compensatedAngles = punchAngles - oldPunchAngles;
		compensatedAngles *= 2.0f * ( tools.random( cvar::misc_rcs_min, cvar::misc_rcs_max ) / 100.0f );
		tools.normalizeAngles( compensatedAngles );

		QAngle viewAngles = cmd->viewangles;
		tools.normalizeAngles( viewAngles );

		QAngle finalAngles = viewAngles - compensatedAngles;
		tools.normalizeAngles( finalAngles );
		tools.clampAngles( finalAngles );

		interfaces::engine->setViewAngles( finalAngles );

		oldShotsFired = shotsFired;
	}

	oldPunchAngles = punchAngles;
}