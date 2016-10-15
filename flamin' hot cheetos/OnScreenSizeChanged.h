#ifndef ONSCREENSIZECHANGED_H
#define ONSCREENSIZECHANGED_H

#pragma once

#include "stdafx.h"

void __stdcall OnScreenSizeChanged( int oldwidth, int oldheight );

typedef void( __stdcall* OnScreenSizeChanged_t )( int, int );
extern OnScreenSizeChanged_t originalOnScreenSizeChanged;

#endif