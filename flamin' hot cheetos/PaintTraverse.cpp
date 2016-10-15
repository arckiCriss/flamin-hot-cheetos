#include "PaintTraverse.h"

PaintTraverse_t originalPaintTraverse;

void __stdcall PaintTraverse( unsigned int vguipanel, bool forcerepaint, bool allowforce )
{
	originalPaintTraverse( interfaces::panel, vguipanel, forcerepaint, allowforce );

	static std::once_flag onceFlag;
	std::call_once( onceFlag, [ ]
	{
		drawing.initializeFonts( );
	} );

	static unsigned int drawpanel = 0;

	if ( !drawpanel )
	{
		const char* panelName = interfaces::panel->GetName( vguipanel );
		if ( panelName && panelName [ 0 ] == 'M' && panelName [ 3 ] == 'S' && panelName [ 9 ] == 'T' && panelName [ 12 ] == 'P' )
			drawpanel = vguipanel;
	}

	if ( drawpanel == vguipanel )
	{
		if ( interfaces::engine->InGame( ) && interfaces::engine->IsConnected( ) )
		{
			CBaseEntity* local = interfaces::entitylist->GetClientEntity( interfaces::engine->GetLocalPlayer( ) );
			if ( !local )
				return;

			visuals.think( local );
		}

		drawing.drawString( drawing.menuTitleFont, false, 3, 3, Color( 255, 255, 255 ), charenc( "flamin' hot cheetos | 10-14-2016" ) );

		menu.think( );
	}
}