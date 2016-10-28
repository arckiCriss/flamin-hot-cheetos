#include "IN_KeyEvent.h"

#include "Menu.h"

IN_KeyEvent_t originalKeyEvent;

int __stdcall IN_KeyEvent( int eventCode, int keyNum, const char* currentBinding )
{
	if ( menu.isActive( ) )
		return 0;

	return originalKeyEvent( interfaces::client, eventCode, keyNum, currentBinding );
}