#include "OnScreenSizeChanged.h"

OnScreenSizeChanged_t originalOnScreenSizeChanged;

void __stdcall OnScreenSizeChanged( int oldWidth, int oldHeight )
{
	originalOnScreenSizeChanged( interfaces::surface, oldWidth, oldHeight );

	drawing.initializeFonts( );
}