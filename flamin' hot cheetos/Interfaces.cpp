#include "Interfaces.h"

namespace interfaces
{
	DWORD              clientbase = 0;

	CHLClient*         client = nullptr;
	void*              clientMode = nullptr;
	IEngineClient*     engine = nullptr;
	IClientEntityList* entitylist = nullptr;
	ISurface*          surface = nullptr;
	IPanel*            panel = nullptr;
	IVDebugOverlay*    debugoverlay = nullptr;
	IEngineTrace*      enginetrace = nullptr;
	CInput*            input = nullptr;
	IVModelInfo*       modelinfo = nullptr;
	CGlobalVars*       globalvars = nullptr;
	ICVar*             convar = nullptr;
	void*              viewRender = nullptr;

	void initialize( )
	{
		clientbase = ( DWORD ) GetModuleHandleA( charenc( "client.dll" ) );
		client = ( CHLClient* ) tools.getInterface( strenc( "client.dll" ), strenc( "VClient" ) );
		clientMode = **( void*** ) ( ( *( DWORD** ) interfaces::client ) [ 10 ] + 0x5 );
		engine = ( IEngineClient* ) tools.getInterface( strenc( "engine.dll" ), strenc( "VEngineClient" ) );
		entitylist = ( IClientEntityList* ) tools.getInterface( strenc( "client.dll" ), strenc( "VClientEntityList" ) );
		surface = ( ISurface* ) tools.getInterface( strenc( "vguimatsurface.dll" ), strenc( "VGUI_Surface" ) );
		panel = ( IPanel* ) tools.getInterface( strenc( "vgui2.dll" ), strenc( "VGUI_Panel" ) );
		debugoverlay = ( IVDebugOverlay* ) tools.getInterface( strenc( "engine.dll" ), strenc( "VDebugOverlay" ) );
		enginetrace = ( IEngineTrace* ) tools.getInterface( strenc( "engine.dll" ), strenc( "EngineTraceClient" ) );
		modelinfo = ( IVModelInfo* ) tools.getInterface( strenc( "engine.dll" ), strenc( "VModelInfoClient" ) );
		globalvars = ( CGlobalVars* ) *( DWORD* ) *( DWORD* ) ( tools.getPatternOffset( strenc( "client.dll" ), ( PBYTE ) charenc( "\xA1\x00\x00\x00\x00\x8B\x40\x10\x89\x41\x04" ), strenc( "x????xxxxxx" ) ) + 0x1 );
		convar = ( ICVar* ) tools.getInterface( strenc( "vstdlib.dll" ), strenc( "VEngineCvar" ) );
		viewRender = **( void*** ) ( ( DWORD ) tools.getPatternOffset( strenc( "client.dll" ), ( PBYTE ) charenc( "\xFF\x50\x14\xE8\x00\x00\x00\x00\x5D" ), strenc( "xxxx????x" ) ) - 7 );

		DWORD* clientVmt = ( DWORD* ) *( DWORD* ) client;
		input = *( CInput** ) ( clientVmt [ 15 ] + 0x1 );

		engine->clientCmd_Unrestricted( charenc( "echo [successfully initialized interfaces]" ) );
	}
}