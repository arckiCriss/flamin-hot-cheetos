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
		if ( !strcmp( interfaces::panel->getName( vguipanel ), charenc( "MatSystemTopPanel" ) ) )
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