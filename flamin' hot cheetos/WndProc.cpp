#include "WndProc.h"

HWND windowHandle = 0;
WNDPROC originalWindowProc = 0;

bool isPressed [ 256 ] = { };

LONG_PTR __stdcall WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
	case WM_LBUTTONDOWN:
		isPressed [ VK_LBUTTON ] = true;
		break;
	case WM_LBUTTONUP:
		isPressed [ VK_LBUTTON ] = false;
		break;
	case WM_RBUTTONDOWN:
		isPressed [ VK_RBUTTON ] = true;
		break;
	case WM_RBUTTONUP:
		isPressed [ VK_RBUTTON ] = false;
		break;
	case WM_SYSKEYDOWN:
		isPressed [ wParam ] = true;
		break;
	case WM_SYSKEYUP:
		isPressed [ wParam ] = false;
		break;
	case WM_KEYDOWN:
		isPressed [ wParam ] = true;
		break;
	case WM_KEYUP:
		isPressed [ wParam ] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM( wParam );

		if ( button == XBUTTON1 )
			isPressed [ VK_XBUTTON1 ] = true;
		else if ( button == XBUTTON2 )
			isPressed [ VK_XBUTTON2 ] = true;

		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM( wParam );

		if ( button == XBUTTON1 )
			isPressed [ VK_XBUTTON1 ] = false;
		else if ( button == XBUTTON2 )
			isPressed [ VK_XBUTTON2 ] = false;

		break;
	}
	default:
		break;
	}

	return CallWindowProc( originalWindowProc, hWnd, uMsg, wParam, lParam );
}