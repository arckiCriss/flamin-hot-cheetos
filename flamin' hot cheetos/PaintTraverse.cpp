#include "PaintTraverse.h"

#include "Visuals.h"
#include "Menu.h"

PaintTraverse_t originalPaintTraverse;

void __stdcall PaintTraverse( unsigned int vguipanel, bool forceRepaint, bool allowForce )
{
	originalPaintTraverse( interfaces::panel, vguipanel, forceRepaint, allowForce );

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
		visuals.think( );

		drawing.drawString( drawing.menuTitleFont, false, 3, 3, Color( 255, 255, 255 ), charenc( "flamin' hot cheetos | 10-29-2016" ) );

		menu.think( );
	}
}