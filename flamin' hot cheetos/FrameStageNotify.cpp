#include "FrameStageNotify.h"

FrameStageNotify_t originalFrameStageNotify;

void __stdcall FrameStageNotify( ClientFrameStage_t curstage )
{
	originalFrameStageNotify( interfaces::client, curstage );

	if ( curstage == FRAME_NET_UPDATE_POSTDATAUPDATE_START )
	{
		if ( cvar::misc_skinchanger )
		{
			player_info_t info;

			CBaseEntity* local = interfaces::entitylist->GetClientEntity( interfaces::engine->GetLocalPlayer( ) );
			if ( !local
				|| local->GetLifeState( ) != LIFE_ALIVE
				|| !interfaces::engine->GetPlayerInfo( local->GetIndex( ), &info ) )
				return;

			for ( int i = 1; i <= interfaces::entitylist->GetHighestEntityIndex( ); i++ )
			{
				if ( i == local->GetIndex( ) )
					continue;

				CBaseEntity* entity = interfaces::entitylist->GetClientEntity( i );
				if ( !entity )
					continue;

				std::string modelName = interfaces::modelinfo->GetModelName( entity->GetModel( ) );
				if ( modelName.find( charenc( "models/weapons" ) ) == std::string::npos )
					continue;

				CBaseEntity* owner = interfaces::entitylist->GetClientEntityFromHandle( entity->GetOwner( ) );
				if ( !owner
					|| owner != local )
					continue;

				CBaseCombatWeapon* weapon = ( CBaseCombatWeapon* ) entity;
				if ( !weapon )
					return;

				if ( !weapon->IsKnife( ) )
				{
					weapon->setPattern( info, config.skincfg [ weapon->GetItemDefinitionIndex( ) ].skin, config.skincfg [ weapon->GetItemDefinitionIndex( ) ].wear, config.skincfg [ weapon->GetItemDefinitionIndex( ) ].seed, config.skincfg [ weapon->GetItemDefinitionIndex( ) ].stattrak, config.skincfg [ weapon->GetItemDefinitionIndex( ) ].name );
				}
				else
				{
					if ( cvar::misc_knifechanger )
					{
						if ( cvar::misc_knifechanger_model == 0 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_BAYONET ].skin, config.skincfg [ WEAPON_KNIFE_BAYONET ].wear, config.skincfg [ WEAPON_KNIFE_BAYONET ].seed, config.skincfg [ WEAPON_KNIFE_BAYONET ].stattrak, config.skincfg [ WEAPON_KNIFE_BAYONET ].name );
						else if ( cvar::misc_knifechanger_model == 1 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_FLIP ].skin, config.skincfg [ WEAPON_KNIFE_FLIP ].wear, config.skincfg [ WEAPON_KNIFE_FLIP ].seed, config.skincfg [ WEAPON_KNIFE_FLIP ].stattrak, config.skincfg [ WEAPON_KNIFE_FLIP ].name );
						else if ( cvar::misc_knifechanger_model == 2 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_GUT ].skin, config.skincfg [ WEAPON_KNIFE_GUT ].wear, config.skincfg [ WEAPON_KNIFE_GUT ].seed, config.skincfg [ WEAPON_KNIFE_GUT ].stattrak, config.skincfg [ WEAPON_KNIFE_GUT ].name );
						else if ( cvar::misc_knifechanger_model == 3 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_KARAMBIT ].skin, config.skincfg [ WEAPON_KNIFE_KARAMBIT ].wear, config.skincfg [ WEAPON_KNIFE_KARAMBIT ].seed, config.skincfg [ WEAPON_KNIFE_KARAMBIT ].stattrak, config.skincfg [ WEAPON_KNIFE_KARAMBIT ].name );
						else if ( cvar::misc_knifechanger_model == 4 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_M9BAYONET ].skin, config.skincfg [ WEAPON_KNIFE_M9BAYONET ].wear, config.skincfg [ WEAPON_KNIFE_M9BAYONET ].seed, config.skincfg [ WEAPON_KNIFE_M9BAYONET ].stattrak, config.skincfg [ WEAPON_KNIFE_M9BAYONET ].name );
						else if ( cvar::misc_knifechanger_model == 5 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_HUNTSMAN ].skin, config.skincfg [ WEAPON_KNIFE_HUNTSMAN ].wear, config.skincfg [ WEAPON_KNIFE_HUNTSMAN ].seed, config.skincfg [ WEAPON_KNIFE_HUNTSMAN ].stattrak, config.skincfg [ WEAPON_KNIFE_HUNTSMAN ].name );
						else if ( cvar::misc_knifechanger_model == 6 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_FALCHION ].skin, config.skincfg [ WEAPON_KNIFE_FALCHION ].wear, config.skincfg [ WEAPON_KNIFE_FALCHION ].seed, config.skincfg [ WEAPON_KNIFE_FALCHION ].stattrak, config.skincfg [ WEAPON_KNIFE_FALCHION ].name );
						else if ( cvar::misc_knifechanger_model == 7 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_BUTTERFLY ].skin, config.skincfg [ WEAPON_KNIFE_BUTTERFLY ].wear, config.skincfg [ WEAPON_KNIFE_BUTTERFLY ].seed, config.skincfg [ WEAPON_KNIFE_BUTTERFLY ].stattrak, config.skincfg [ WEAPON_KNIFE_BUTTERFLY ].name );
						else if ( cvar::misc_knifechanger_model == 8 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_DAGGER ].skin, config.skincfg [ WEAPON_KNIFE_DAGGER ].wear, config.skincfg [ WEAPON_KNIFE_DAGGER ].seed, config.skincfg [ WEAPON_KNIFE_DAGGER ].stattrak, config.skincfg [ WEAPON_KNIFE_DAGGER ].name );
						else if ( cvar::misc_knifechanger_model == 9 )
							weapon->setPattern( info, config.skincfg [ WEAPON_KNIFE_BOWIE ].skin, config.skincfg [ WEAPON_KNIFE_BOWIE ].wear, config.skincfg [ WEAPON_KNIFE_BOWIE ].seed, config.skincfg [ WEAPON_KNIFE_BOWIE ].stattrak, config.skincfg [ WEAPON_KNIFE_BOWIE ].name );
					}
				}
			}
		}
	}
}