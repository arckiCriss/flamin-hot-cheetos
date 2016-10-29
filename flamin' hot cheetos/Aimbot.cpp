#include "Aimbot.h"

Aimbot aimbot;

Aimbot::Aimbot( )
{
	bestTarget = -1;

	viewAngle = QAngle( 0.0f, 0.0f, 0.0f );
	aimAngle = QAngle( 0.0f, 0.0f, 0.0f );
	deltaAngle = QAngle( 0.0f, 0.0f, 0.0f );
	finalAngle = QAngle( 0.0f, 0.0f, 0.0f );

	hitboxPosition = Vector( 0.0f, 0.0f, 0.0f );
}

void Aimbot::think( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd )
{
	if ( !cvar::aimbot_enabled )
		return;

	if ( !weapon->isValid( ) )
		return;

	if ( !( GetAsyncKeyState( cvar::general_key_aimbot ) & 0x8000 ) )
		return;

	bestTarget = getBestTarget( local, weapon, cmd, hitboxPosition );
	if ( bestTarget == -1 )
		return;

	CBaseEntity* entity = interfaces::entitylist->getClientEntity( bestTarget );
	if ( !entity )
		return;

	if ( tools.getDistance( local->getEyePosition( ), hitboxPosition ) > 8192.0f )
		return;

	tools.computeAngle( local->getEyePosition( ), hitboxPosition, aimAngle );
	tools.normalizeAngles( aimAngle );

	aimAngle -= getRandomizedRecoil( local );
	aimAngle += getRandomizedAngles( local );

	deltaAngle = viewAngle - aimAngle;
	tools.normalizeAngles( deltaAngle );

	float randomSmoothing = 1.0f;

	if ( cvar::aimbot_randomize_smoothing >= 1.0f )
		randomSmoothing = tools.random( cvar::aimbot_randomize_smoothing / 10.0f, 1.0f );

	finalAngle = viewAngle - deltaAngle / cvar::aimbot_smoothing * randomSmoothing;
	tools.normalizeAngles( finalAngle );
	tools.clampAngles( finalAngle );

	interfaces::engine->setViewAngles( finalAngle );
}

QAngle Aimbot::getRandomizedRecoil( CBaseEntity* local )
{
	QAngle punchAngles = local->getPunchAngles( );
	QAngle compensatedAngles = punchAngles * 2.0f * ( tools.random( cvar::aimbot_rcs_min, cvar::aimbot_rcs_max ) / 100.0f );
	tools.normalizeAngles( compensatedAngles );

	return ( local->getShotsFired( ) > 1 ? compensatedAngles : QAngle( 0.0f, 0.0f, 0.0f ) );
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

bool Aimbot::getClosestHitbox( CBaseEntity* local, CBaseEntity* entity, Vector& destination )
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

		float fov = tools.getFov( viewAngle, tools.computeAngle( local->getEyePosition( ), temp ) );
		if ( fov < bestFov )
		{
			bestFov = fov;
			bestHitbox = hitbox;
		}
	}

	if ( bestHitbox != -1 )
	{
		if ( !tools.getHitboxPosition( bestHitbox, destination, entity ) )
			return true;
	}

	return false;
}

int Aimbot::getBestTarget( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd, Vector& destination )
{
	int bestTarget = -1;
	float bestFov = cvar::aimbot_fov;

	viewAngle = cmd->viewangles;

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

		float fov = tools.getFov( viewAngle + local->getPunchAngles( ) * 2.0f, tools.computeAngle( local->getEyePosition( ), hitbox ) );
		if ( fov < bestFov && fov < cvar::aimbot_fov )
		{
			if ( tools.isVisible( local->getEyePosition( ), hitbox, entity ) )
			{
				bestFov = fov;
				destination = hitbox;
				bestTarget = i;
			}
		}
	}

	return bestTarget;
}