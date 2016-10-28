#include "Aimbot.h"

Aimbot aimbot;

Aimbot::Aimbot( )
{
	bestTarget = -1;

	viewAngles = QAngle( 0.0f, 0.0f, 0.0f );
	aimAngles = QAngle( 0.0f, 0.0f, 0.0f );
	finalAngles = QAngle( 0.0f, 0.0f, 0.0f );

	hitboxPosition = Vector( 0.0f, 0.0f, 0.0f );
}

void Aimbot::think( CBaseEntity* local, CBaseCombatWeapon* weapon )
{
	if ( !cvar::aimbot_enabled )
		return;

	if ( menu.isActive( ) )
		return;

	if ( weapon->isOther( ) || weapon->isKnife( ) )
		return;

	if ( !( GetAsyncKeyState( cvar::general_key_aimbot ) & 0x8000 ) )
		return;

	/*if ( !( GetForegroundWindow( ) == FindWindow( charenc( "Valve001" ), 0 ) ) )
		return;*/

	bestTarget = getBestTarget( local, weapon, hitboxPosition );
	if ( bestTarget == -1 )
		return;

	CBaseEntity* entity = interfaces::entitylist->getClientEntity( bestTarget );
	if ( !entity )
		return;

	if ( tools.getDistance( local->getEyePosition( ), hitboxPosition ) > 8192.0f )
		return;

	hitboxPosition.x += tools.random( -cvar::aimbot_randomize_hitbox, cvar::aimbot_randomize_hitbox );
	hitboxPosition.y += tools.random( -cvar::aimbot_randomize_hitbox, cvar::aimbot_randomize_hitbox );

	tools.computeAngle( local->getEyePosition( ), hitboxPosition, aimAngles );
	tools.normalizeAngles( aimAngles );

	aimAngles -= getRandomizedRecoil( local );
	aimAngles += getRandomizedAngles( local );

	finalAngles = viewAngles - aimAngles;
	tools.normalizeAngles( finalAngles );

	finalAngles = viewAngles - finalAngles / cvar::aimbot_smoothing;
	tools.normalizeAngles( finalAngles );
	tools.clampAngles( finalAngles );

	interfaces::engine->setViewAngles( finalAngles );

	/*if ( finalAngles.x > cvar::aimbot_smoothing )
		finalAngles.x = cvar::aimbot_smoothing;
	else if ( finalAngles.x < -cvar::aimbot_smoothing )
		finalAngles.x = -cvar::aimbot_smoothing;

	if ( finalAngles.y > cvar::aimbot_smoothing )
		finalAngles.y = cvar::aimbot_smoothing;
	else if ( finalAngles.y < -cvar::aimbot_smoothing )
		finalAngles.y = -cvar::aimbot_smoothing;

	static float gameSensitivity = interfaces::convar->findVar( charenc( "sensitivity" ) )->getFloat( );

	float pixels = 0.022f * gameSensitivity * systemSensitivity;

	finalAngles.x /= pixels * -1.0f;
	finalAngles.y /= pixels;

	tools.moveMouse( finalAngles.y, finalAngles.x );*/
}

QAngle Aimbot::getRandomizedRecoil( CBaseEntity* local )
{
	QAngle punchAngles = local->getPunchAngles( ) * tools.random( cvar::aimbot_rcs_min, cvar::aimbot_rcs_max );
	tools.normalizeAngles( punchAngles );

	return ( local->getShotsFired( ) > 1 ? punchAngles : QAngle( 0.0f, 0.0f, 0.0f ) );
}

QAngle Aimbot::getRandomizedAngles( CBaseEntity* local )
{
	Vector randomizedValue = Vector( 0.0f, 0.0f, 0.0f );

	float randomRate = tools.random( -cvar::aimbot_randomize_angle, cvar::aimbot_randomize_angle );
	float randomDeviation = tools.random( -cvar::aimbot_randomize_angle, cvar::aimbot_randomize_angle );

	switch ( rand( ) % 2 )
	{
	case 0:
		randomizedValue.x = ( randomRate * cos( randomDeviation ) );
		randomizedValue.y = ( randomRate * cos( randomDeviation ) );
		randomizedValue.z = ( randomRate * cos( randomDeviation ) );
		tools.normalizeAngles( randomizedValue );
		break;
	case 1:
		randomizedValue.x = ( randomRate * sin( randomDeviation ) );
		randomizedValue.y = ( randomRate * sin( randomDeviation ) );
		randomizedValue.z = ( randomRate * sin( randomDeviation ) );
		tools.normalizeAngles( randomizedValue );
		break;
	}

	return ( local->getShotsFired( ) > 1 ? randomizedValue : QAngle( 0.0f, 0.0f, 0.0f ) );
}

bool Aimbot::getClosestHitbox( CBaseEntity* local, CBaseEntity* entity, Vector& dest )
{
	int bestHitbox = -1;
	float bestFov = cvar::aimbot_fov;

	std::vector<int> hitboxes;
	hitboxes.emplace_back( HITBOX_HEAD );
	hitboxes.emplace_back( HITBOX_NECK );
	hitboxes.emplace_back( HITBOX_LOWER_NECK );
	hitboxes.emplace_back( HITBOX_UPPER_CHEST );
	hitboxes.emplace_back( HITBOX_CHEST );
	hitboxes.emplace_back( HITBOX_BODY );
	// hitboxes.emplace_back( HITBOX_PELVIS );

	for ( auto hitbox : hitboxes )
	{
		Vector temp;
		if ( !tools.getHitboxPosition( hitbox, temp, entity ) )
			continue;

		float fov = tools.getFov( viewAngles, tools.computeAngle( local->getEyePosition( ), temp ) );
		if ( fov < bestFov )
		{
			bestFov = fov;
			bestHitbox = hitbox;
		}
	}

	if ( bestHitbox != -1 )
	{
		if ( !tools.getHitboxPosition( bestHitbox, dest, entity ) )
			return true;
	}

	return false;
}

int Aimbot::getBestTarget( CBaseEntity* local, CBaseCombatWeapon* weapon, Vector& dest )
{
	int bestTarget = -1;
	float bestFov = cvar::aimbot_fov;

	interfaces::engine->getViewAngles( viewAngles );

	for ( int i = 1; i <= interfaces::globalvars->maxclients; i++ )
	{
		if ( i == local->getIndex( ) )
			continue;

		CBaseEntity* entity = interfaces::entitylist->getClientEntity( i );
		if ( !entity
			|| entity->isDormant( )
			|| entity->getLifeState( ) != LIFE_ALIVE
			|| entity->isProtected( )
			|| entity->getClientClass( )->getClassID( ) != CCSPlayer
			|| entity->getTeamNum( ) == local->getTeamNum( )
			|| !( entity->getFlags( ) & FL_ONGROUND ) )
			continue;

		Vector hitbox;
		if ( getClosestHitbox( local, entity, hitbox ) )
			continue;

		hitbox = tools.getPredictedPosition( hitbox, entity->getVelocity( ) );

		float fov = tools.getFov( viewAngles + local->getPunchAngles( ) * 2.0f, tools.computeAngle( local->getEyePosition( ), hitbox ) );
		if ( fov < bestFov && fov < cvar::aimbot_fov )
		{
			if ( tools.isVisible( local->getEyePosition( ), hitbox, entity ) )
			{
				bestFov = fov;
				dest = hitbox;
				bestTarget = i;
			}
		}
	}

	return bestTarget;
}