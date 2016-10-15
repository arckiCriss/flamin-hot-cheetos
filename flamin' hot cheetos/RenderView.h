#ifndef RENDERVIEW_H
#define RENDERVIEW_H

#pragma once

#include "stdafx.h"

void __stdcall RenderView( CViewSetup& view, int clearflags, int whattodraw );

typedef void( __stdcall* RenderView_t )( CViewSetup&, int, int );
extern RenderView_t originalRenderView;

#endif