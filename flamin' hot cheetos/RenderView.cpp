#include "RenderView.h"

RenderView_t originalRenderView;

void __stdcall RenderView( CViewSetup& view, CViewSetup& hudViewSetup, int clearFlags, int whatToDraw )
{
	if ( cvar::misc_overridefov > 0 )
		view.fovViewmodel += cvar::misc_overridefov;

	originalRenderView( interfaces::viewRender, view, hudViewSetup, clearFlags, whatToDraw );
}