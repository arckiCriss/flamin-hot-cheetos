#ifndef LEGITBOT_H
#define LEGITBOT_H

#pragma once

#include "stdafx.h"

class Aimbot
{
public:
	Aimbot( );

	void   think( CBaseEntity* local, CBaseCombatWeapon* weapon );

private:
	bool   getClosestHitbox( CBaseEntity* local, CBaseEntity* entity, Vector& dest );
	int    getBestTarget( CBaseEntity* local, CBaseCombatWeapon* weapon, Vector& dest );
	QAngle getRandomizedRecoil( CBaseEntity* local );
	QAngle getRandomizedAngles( CBaseEntity* local );

private:
	int    bestTarget;

	QAngle viewAngles;
	QAngle aimAngles;
	QAngle finalAngles;

	Vector hitboxPosition;
};

extern Aimbot aimbot;

#endif