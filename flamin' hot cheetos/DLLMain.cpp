#include "stdafx.h"

//------------------------------------------------------------------------------------------
// flamin' hot cheetos
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// to do:
//  - make this project a little prettier
//    - huge project clean-up should be done
//    - working on converting from hungarian to http://geosoft.no/development/cppstyle.html
//  - implement proper aimbot smoothing (for mouse simulation)
//  - improve header inclusion
//  - keyboard hook for keystate management
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// notes:
//  - modern c++ functions such as sleep_for and async threading (can) break injection
//  - manual mapping crashes game
//    - this is possible that it's just a injector specific bug
//    - fixed when compiling using v120 toolkit?
//------------------------------------------------------------------------------------------

void uninitializeRoutine( void* lpArguments );

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

	uninitializeRoutine( lpArguments );
}

void uninitializeRoutine( void* lpArguments )
{
	hooks::restore( );

	if ( lpArguments )
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

int __stdcall DllMain( HMODULE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	switch ( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) initializeRoutine, hInstance, 0, nullptr );
		CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE ) handleCore, hInstance, 0, nullptr );
		break;
	case DLL_PROCESS_DETACH:
		uninitializeRoutine( hInstance );
		break;
	}

	return 1;
}