#include "PaintTraverse.h"

PaintTraverse_t originalPaintTraverse;

void __stdcall PaintTraverse( unsigned int vguipanel, bool forcerepaint, bool allowforce )
{
	originalPaintTraverse( interfaces::panel, vguipanel, forcerepaint, allowforce );

	static bool doOnce = false;
	if ( !doOnce )
	{
		drawing.initializeFonts( );
		doOnce = true;
	}

	const char* panelName = interfaces::panel->GetName( vguipanel );
	if ( !panelName )
		return;

	if ( panelName [ 0 ] == 'M' && panelName [ 3 ] == 'S' && panelName [ 9 ] == 'T' )
	{
		if ( interfaces::engine->InGame( ) && interfaces::engine->IsConnected( ) )
		{
			CBaseEntity* local = interfaces::entitylist->GetClientEntity( interfaces::engine->GetLocalPlayer( ) );
			if ( !local )
				return;

			visuals.getScreenSize( );
			visuals.think( local );
		}

		menu.think( );

		drawing.drawString( drawing.menuTitleFont, false, 3, 3, Color( 255, 255, 255 ), charenc( "flamin' hot cheetos | 09-24-2016" ) );
	}
}