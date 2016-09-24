#include "stdafx.h"
#include <future>

//------------------------------------------------------------------------------------------
// flamin' hot cheetos
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// to do:
//  - make this project a little prettier
//    - working on converting from messy hungarian to http://geosoft.no/development/cppstyle.html
//    - I have trouble deciding whether I should make x variable a class member
//  - use engine button handling to replace getasynckeystate
//  - clean-up classes such as Vector (credits/thanks to whoever created it though!)
//  - implement proper aimbot smoothing
//  - improve header inclusion
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
// notes:
//  - modern c++ functions such as sleep_for and async threading breaks injection
//    - probably because of the run-time shit it does on process entry
//  - manual mapping crashes game on team selection
//    - this is possible that it's just a injector specific bug
//------------------------------------------------------------------------------------------

bool shouldUnload = false;

DWORD __stdcall initializeRoutine( void* hInstance )
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

	FreeLibraryAndExitThread( ( HMODULE ) hInstance, 0 );
}

BOOL APIENTRY DllMain( HMODULE hInstance, DWORD dwReason, LPVOID lpReserved )
{
	switch ( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		// DisableThreadLibraryCalls(hInstance);
		CreateThread( nullptr, 0, initializeRoutine, hInstance, 0, nullptr );
		break;
	case DLL_PROCESS_DETACH:
		hooks::restore( );
		break;
	}

	return TRUE;
}