#include "Triggerbot.h"

Triggerbot triggerbot;

Triggerbot::Triggerbot( )
{
	viewAngles = QAngle( 0.0f, 0.0f, 0.0f );
}

void Triggerbot::think( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd )
{
	if ( !cvar::misc_triggerbot )
		return;

	if ( !weapon->isValid( ) )
		return;

	if ( !( GetAsyncKeyState( cvar::general_key_triggerbot ) & 0x8000 ) )
		return;

	interfaces::engine->getViewAngles( viewAngles );
	viewAngles += local->getPunchAngles( ) * 2.0f;

	Vector traceStart, traceEnd;
	tools.angleVectors( viewAngles, traceEnd );

	traceStart = local->getEyePosition( );
	traceEnd = traceStart + ( traceEnd * 8192.0f );

	IEngineTrace::trace_t trace;
	IEngineTrace::Ray_t ray;
	IEngineTrace::CTraceFilter filter;
	filter.skip = local;

	ray.Init( traceStart, traceEnd );
	interfaces::enginetrace->traceRay( ray, 0x46004003, &filter, &trace );

	if ( !trace.entity
		|| trace.entity == local
		|| trace.entity->getLifeState( ) != LIFE_ALIVE
		|| trace.entity->isProtected( )
		|| trace.entity->getTeamNum( ) == local->getTeamNum( ) )
		return;

	if ( !( trace.hitgroup < 10 && trace.hitgroup > 0 ) )
		return;

	weapon->getItemDefinitionIndex( ) == WEAPON_REVOLVER ? cmd->buttons |= IN_ATTACK2 : cmd->buttons |= IN_ATTACK;

	if ( weapon->isPistol( ) && tools.isNotAbleToShoot( local, weapon ) )
		weapon->getItemDefinitionIndex( ) == WEAPON_REVOLVER ? cmd->buttons &= ~IN_ATTACK2 : cmd->buttons &= ~IN_ATTACK;
}