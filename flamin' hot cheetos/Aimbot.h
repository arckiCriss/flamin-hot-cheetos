#ifndef LEGITBOT_H
#define LEGITBOT_H

#pragma once

#include "stdafx.h"

class Aimbot
{
public:
	Aimbot( );

	void   think( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd );

private:
	bool   getClosestHitbox( CBaseEntity* local, CBaseEntity* entity, Vector& destination );
	int    getBestTarget( CBaseEntity* local, CBaseCombatWeapon* weapon, CUserCmd* cmd, Vector& destination );

	QAngle getRandomizedRecoil( CBaseEntity* local );
	QAngle getRandomizedAngles( CBaseEntity* local );

private:
	int    bestTarget;

	QAngle viewAngle;
	QAngle aimAngle;
	QAngle deltaAngle;
	QAngle finalAngle;

	Vector hitboxPosition;
};

extern Aimbot aimbot;

#endif