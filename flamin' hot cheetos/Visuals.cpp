#include "Visuals.h"

Visuals visuals;

Visuals::Visuals( )
{
	width = 0;
	height = 0;

	espColor = Color( 0, 0, 0, 0 );
	glowColor = Color( 0, 0, 0, 0 );
}

void Visuals::think( )
{
	if ( !cvar::esp_enabled )
		return;

	if ( !interfaces::engine->inGame( ) || !interfaces::engine->isConnected( ) )
		return;

	CBaseEntity* local = interfaces::entitylist->getClientEntity( interfaces::engine->getLocalPlayer( ) );
	if ( !local )
		return;

	for ( int i = 1; i <= interfaces::entitylist->getHighestEntityIndex( ); i++ )
	{
		if ( i == local->getIndex( ) )
			continue;

		CBaseEntity* entity = interfaces::entitylist->getClientEntity( i );
		if ( !entity
			|| entity->isDormant( ) )
			continue;

		drawWorld( entity );

		if ( entity->getClientClass( )->getClassID( ) != CCSPlayer
			|| entity->getLifeState( ) != LIFE_ALIVE
			|| entity->getTeamNum( ) == local->getTeamNum( )
			|| !interfaces::engine->getPlayerInfo( i, &info ) )
			continue;

		drawGlow( entity );
		drawPlayer( local, entity );
	}

	drawScoreboard( local );
}

void Visuals::getScreenSize( )
{
	interfaces::engine->getScreenSize( width, height );
}

