#include "Hooks.h"

namespace hooks
{
	std::unique_ptr<VFTManager> client = nullptr;
	std::unique_ptr<VFTManager> panel = nullptr;
	std::unique_ptr<VFTManager> surface = nullptr;
	std::unique_ptr<VFTManager> modelcache = nullptr;
	std::unique_ptr<VFTManager> renderview = nullptr;

	void initialize( )
	{
		panel = std::make_unique<VFTManager>( ( DWORD** ) interfaces::panel, true );
		originalPaintTraverse = panel->hook( 41, ( PaintTraverse_t ) PaintTraverse );

		client = std::make_unique<VFTManager>( ( DWORD** ) interfaces::client, true );
		originalCreateMove = client->hook( 21, ( CreateMove_t ) CreateMove );
		originalFrameStageNotify = client->hook( 36, ( FrameStageNotify_t ) FrameStageNotify );
		originalKeyEvent = client->hook( 20, ( IN_KeyEvent_t ) IN_KeyEvent );

		modelcache = std::make_unique<VFTManager>( ( DWORD** ) interfaces::modelcache, true );
		originalFindMDL = modelcache->hook( 10, ( FindMDL_t ) FindMDL );

		renderview = std::make_unique<VFTManager>( ( DWORD** ) interfaces::viewRender, true );
		originalRenderView = renderview->hook( 6, ( RenderView_t ) RenderView );

		surface = std::make_unique<VFTManager>( ( DWORD** ) interfaces::surface, true );
		originalOnScreenSizeChanged = surface->hook( 116, ( OnScreenSizeChanged_t ) OnScreenSizeChanged );

		interfaces::engine->clientCmd_Unrestricted( charenc( "echo [successfully hooked functions]" ) );
	}

	void restore( )
	{
		panel->restoreTable( );
		client->restoreTable( );
		modelcache->restoreTable( );
		surface->restoreTable( );

		Sleep( 500 );

		interfaces::engine->clientCmd_Unrestricted( charenc( "cl_mouseenable 1" ) );
		interfaces::engine->clientCmd_Unrestricted( charenc( "echo [successfully unhooked functions]" ) );
	}
}