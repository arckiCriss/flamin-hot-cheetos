#ifndef FRAMESTAGENOTIFY_H
#define FRAMESTAGENOTIFY_H

#pragma once

#include "stdafx.h"

void __stdcall FrameStageNotify( ClientFrameStage_t curstage );

typedef void( __stdcall* FrameStageNotify_t )( ClientFrameStage_t );
extern FrameStageNotify_t originalFrameStageNotify;

#endif