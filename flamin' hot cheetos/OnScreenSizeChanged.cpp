#include "OnScreenSizeChanged.h"

OnScreenSizeChanged_t originalOnScreenSizeChanged;

void __stdcall OnScreenSizeChanged( int oldwidth, int oldheight )
{
	originalOnScreenSizeChanged( interfaces::surface, oldwidth, oldheight );

	drawing.initializeFonts( );
}