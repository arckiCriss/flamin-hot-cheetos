#ifndef WNDPROC_H
#define WNDPROC_H

#pragma once

#include "stdafx.h"

LONG_PTR __stdcall WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

extern HWND windowHandle;
extern WNDPROC originalWindowProc;

extern bool isPressed [ 256 ];

#endif