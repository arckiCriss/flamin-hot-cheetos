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

	static unsigned int drawpanel = 0;

	if ( !drawpanel )
	{
		const char* pszPanelName = interfaces::panel->GetName( vguipanel );
		if ( pszPanelName && pszPanelName [ 0 ] == 'M' && pszPanelName [ 3 ] == 'S' && pszPanelName [ 9 ] == 'T' && pszPanelName [ 12 ] == 'P' )
			drawpanel = vguipanel;
	}

	if ( drawpanel == vguipanel )
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

		drawing.drawString( drawing.menuTitleFont, false, 3, 3, Color( 255, 255, 255 ), charenc( "flamin' hot cheetos | 09-27-2016" ) );
	}
}