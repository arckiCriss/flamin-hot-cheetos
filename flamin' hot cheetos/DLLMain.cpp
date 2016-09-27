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
//  - clean-up classes such as vector (credits to whoever created it though)
//  - implement proper aimbot smoothing (for mouse simulation)
//  - improve header inclusion
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// notes:
//  - modern c++ functions such as sleep_for and async threading (can) break injection
//  - manual mapping crashes game
//    - this is possible that it's just a injector specific bug
//    - fixed when compiling using v120 toolkit?
//------------------------------------------------------------------------------------------

bool shouldUnload = false;

void initializeRoutine( void* lpArguments )
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

	hooks::restore( );

	FreeLibraryAndExitThread( ( HMODULE ) lpArguments, 0 );
}

void handleCore( void* lpArguments )
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
}

BOOL APIENTRY DllMain( HMODULE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	switch ( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls( hInstance );
		std::async( std::launch::async, initializeRoutine, hInstance );
		std::async( std::launch::async, handleCore, hInstance );
		// CreateThread( nullptr, 0, initializeRoutine, hInstance, 0, nullptr );
		// CreateThread( nullptr, 0, handleCore, hInstance, 0, nullptr );
		break;
	}

	return TRUE;
}