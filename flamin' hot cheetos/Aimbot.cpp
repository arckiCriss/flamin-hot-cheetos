#include "Aimbot.h"

// windows mouse sensitivity (usually 1 is default)
#define systemSensitivity 1.0f

Aimbot aimbot;

Aimbot::Aimbot( )
{
	bestTarget = -1;

	viewAngles = QAngle( 0.0f, 0.0f, 0.0f );
	hitboxPosition = Vector( 0.0f, 0.0f, 0.0f );
	finalAngles = QAngle( 0.0f, 0.0f, 0.0f );
}

void Aimbot::think( CBaseEntity* local, CBaseCombatWeapon* weapon )
{
	if ( !cvar::aimbot_enabled )
		return;

	if ( menu.isActive( ) )
		return;

	if ( weapon->IsOther( ) || weapon->IsKnife( ) )
		return;

	if ( !( GetAsyncKeyState( cvar::general_key_aimbot ) & 0x8000 ) )
		return;

	if ( !( GetForegroundWindow( ) == FindWindow( charenc( "Valve001" ), 0 ) ) )
		return;

	bestTarget = getBestTarget( local, weapon, hitboxPosition );
	if ( bestTarget == -1 )
		return;

	CBaseEntity* entity = interfaces::entitylist->GetClientEntity( bestTarget );
	if ( !entity )
		return;

	if ( tools.getDistance( local->GetEyePosition( ), hitboxPosition ) > 8192.0f )
		return;

	hitboxPosition.x += tools.random( -cvar::aimbot_randomize_hitbox, cvar::aimbot_randomize_hitbox );
	hitboxPosition.y += tools.random( -cvar::aimbot_randomize_hitbox, cvar::aimbot_randomize_hitbox );

	tools.computeAngle( local->GetEyePosition( ), hitboxPosition, finalAngles );
	tools.normalizeAngles( finalAngles );

	finalAngles -= getRandomizedRecoil( local );

	finalAngles = viewAngles - finalAngles;
	tools.normalizeAngles( finalAngles );

	float smoothRate = cvar::aimbot_smoothing / 2.0f;

	if ( finalAngles.x > smoothRate )
		finalAngles.x = smoothRate;
	else if ( finalAngles.x < -smoothRate )
		finalAngles.x = -smoothRate;

	if ( finalAngles.y > smoothRate )
		finalAngles.y = smoothRate;
	else if ( finalAngles.y < -smoothRate )
		finalAngles.y = -smoothRate;

	finalAngles += getRandomizedAngles( local );

	static float gameSensitivity = interfaces::convar->FindVar( charenc( "sensitivity" ) )->GetFloat( );

	float pixels = 0.022f * gameSensitivity * systemSensitivity;

	finalAngles.x /= pixels * -1.0f;
	finalAngles.y /= pixels;

	tools.moveMouse( finalAngles.y, finalAngles.x );
}

Vector Aimbot::getRandomizedRecoil( CBaseEntity* local )
{
	QAngle punchAngles = local->GetPunchAngles( ) * tools.random( cvar::aimbot_rcs_min, cvar::aimbot_rcs_max );
	return ( local->GetShotsFired( ) > 1 ? punchAngles : Vector( 0.0f, 0.0f, 0.0f ) );
}

float Aimbot::getRandomizedAngles( CBaseEntity* local )
{
	float randomizedValue = 0.0f;

	float randomRate = tools.random( -cvar::aimbot_randomize_angle, cvar::aimbot_randomize_angle );
	float randomDeviation = tools.random( -cvar::aimbot_randomize_angle, cvar::aimbot_randomize_angle );

	switch ( rand( ) % 2 )
	{
	case 0:
		randomizedValue = ( randomRate * cos( randomDeviation ) );
		break;
	case 1:
		randomizedValue = ( randomRate * sin( randomDeviation ) );
		break;
	}

	return ( local->GetShotsFired( ) > 1 ? randomizedValue : 0.0f );
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
	hitboxes.emplace_back( HITBOX_PELVIS );

	for ( auto hitbox : hitboxes )
	{
		Vector temp;
		if ( !tools.getHitboxPosition( hitbox, temp, entity ) )
			continue;

		float fov = tools.getFov( viewAngles, tools.computeAngle( local->GetEyePosition( ), temp ) );
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

	interfaces::engine->GetViewAngles( viewAngles );

	for ( int i = 1; i <= interfaces::globalvars->maxclients; i++ )
	{
		if ( i == local->GetIndex( ) )
			continue;

		CBaseEntity* entity = interfaces::entitylist->GetClientEntity( i );
		if ( !entity
			|| entity->IsDormant( )
			|| entity->GetLifeState( ) != LIFE_ALIVE
			|| entity->IsProtected( )
			|| entity->GetClientClass( )->GetClassID( ) != CCSPlayer
			|| entity->GetTeamNum( ) == local->GetTeamNum( )
			|| !( entity->GetFlags( ) & FL_ONGROUND ) )
			continue;

		Vector hitbox;
		if ( getClosestHitbox( local, entity, hitbox ) )
			continue;

		hitbox = tools.getPredictedPosition( hitbox, entity->GetVelocity( ) );

		float fov = tools.getFov( viewAngles + local->GetPunchAngles( ) * 2.0f, tools.computeAngle( local->GetEyePosition( ), hitbox ) );
		if ( fov < bestFov && fov < cvar::aimbot_fov )
		{
			if ( tools.isVisible( local->GetEyePosition( ), hitbox, entity ) )
			{
				bestFov = fov;
				dest = hitbox;
				bestTarget = i;
			}
		}
	}

	return bestTarget;
}