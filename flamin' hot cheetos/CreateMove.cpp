#include "CreateMove.h"

CreateMove_t originalCreateMove;

void __stdcall CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
	originalCreateMove( interfaces::client, sequence_number, input_sample_frametime, active );

	CUserCmd* cmd = interfaces::input->getUserCmd( 0, sequence_number );
	if ( !cmd )
		return;

	CInput::CVerifiedUserCmd* verifiedCommands = *( CInput::CVerifiedUserCmd** )( ( DWORD ) interfaces::input + VERIFIEDCMDOFFSET );
	CInput::CVerifiedUserCmd* verified = &verifiedCommands [ sequence_number % MULTIPLAYER_BACKUP ];
	if ( !verified )
		return;

	CBaseEntity* local = interfaces::entitylist->getClientEntity( interfaces::engine->getLocalPlayer( ) );
	if ( !local || local->getLifeState( ) != LIFE_ALIVE )
		return;

	miscellaneous.doBunnyhop( local, cmd );

	CBaseCombatWeapon* weapon = tools.getActiveWeapon( local );
	if ( weapon )
	{
		miscellaneous.doRecoilControl( local, weapon, cmd );
		aimbot.think( local, weapon );
		triggerbot.think( local, weapon, cmd );
	}

	tools.normalizeAngles( cmd->viewangles );
	tools.clampAngles( cmd->viewangles );

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->getChecksum( );
}