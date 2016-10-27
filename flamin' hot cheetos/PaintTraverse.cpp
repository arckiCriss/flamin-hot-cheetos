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
		const char* panelName = interfaces::panel->getName( vguipanel );
		if ( panelName && panelName [ 0 ] == 'M' && panelName [ 3 ] == 'S' && panelName [ 9 ] == 'T' && panelName [ 12 ] == 'P' )
			drawpanel = vguipanel;
	}

	if ( drawpanel == vguipanel )
	{
		if ( interfaces::engine->inGame( ) && interfaces::engine->isConnected( ) )
		{
			CBaseEntity* local = interfaces::entitylist->getClientEntity( interfaces::engine->getLocalPlayer( ) );
			if ( !local )
				return;

			visuals.think( local );
		}

		drawing.drawString( drawing.menuTitleFont, false, 3, 3, Color( 255, 255, 255 ), charenc( "flamin' hot cheetos | 10-27-2016" ) );

		menu.think( );
	}
}