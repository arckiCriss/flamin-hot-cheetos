#ifndef INTERFACES_H
#define INTERFACES_H

#pragma once

#include "stdafx.h"

class CHLClient;
class IEngineClient;
class IClientEntityList;
class ISurface;
class IPanel;
class IVDebugOverlay;
class IEngineTrace;
class CUserCmd;
class CInput;
class IVModelInfo;
class CGlobalVars;
class ICVar;

namespace interfaces
{
	extern DWORD              clientbase;

	extern CHLClient*         client;
	extern void*              clientMode;
	extern IEngineClient*     engine;
	extern IClientEntityList* entitylist;
	extern ISurface*          surface;
	extern IPanel*            panel;
	extern IVDebugOverlay*    debugoverlay;
	extern IEngineTrace*      enginetrace;
	extern CInput*            input;
	extern IVModelInfo*       modelinfo;
	extern CGlobalVars*       globalvars;
	extern ICVar*             convar;
	extern void*              viewRender;

	void initialize( );
}

#endif