void Visuals::drawPlayer( CBaseEntity* local, CBaseEntity* entity )
{
	const matrix3x4& trans = entity->getCoordinateFrame( );

	Vector min, max;
	entity->getRenderBounds( min, max );

	Vector pointList [ ] = {
		Vector( min.x, min.y, min.z ),
		Vector( min.x, max.y, min.z ),
		Vector( max.x, max.y, min.z ),
		Vector( max.x, min.y, min.z ),
		Vector( max.x, max.y, max.z ),
		Vector( min.x, max.y, max.z ),
		Vector( min.x, min.y, max.z ),
		Vector( max.x, min.y, max.z )
	};

	Vector transformed [ 8 ];

	for ( int i = 0; i < 8; i++ )
		tools.VectorTransform( pointList [ i ], trans, transformed [ i ] );

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if ( !tools.WorldToScreen( transformed [ 3 ], flb )
		|| !tools.WorldToScreen( transformed [ 0 ], blb )
		|| !tools.WorldToScreen( transformed [ 2 ], frb )
		|| !tools.WorldToScreen( transformed [ 6 ], blt )
		|| !tools.WorldToScreen( transformed [ 5 ], brt )
		|| !tools.WorldToScreen( transformed [ 4 ], frt )
		|| !tools.WorldToScreen( transformed [ 1 ], brb )
		|| !tools.WorldToScreen( transformed [ 7 ], flt ) )
		return;

	Vector arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for ( int i = 0; i < 8; i++ )
	{
		if ( left > arr [ i ].x )
			left = arr [ i ].x;
		if ( top < arr [ i ].y )
			top = arr [ i ].y;
		if ( right < arr [ i ].x )
			right = arr [ i ].x;
		if ( bottom > arr [ i ].y )
			bottom = arr [ i ].y;
	}

	float x = left;
	float y = bottom;
	float w = right - left;
	float h = top - bottom;

	if ( tools.isVisible( local->getEyePosition( ), entity->getEyePosition( ), entity ) )
	{
		if ( entity->getTeamNum( ) == 2 )
			espColor = Color( 255, 0, 0 );
		else if ( entity->getTeamNum( ) == 3 )
			espColor = Color( 0, 0, 255 );
	}
	else
	{
		if ( entity->getTeamNum( ) == 2 )
			espColor = Color( 255, 255, 0 );
		else if ( entity->getTeamNum( ) == 3 )
			espColor = Color( 0, 255, 0 );
	}

	if ( cvar::esp_draw_box )
		drawing.drawOutlinedBox( x, y, w, h, espColor, Color( 0, 0, 0 ) );

	if ( cvar::esp_draw_health )
		drawing.drawBar( x - 4, y + h / 2 - 1, 4, h + 1, Color( 255.0f - entity->getHealth( ) * 2.55f, entity->getHealth( ) * 2.55f, 0 ), entity->getHealth( ) );

	if ( cvar::esp_draw_armor )
		drawing.drawBar( x - 9, y + h / 2 - 1, 4, h + 1, Color( 255.0f - entity->getArmor( ) * 2.55f, entity->getArmor( ) * 2.55f, 0 ), entity->getArmor( ) );

	if ( cvar::esp_draw_name )
		drawing.drawString( drawing.espFont, true, x + w / 2, y - 16, Color( 255, 255, 255 ), info.m_szPlayerName );

	if ( cvar::esp_draw_weapon )
	{
		CBaseCombatWeapon* weapon = tools.getActiveWeapon( entity );
		if ( weapon )
		{
			char weaponName [ 32 ];
			switch ( weapon->getItemDefinitionIndex( ) )
			{
			case ITEM_NONE: { strcpy( weaponName, charenc( "NONE" ) ); break; }
			case WEAPON_DEAGLE: { strcpy( weaponName, charenc( "DEAGLE" ) ); break; }
			case WEAPON_DUALS: { strcpy( weaponName, charenc( "DUALIES" ) ); break; }
			case WEAPON_FIVE7: { strcpy( weaponName, charenc( "FIVE-SEVEN" ) ); break; }
			case WEAPON_GLOCK: { strcpy( weaponName, charenc( "GLOCK-18" ) ); break; }
			case WEAPON_AK47: { strcpy( weaponName, charenc( "AK-47" ) ); break; }
			case WEAPON_AUG: { strcpy( weaponName, charenc( "AUG" ) ); break; }
			case WEAPON_AWP: { strcpy( weaponName, charenc( "AWP" ) ); break; }
			case WEAPON_FAMAS: { strcpy( weaponName, charenc( "FAMAS" ) ); break; }
			case WEAPON_G3SG1: { strcpy( weaponName, charenc( "G3SG1" ) ); break; }
			case WEAPON_GALIL: { strcpy( weaponName, charenc( "GALIL" ) ); break; }
			case WEAPON_M249: { strcpy( weaponName, charenc( "M249" ) ); break; }
			case WEAPON_M4A1: { strcpy( weaponName, charenc( "M4A1" ) ); break; }
			case WEAPON_MAC10: { strcpy( weaponName, charenc( "MAC-10" ) ); break; }
			case WEAPON_P90: { strcpy( weaponName, charenc( "P90" ) ); break; }
			case WEAPON_UMP45: { strcpy( weaponName, charenc( "UMP-45" ) ); break; }
			case WEAPON_XM1014: { strcpy( weaponName, charenc( "XM1014" ) ); break; }
			case WEAPON_BIZON: { strcpy( weaponName, charenc( "BIZON" ) ); break; }
			case WEAPON_MAG7: { strcpy( weaponName, charenc( "MAG-7" ) ); break; }
			case WEAPON_NEGEV: { strcpy( weaponName, charenc( "NEGEV" ) ); break; }
			case WEAPON_SAWEDOFF: { strcpy( weaponName, charenc( "SAWED-OFF" ) ); break; }
			case WEAPON_TEC9: { strcpy( weaponName, charenc( "TEC-9" ) ); break; }
			case WEAPON_TASER: { strcpy( weaponName, charenc( "ZEUS" ) ); break; }
			case WEAPON_P2000: { strcpy( weaponName, charenc( "P2000" ) ); break; }
			case WEAPON_MP7: { strcpy( weaponName, charenc( "MP7" ) ); break; }
			case WEAPON_MP9: { strcpy( weaponName, charenc( "MP9" ) ); break; }
			case WEAPON_NOVA: { strcpy( weaponName, charenc( "NOVA" ) ); break; }
			case WEAPON_P250: { strcpy( weaponName, charenc( "P250" ) ); break; }
			case WEAPON_SCAR20: {  strcpy( weaponName, charenc( "SCAR-20" ) ); break; }
			case WEAPON_SG553: { strcpy( weaponName, charenc( "SG553" ) ); break; }
			case WEAPON_SCOUT: { strcpy( weaponName, charenc( "SCOUT" ) ); break; }
			case WEAPON_REVOLVER: { strcpy( weaponName, charenc( "REVOLVER" ) ); break; }
			case WEAPON_KNIFE_CT: { strcpy( weaponName, charenc( "KNIFE" ) ); break; }
			case WEAPON_KNIFE_T: { strcpy( weaponName, charenc( "KNIFE" ) ); break; }
			case WEAPON_KNIFE_GUT: { strcpy( weaponName, charenc( "GUT KNIFE" ) ); break; }
			case WEAPON_KNIFE_FLIP: { strcpy( weaponName, charenc( "FLIP KNIFE" ) ); break; }
			case WEAPON_KNIFE_BAYONET: { strcpy( weaponName, charenc( "BAYONET" ) ); break; }
			case WEAPON_KNIFE_KARAMBIT: { strcpy( weaponName, charenc( "KARAMBIT" ) ); break; }
			case WEAPON_KNIFE_M9BAYONET: { strcpy( weaponName, charenc( "M9 BAYONET" ) ); break; }
			case WEAPON_KNIFE_BUTTERFLY: { strcpy( weaponName, charenc( "BUTTERFLY KNIFE" ) ); break; }
			case WEAPON_KNIFE_HUNTSMAN: { strcpy( weaponName, charenc( "HUNTSMAN KNIFE" ) ); break; }
			case WEAPON_KNIFE_BOWIE: { strcpy( weaponName, charenc( "BOWIE KNIFE" ) ); break; }
			case WEAPON_FLASH: { strcpy( weaponName, charenc( "FLASHBANG" ) ); break; }
			case WEAPON_HE: { strcpy( weaponName, charenc( "HE GRENADE" ) ); break; }
			case WEAPON_SMOKE: { strcpy( weaponName, charenc( "SMOKE GRENADE" ) ); break; }
			case WEAPON_MOLOTOV: { strcpy( weaponName, charenc( "MOLOTOV" ) ); break; }
			case WEAPON_DECOY: { strcpy( weaponName, charenc( "DECOY GRENADE" ) ); break; }
			case WEAPON_INC: { strcpy( weaponName, charenc( "INCENDIARY GRENADE" ) ); break; }
			case WEAPON_M4A1S: { strcpy( weaponName, charenc( "M4A1-S" ) ); break; }
			case WEAPON_USPS: { strcpy( weaponName, charenc( "USP-S" ) ); break; }
			case WEAPON_CZ75: { strcpy( weaponName, charenc( "CZ-75" ) ); break; }
			case WEAPON_C4: { strcpy( weaponName, charenc( "C4" ) ); break; }
			default: { strcpy( weaponName, charenc( "NONE" ) ); break; }
			}

			if ( !weapon->isValid( ) )
				sprintf_s( weaponName, sizeof( weaponName ), "%s | %i", weaponName, weapon->getClip1( ) );
			else
				sprintf_s( weaponName, sizeof( weaponName ), "%s", weaponName );

			drawing.drawString( drawing.espFont, true, x + w / 2, top + 2, Color( 255, 255, 255 ), weaponName );
		}
	}

	int place = 0;

	if ( cvar::esp_draw_health_text )
		drawing.drawString( drawing.espFont, false, x + w + 5, y - 4 + ( place++ * 11 ), Color( 255, 255, 255 ), charenc( "%i HP" ), entity->getHealth( ) );

	if ( cvar::esp_draw_armor_text )
		drawing.drawString( drawing.espFont, false, x + w + 5, y - 4 + ( place++ * 11 ), Color( 255, 255, 255 ), charenc( "%i AP" ), entity->getArmor( ) );

	if ( cvar::esp_draw_callout )
		drawing.drawString( drawing.espFont, false, x + w + 5, y - 4 + ( place++ * 11 ), Color( 255, 255, 255 ), entity->getLastPlaceName( ) );
}

