#include "Hooks.h"
#include "Proxy.h"

namespace hooks
{
	std::unique_ptr<VFTManager> client = nullptr;
	std::unique_ptr<VFTManager> panel = nullptr;
	std::unique_ptr<VFTManager> renderview = nullptr;
	std::unique_ptr<VFTManager> surface = nullptr;

	void initialize( )
	{
		panel = std::make_unique<VFTManager>( ( DWORD** ) interfaces::panel, true );
		originalPaintTraverse = panel->hook( 41, ( PaintTraverse_t ) PaintTraverse );

		client = std::make_unique<VFTManager>( ( DWORD** ) interfaces::client, true );
		originalKeyEvent = client->hook( 20, ( IN_KeyEvent_t ) IN_KeyEvent );
		// originalCreateMove = client->hook( 21, ( CreateMove_t ) CreateMove );
		originalFrameStageNotify = client->hook( 36, ( FrameStageNotify_t ) FrameStageNotify );

		renderview = std::make_unique<VFTManager>( ( DWORD** ) interfaces::viewRender, true );
		// originalRenderView = renderview->hook( 6, ( RenderView_t ) RenderView );

		surface = std::make_unique<VFTManager>( ( DWORD** ) interfaces::surface, true );
		// originalOnScreenSizeChanged = surface->hook( 116, ( OnScreenSizeChanged_t ) OnScreenSizeChanged );

		proxy::initialize( );

		interfaces::engine->clientCmd_Unrestricted( charenc( "echo [successfully hooked functions]" ) );
	}

	void restore( )
	{
		panel->restoreTable( );
		client->restoreTable( );
		renderview->restoreTable( );
		surface->restoreTable( );

		Sleep( 500 );

		interfaces::engine->clientCmd_Unrestricted( charenc( "cl_mouseenable 1" ) );
		interfaces::engine->clientCmd_Unrestricted( charenc( "echo [successfully unhooked functions]" ) );
	}
}