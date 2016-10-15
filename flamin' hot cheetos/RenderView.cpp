#include "RenderView.h"

RenderView_t originalRenderView;

void __stdcall RenderView( CViewSetup& view, int clearflags, int whattodraw )
{
	if ( cvar::misc_overridefov > 0 )
		view.fovViewmodel += cvar::misc_overridefov;

	originalRenderView( view, clearflags, whattodraw );
}