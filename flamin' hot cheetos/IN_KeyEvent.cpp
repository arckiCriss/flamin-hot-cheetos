#include "IN_KeyEvent.h"

IN_KeyEvent_t originalKeyEvent;

int __stdcall IN_KeyEvent( int eventcode, int keynum, const char* currentbinding )
{
	if ( menu.isActive( ) )
		return 0;

	return originalKeyEvent( eventcode, keynum, currentbinding );
}