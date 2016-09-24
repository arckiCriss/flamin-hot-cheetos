#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H

#pragma once

#include "stdafx.h"

class Misc
{
public:
	void doBunnyhop( CBaseEntity* local, CUserCmd* cmd );
	void doRecoilControl( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd );
};

extern Misc miscellaneous;

#endif