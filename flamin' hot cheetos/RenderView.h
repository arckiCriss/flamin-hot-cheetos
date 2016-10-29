#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#pragma once

#include "stdafx.h"

void __stdcall RenderView( CViewSetup& view, CViewSetup& hudViewSetup, int clearFlags, int whatToDraw );

typedef void( __thiscall* RenderView_t )( void*, CViewSetup&, CViewSetup&, int, int );
extern RenderView_t originalRenderView;

#endif