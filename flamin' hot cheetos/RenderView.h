#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#pragma once

#include "stdafx.h"

void __stdcall RenderView( CViewSetup& view, int clearflags, int whattodraw );

typedef void( __thiscall* RenderView_t )( void*, CViewSetup&, int, int );
extern RenderView_t originalRenderView;

#endif