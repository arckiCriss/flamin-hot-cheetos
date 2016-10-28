#include "RenderView.h"

RenderView_t originalRenderView;

void __fastcall RenderView( void* thisptr, void*, CViewSetup& view, CViewSetup& hudViewSetup, int clearFlags, int whatToDraw )
{
	if ( cvar::misc_overridefov > 0 )
		view.fovViewmodel += cvar::misc_overridefov;

	originalRenderView( thisptr, view, hudViewSetup, clearFlags, whatToDraw );
}