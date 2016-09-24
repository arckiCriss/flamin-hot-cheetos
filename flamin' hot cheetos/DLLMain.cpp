#include "stdafx.h"
#include <future>

//------------------------------------------------------------------------------------------
// flamin' hot cheetos
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// to do:
//  - make this project a little prettier
//    - working on converting from messy hungarian to http://geosoft.no/development/cppstyle.html
//  - use engine button handling to replace winapi functions
//  - clean-up classes such as vector (credits/thanks to whoever created it though!)
//  - implement proper aimbot smoothing (for mouse simulated)
//  - improve header inclusion
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// notes:
//  - modern c++ functions such as sleep_for and async threading breaks injection
//    - probably because of the injection method and run-time shit it does on entry
//  - manual mapping crashes game on team selection
//    - this is possible that it's just a injector specific bug
//------------------------------------------------------------------------------------------

bool shouldUnload = false;

DWORD __stdcall initializeRoutine( void* lpArguments )
{
	while ( !GetModuleHandleA( charenc( "client.dll" ) )
		|| !GetModuleHandleA( charenc( "engine.dll" ) ) )
		Sleep( 100 );

	config.loadConfig( );
	config.loadSkinConfig( );

	interfaces::initialize( );
	offsets::initialize( );
	hooks::initialize( );

	while ( !shouldUnload )
		Sleep( 1000 );

	FreeLibraryAndExitThread( ( HMODULE ) lpArguments, 0 );

	return 1;
}

DWORD __stdcall handleCore( void* lpArguments )
{
	while ( !shouldUnload )
	{
		if ( GetAsyncKeyState( VK_DELETE ) & 1 )
		{
			if ( GetForegroundWindow( ) == FindWindow( charenc( "Valve001" ), 0 ) )
				shouldUnload = true;
		}

		if ( GetAsyncKeyState( VK_HOME ) & 1 )
		{
			config.loadConfig( );
			config.loadSkinConfig( );
		}
		else if ( GetAsyncKeyState( VK_END ) & 1 )
		{
			config.saveConfig( );
			config.saveSkinConfig( );
		}

		Sleep( 1000 );
	}

	return 1;
}

BOOL APIENTRY DllMain( HMODULE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	switch ( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		// DisableThreadLibraryCalls(hInstance);
		CreateThread( nullptr, 0, initializeRoutine, hInstance, 0, nullptr );
		CreateThread( nullptr, 0, handleCore, hInstance, 0, nullptr );
		break;
	case DLL_PROCESS_DETACH:
		hooks::restore( );
		break;
	}

	return TRUE;
}