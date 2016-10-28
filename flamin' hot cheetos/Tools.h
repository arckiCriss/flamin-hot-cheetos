#ifndef TOOLS_H
#define TOOLS_H

#pragma once

#include "stdafx.h"
#include "SDKDefinitions.h"

class Tools
{
public:
	void*              getInterface( std::string moduleName, std::string interfaceName );
	DWORD_PTR          getPatternOffset( std::string moduleName, PBYTE pattern, std::string mask, DWORD_PTR codeBase = 0, DWORD_PTR codeSize = 0 );

	bool               isVisible( Vector& start, Vector& end, CBaseEntity* entity );
	CBaseCombatWeapon* getActiveWeapon( CBaseEntity* entity );
	bool               WorldToScreen( Vector& world, Vector& screen );

	void               VectorTransform( const Vector& in1, const matrix3x4& in2, Vector& out );
	void               sinCos( float radians, float* sine, float* cosine );

	void               angleVectors( const Vector angles, Vector& forward );

	bool               getHitboxPosition( int hitbox, Vector& origin, CBaseEntity* entity );
	Vector             getPredictedPosition( Vector src, Vector velocity );
	float              getDistance( Vector origin, Vector other );
	float              getFov( QAngle viewAngles, QAngle aimAngles );

	void               computeAngle( Vector source, Vector dest, QAngle& angles );
	QAngle             computeAngle( Vector source, Vector dest );
	void               normalizeAngles( QAngle& angles );
	void               clampAngles( QAngle& angles );

	bool               isAbleToShoot( CBaseEntity* entity, CBaseCombatWeapon* weapon );

	float              random( float min, float max );

	void               moveMouse( float x, float y );
};

extern Tools tools;

#endif