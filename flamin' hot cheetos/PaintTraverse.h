#ifndef PAINTTRAVERSE_H
#define PAINTTRAVERSE_H

#pragma once

#include "stdafx.h"

void __stdcall PaintTraverse( unsigned int vguipanel, bool forcerepaint, bool allowforce );

typedef void( __thiscall* PaintTraverse_t )( IPanel*, unsigned int, bool, bool );
extern PaintTraverse_t originalPaintTraverse;

#endif