void Visuals::drawWorld( CBaseEntity* entity )
{
	if ( !cvar::esp_draw_world )
		return;

	if ( entity->getAbsAngles( ).x == 0.0f && entity->getAbsAngles( ).z == 0.0f )
		return;

	std::string modelName = interfaces::modelinfo->getModelName( entity->getModel( ) );

	if ( modelName.empty( ) )
		return;

	if ( modelName.find( strenc( "models/weapons/w_pist_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_rif_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_mach_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_snip_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_eq_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_shot_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos
		|| modelName.find( strenc( "models/weapons/w_smg_" ) ) != std::string::npos && modelName.find( strenc( "_dropped.mdl" ) ) != std::string::npos )
	{
		modelName.erase( 0, modelName.find_first_of( "_", std::string( strenc( "abc" ) ).length( ) ) + 1 );
		modelName.erase( modelName.find_last_of( "_" ), 12 );
		modelName.erase( 0, modelName.find_first_of( "_", std::string( strenc( "abc" ) ).length( ) ) + 1 );

		if ( !strcmp( modelName.c_str( ), charenc( "ied" ) ) )
			modelName = strenc( "c4" );
		else if ( !strcmp( modelName.c_str( ), charenc( "223" ) ) )
			modelName = strenc( "usp-s" );
		else if ( !strcmp( modelName.c_str( ), charenc( "75" ) ) )
			modelName = strenc( "cz75" );
		else if ( !strcmp( modelName.c_str( ), charenc( "s" ) ) )
			modelName = strenc( "m4a1-s" );

		drawBoundingBox( entity, Color( 255, 255, 255 ), modelName.c_str( ) );
	}
}

void Visuals::drawBoundingBox( CBaseEntity* entity, Color color, const char* text )
{
	const matrix3x4& trans = entity->getCoordinateFrame( );

	Vector min, max;
	entity->getRenderBounds( min, max );

	Vector pointList [ ] = {
		Vector( min.x, min.y, min.z ),
		Vector( min.x, max.y, min.z ),
		Vector( max.x, max.y, min.z ),
		Vector( max.x, min.y, min.z ),
		Vector( max.x, max.y, max.z ),
		Vector( min.x, max.y, max.z ),
		Vector( min.x, min.y, max.z ),
		Vector( max.x, min.y, max.z )
	};

	Vector transformed [ 8 ];

	for ( int i = 0; i < 8; i++ )
		tools.VectorTransform( pointList [ i ], trans, transformed [ i ] );

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if ( !tools.WorldToScreen( transformed [ 3 ], flb ) ||
		!tools.WorldToScreen( transformed [ 0 ], blb ) ||
		!tools.WorldToScreen( transformed [ 2 ], frb ) ||
		!tools.WorldToScreen( transformed [ 6 ], blt ) ||
		!tools.WorldToScreen( transformed [ 5 ], brt ) ||
		!tools.WorldToScreen( transformed [ 4 ], frt ) ||
		!tools.WorldToScreen( transformed [ 1 ], brb ) ||
		!tools.WorldToScreen( transformed [ 7 ], flt ) )
		return;

	Vector arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for ( int i = 0; i < 8; i++ )
	{
		if ( left > arr [ i ].x )
			left = arr [ i ].x;
		if ( top < arr [ i ].y )
			top = arr [ i ].y;
		if ( right < arr [ i ].x )
			right = arr [ i ].x;
		if ( bottom > arr [ i ].y )
			bottom = arr [ i ].y;
	}

	float w = right - left;
	float h = bottom - top;

	drawing.drawLine( left, bottom, left, top, color );
	drawing.drawLine( left - 1, bottom - 1, left - 1, top + 2, Color( 0, 0, 0 ) );
	drawing.drawLine( left + 1, bottom + 1, left + 1, top + 2, Color( 0, 0, 0 ) );

	drawing.drawLine( right, top, right, bottom, color );
	drawing.drawLine( right + 1, top + 1, right + 1, bottom - 1, Color( 0, 0, 0 ) );
	drawing.drawLine( right - 1, top - 1, right - 1, bottom + 1, Color( 0, 0, 0 ) );

	drawing.drawLine( left, top, right, top, color );
	drawing.drawLine( left + 1, top - 1, right + 1, top - 1, Color( 0, 0, 0 ) );
	drawing.drawLine( left, top + 1, right + 1, top + 1, Color( 0, 0, 0 ) );

	drawing.drawLine( right, bottom, left, bottom, color );
	drawing.drawLine( right + 1, bottom - 1, left - 1, bottom - 1, Color( 0, 0, 0 ) );
	drawing.drawLine( right - 1, bottom + 1, left + 1, bottom + 1, Color( 0, 0, 0 ) );

	if ( text )
		drawing.drawString( drawing.espFont, true, left + w / 2, top - 15, color, text );
}

void Visuals::drawGlow( CBaseEntity* entity )
{
	if ( !cvar::esp_draw_glow )
		return;

	static GlowObjectPointer_t getGlowObject = 0;

	if ( !getGlowObject )
		getGlowObject = ( GlowObjectPointer_t ) ( tools.getPatternOffset( strenc( "client.dll" ), ( PBYTE ) charenc( "\xA1\x00\x00\x00\x00\xA8\x01\x75\x4E\x33" ), strenc( "x????xxxx" ) ) );

	if ( !getGlowObject )
		return;

	void* glowObjectPointer = getGlowObject( );

	UINT32 glowIndex = *( UINT32* ) ( ( uintptr_t ) entity + 0xA308 + 0x18 );

	CGlowObjectManager::GlowObjectDefinition_t* glowObjectArray = *( CGlowObjectManager::GlowObjectDefinition_t** )glowObjectPointer;
	CGlowObjectManager::GlowObjectDefinition_t* glowObject = &glowObjectArray [ glowIndex ];
	if ( !glowObject )
		return;

	if ( entity->getTeamNum( ) == 2 )
		glowColor = Color( 223, 175, 86, 153 );
	else if ( entity->getTeamNum( ) == 3 )
		glowColor = Color( 113, 154, 220, 153 );

	glowObject->glowColor = Vector( ( 1.0f / 255.0f ) * glowColor.r( ), ( 1.0f / 255.0f ) * glowColor.g( ), ( 1.0f / 255.0f ) * glowColor.b( ) );
	glowObject->glowAlpha = ( 1.0f / 255.0f ) * glowColor.a( );
	glowObject->shouldGlow = true;
	glowObject->isUnknown = false;
}

void Visuals::drawScoreboard( CBaseEntity* local )
{
	if ( !cvar::misc_scoreboard )
		return;

	static bool shouldDraw = false;

	if ( GetAsyncKeyState( VK_PRIOR ) & 1 )
		shouldDraw = !shouldDraw;

	if ( !shouldDraw )
		return;

	int boardWidth = 950, boardHeight = 325;

	getScreenSize( );
	int x = width / 2 - boardWidth / 2;
	int y = height / 2 - boardHeight / 2;

	drawing.drawFilledRect( x, y, boardWidth, boardHeight, Color( 10, 10, 10 ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 10, y + 4, Color( 255, 255, 255 ), charenc( "flamin' hot scoreboard" ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 327 + 65, y + 4, Color( 255, 255, 255 ), charenc( "HP" ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 375 + 65, y + 4, Color( 255, 255, 255 ), charenc( "Money" ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 475 + 65, y + 4, Color( 255, 255, 255 ), charenc( "Wins" ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 605, y + 4, Color( 255, 255, 255 ), charenc( "Rank" ) );
	drawing.drawString( drawing.scoreboardFont, false, x + 805, y + 4, Color( 255, 255, 255 ), charenc( "Steam ID" ) );
	drawing.drawLine( x + 10, y + 26, x + boardWidth - 10, y + 26, Color( 90, 90, 90, 205 ) );
	drawing.drawOutlinedRect( x - 2, y - 2, boardWidth + 4, boardHeight + 4, Color( 0, 0, 0 ) );

	drawing.drawString( drawing.scoreboardFont, false, x + 15, y + 35, Color( 255, 0, 0 ), charenc( "Terrorists" ) );
	drawing.drawLine( x + 10, y + 50, x + boardWidth - 10, y + 50, Color( 255, 0, 0 ) );

	drawing.drawString( drawing.scoreboardFont, false, x + 15, y + 190, Color( 0, 100, 255 ), charenc( "Counter-Terrorists" ) );
	drawing.drawLine( x + 10, y + 205, x + boardWidth - 10, y + 205, Color( 0, 100, 255 ) );

	static DWORD playerResource = 0;

	if ( !playerResource )
		playerResource = tools.getPatternOffset( strenc( "client.dll" ), ( PBYTE ) charenc( "\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x00\x53\x56\x8B\xD9\xC7" ), strenc( "xxxxxxxx????xx?????xxxxx" ) ) + 0xE;

	if ( !playerResource )
		return;

	DWORD resourcePointer = **( DWORD** ) playerResource;

	int place = 0, place2 = 0;
	bool doSwap = false, doSwap2 = false;

	for ( int i = 1; i <= interfaces::globalvars->maxclients; i++ )
	{
		player_info_t info;

		CBaseEntity* entity = interfaces::entitylist->getClientEntity( i );
		if ( !entity
			|| !interfaces::engine->getPlayerInfo( i, &info ) )
			continue;

		int playerRank = *( int* ) ( resourcePointer + offsets::player::m_iCompetitiveRanking + i * 4 );
		int playerWins = *( int* ) ( resourcePointer + offsets::player::m_iCompetitiveWins + i * 4 );

		char playerRankName [ 64 ];
		switch ( playerRank )
		{
		case 0: {strcpy( playerRankName, charenc( "Unranked" ) ); break; }
		case 1: {strcpy( playerRankName, charenc( "Silver I" ) ); break; }
		case 2: {strcpy( playerRankName, charenc( "Silver II" ) ); break; }
		case 3: {strcpy( playerRankName, charenc( "Silver III" ) ); break; }
		case 4: {strcpy( playerRankName, charenc( "Silver IV" ) ); break; }
		case 5: {strcpy( playerRankName, charenc( "Silver Elite" ) ); break; }
		case 6: {strcpy( playerRankName, charenc( "Silver Elite Master" ) ); break; }
		case 7: {strcpy( playerRankName, charenc( "Gold Nova I" ) ); break; }
		case 8: {strcpy( playerRankName, charenc( "Gold Nova II" ) ); break; }
		case 9: {strcpy( playerRankName, charenc( "Gold Nova III" ) ); break; }
		case 10: {strcpy( playerRankName, charenc( "Gold Nova Master" ) ); break; }
		case 11: {strcpy( playerRankName, charenc( "Master Guardian I" ) ); break; }
		case 12: {strcpy( playerRankName, charenc( "Master Guardian II" ) ); break; }
		case 13: {strcpy( playerRankName, charenc( "Master Guardian Elite" ) ); break; }
		case 14: {strcpy( playerRankName, charenc( "Distinguished Master Guardian" ) ); break; }
		case 15: {strcpy( playerRankName, charenc( "Legendary Eagle" ) ); break; }
		case 16: {strcpy( playerRankName, charenc( "Legendary Eagle Master" ) ); break; }
		case 17: {strcpy( playerRankName, charenc( "Supreme Master First Class" ) ); break; }
		case 18: {strcpy( playerRankName, charenc( "The Global Elite" ) ); break; }
		}

		if ( entity->getTeamNum( ) == 2 )
		{
			place++;

			if ( place <= 5 )
			{
				doSwap = !doSwap;

				players [ i ].name = info.m_szPlayerName;
				players [ i ].steamid = info.m_szSteamID;
				players [ i ].health = entity->getHealth( );
				players [ i ].money = entity->getAccount( );
				players [ i ].mmrank = playerRankName;
				players [ i ].mmwins = playerWins;

				if ( doSwap )
					drawing.drawFilledRect( x + 8, y + 38 + ( place * 20 ), boardWidth - 18, 19, Color( 20, 20, 20, 150 ) );

				drawing.drawString( drawing.scoreboardFont, false, x + 10, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), players [ i ].name );
				drawing.drawString( drawing.scoreboardFont, false, x + 325 + 65, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), "%i", players [ i ].health );
				drawing.drawString( drawing.scoreboardFont, false, x + 375 + 65, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), "$%i", players [ i ].money );
				drawing.drawString( drawing.scoreboardFont, false, x + 475 + 65, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), "%i", players [ i ].mmwins );
				drawing.drawString( drawing.scoreboardFont, false, x + 605, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), players [ i ].mmrank );
				drawing.drawString( drawing.scoreboardFont, false, x + 805, y + 40 + ( place * 20 ), Color( 255, 0, 0 ), players [ i ].steamid );
			}
		}

		if ( entity->getTeamNum( ) == 3 )
		{
			place2++;

			if ( place2 <= 5 )
			{
				doSwap2 = !doSwap2;

				players [ i ].name = info.m_szPlayerName;
				players [ i ].steamid = info.m_szSteamID;
				players [ i ].health = entity->getHealth( );
				players [ i ].money = entity->getAccount( );
				players [ i ].mmrank = playerRankName;
				players [ i ].mmwins = playerWins;

				if ( doSwap2 )
					drawing.drawFilledRect( x + 8, y + 193 + ( place2 * 20 ), boardWidth - 18, 19, Color( 20, 20, 20, 150 ) );

				drawing.drawString( drawing.scoreboardFont, false, x + 10, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), players [ i ].name );
				drawing.drawString( drawing.scoreboardFont, false, x + 325 + 65, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), "%i", players [ i ].health );
				drawing.drawString( drawing.scoreboardFont, false, x + 375 + 65, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), "$%i", players [ i ].money );
				drawing.drawString( drawing.scoreboardFont, false, x + 475 + 65, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), "%i", players [ i ].mmwins );
				drawing.drawString( drawing.scoreboardFont, false, x + 605, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), players [ i ].mmrank );
				drawing.drawString( drawing.scoreboardFont, false, x + 805, y + 195 + ( place2 * 20 ), Color( 0, 100, 255 ), players [ i ].steamid );
			}
		}
	}

	place = 0, place2 = 0;
	doSwap = false, doSwap2 = false;
}