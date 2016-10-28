#ifndef SDK_H
#define SDK_H

#include "stdafx.h"

#include "Control Variables.h"

#include "Offsets.h"

#include "Color.h"
#include "SDKDefinitions.h"

#include "dt_recv.h"
#include "checksum_crc.h"

//----------------------------------------
// GET ENGINE FUNCTIONS
//----------------------------------------

inline void**& getVirtualTable( void* inst, size_t offset = 0 )
{
	return *reinterpret_cast< void*** >( ( size_t ) inst + offset );
}
inline const void** getVirtualTable( const void* inst, size_t offset = 0 )
{
	return *reinterpret_cast< const void*** >( ( size_t ) inst + offset );
}

template<typename Fn>
inline Fn getVirtualFunction( const void* inst, size_t index, size_t offset = 0 )
{
	return reinterpret_cast< Fn >( getVirtualTable( inst, offset ) [ index ] );
}

//----------------------------------------
// ENTITY CLASS
//----------------------------------------

class CBaseEntity
{
public:
	char getLifeState( )
	{
		return *( char* ) ( ( DWORD )this + offsets::player::m_lifeState );
	}
	int getHealth( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_iHealth );
	}
	int getArmor( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_ArmorValue );
	}
	int getTeamNum( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_iTeamNum );
	}
	bool isProtected( )
	{
		return *( bool* ) ( ( DWORD )this + offsets::player::m_bGunGameImmunity );
	}
	bool isScoped( )
	{
		return *( bool* ) ( ( DWORD )this + offsets::player::m_bIsScoped );
	}
	int getAccount( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_iAccount );
	}
	int getFlags( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_fFlags );
	}
	int getShotsFired( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_iShotsFired );
	}
	int getTickBase( )
	{
		return *( int* ) ( ( DWORD )this + offsets::player::m_nTickBase );
	}
	const char* getLastPlaceName( )
	{
		return ( char* ) ( ( DWORD )this + offsets::player::m_szLastPlaceName );
	}
	Vector getViewOffset( )
	{
		return *( Vector* ) ( ( DWORD )this + offsets::player::m_vecViewOffset );
	}
	Vector getOrigin( )
	{
		return *( Vector* ) ( ( DWORD )this + offsets::player::m_vecOrigin );
	}
	Vector getEyePosition( )
	{
		return ( getOrigin( ) + getViewOffset( ) );
	}
	Vector getVelocity( )
	{
		return *( Vector* ) ( ( DWORD )this + offsets::player::m_vecVelocity );
	}
	QAngle getPunchAngles( )
	{
		return *( QAngle* ) ( ( DWORD )this + offsets::player::m_aimPunchAngle );
	}
	ULONG getOwner( )
	{
		return *( ULONG* ) ( ( DWORD )this + offsets::player::m_hOwner );
	}
	matrix3x4& getCoordinateFrame( )
	{
		return *( matrix3x4* ) ( ( DWORD )this + offsets::entity::m_rgflCoordinateFrame );
	}
	Vector getAbsAngles( )
	{
		__asm
		{
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}
	int getIndex( )
	{
		__asm
		{
			MOV EDI, this
			LEA ECX, DWORD PTR DS : [EDI + 0x8]
			MOV EDX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EDX + 0x28]
		}
	}
	bool isDormant( )
	{
		__asm
		{
			MOV EDI, this
			LEA ECX, [ EDI + 0x8 ]
			MOV EDX, DWORD PTR DS : [ecx]
			CALL [ EDX + 0x24 ]
		}
	}
	bool setupBones( matrix3x4* matrix, int maxbones, int mask, float time )
	{
		__asm
		{
			MOV EDI, this
			LEA ECX, DWORD PTR DS : [EDI + 0x4]
			MOV EDX, DWORD PTR DS : [ECX]
			PUSH time
			PUSH mask
			PUSH maxbones
			PUSH matrix
			CALL DWORD PTR DS : [EDX + 0x34]
		}
	}
	model_t* getModel( )
	{
		__asm
		{
			MOV EDI, this
			LEA ECX, [ EDI + 0x4 ]
			MOV EDX, DWORD PTR DS : [ECX]
			CALL [ EDX + 0x20 ]
		}
	}
	IVClientClass* getClientClass( )
	{
		typedef IVClientClass* ( __thiscall* original )( void* );
		return getVirtualFunction<original>( ( void* ) ( this + 0x8 ), 2 )( ( void* ) ( this + 0x8 ) );
	}
	void getRenderBounds( Vector& mins, Vector& maxs )
	{
		typedef void( __thiscall* original )( void*, Vector&, Vector& );
		getVirtualFunction<original>( ( void* ) ( this + 0x4 ), 17 )( ( void* ) ( this + 0x4 ), mins, maxs );
	}
};

//----------------------------------------
// WEAPON ENTITY CLASS
//----------------------------------------

class CBaseCombatWeapon : public CBaseEntity
{
private:
	DWORD dwBase;
public:
	int getItemDefinitionIndex( )
	{
		return *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex );
	}
	int getClip1( )
	{
		return *( int* ) ( ( DWORD )this + offsets::weapon::m_iClip1 );
	}
	float getNextPrimaryAttack( )
	{
		return *( float* ) ( ( DWORD )this + offsets::weapon::m_flNextPrimaryAttack );
	}
	bool isPistol( )
	{
		switch ( this->getItemDefinitionIndex( ) )
		{
		case WEAPON_DEAGLE: return true;
		case WEAPON_DUALS: return true;
		case WEAPON_FIVE7: return true;
		case WEAPON_GLOCK: return true;
		case WEAPON_TEC9: return true;
		case WEAPON_P2000: return true;
		case WEAPON_USPS: return true;
		case WEAPON_P250: return true;
		case WEAPON_CZ75: return true;
		case WEAPON_REVOLVER: return true;
		default: return false;
		}
	}
	bool isKnife( )
	{
		switch ( this->getItemDefinitionIndex( ) )
		{
		case WEAPON_KNIFE_CT: return true;
		case WEAPON_KNIFE_T: return true;
		case WEAPON_KNIFE_GUT: return true;
		case WEAPON_KNIFE_FLIP: return true;
		case WEAPON_KNIFE_BAYONET: return true;
		case WEAPON_KNIFE_M9BAYONET: return true;
		case WEAPON_KNIFE_KARAMBIT: return true;
		case WEAPON_KNIFE_HUNTSMAN: return true;
		case WEAPON_KNIFE_BUTTERFLY: return true;
		case WEAPON_KNIFE_BOWIE: return true;
		case WEAPON_KNIFE_FALCHION: return true;
		case WEAPON_KNIFE_DAGGER: return true;
		default: return false;
		}
	}
	bool isOther( )
	{
		switch ( this->getItemDefinitionIndex( ) )
		{
		case ITEM_NONE: return true;
		case WEAPON_TASER: return true;
		case WEAPON_FLASH: return true;
		case WEAPON_HE: return true;
		case WEAPON_SMOKE: return true;
		case WEAPON_MOLOTOV: return true;
		case WEAPON_DECOY: return true;
		case WEAPON_INC: return true;
		case WEAPON_C4: return true;
		default: return false;
		}
	}
	void setPattern( player_info_t info, int skin, float wear, int seed, int stattrak, const char* name = "" )
	{
		if ( this->isOther( ) )
			return;

		if ( this->isKnife( ) )
		{
			switch ( cvar::misc_knifechanger_model )
			{
			case 0: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_BAYONET; break; }
			case 1: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_FLIP; break; }
			case 2: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_GUT; break; }
			case 3: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_KARAMBIT; break; }
			case 4: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_M9BAYONET; break; }
			case 5: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_HUNTSMAN; break; }
			case 6: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_FALCHION; break; }
			case 7: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_BUTTERFLY; break; }
			case 8: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_DAGGER; break; }
			case 9: { *( int* ) ( ( DWORD )this + offsets::weapon::m_iItemDefinitionIndex ) = WEAPON_KNIFE_BOWIE; break; }
			}
		}

		*( int* ) ( ( DWORD )this + offsets::weapon::m_OriginalOwnerXuidLow ) = 0;
		*( int* ) ( ( DWORD )this + offsets::weapon::m_OriginalOwnerXuidHigh ) = 0;
		*( int* ) ( ( DWORD )this + offsets::weapon::m_nFallbackSeed ) = seed;
		*( int* ) ( ( DWORD )this + offsets::weapon::m_nFallbackPaintKit ) = skin;
		*( float* ) ( ( DWORD )this + offsets::weapon::m_flFallbackWear ) = wear;
		*( int* ) ( ( DWORD )this + offsets::weapon::m_iAccountID ) = info.m_nXuidLow;
		*( int* ) ( ( DWORD )this + offsets::weapon::m_iItemIDHigh ) = 1;

		if ( stattrak > 0 )
			*( int* ) ( ( DWORD )this + offsets::weapon::m_nFallbackStatTrak ) = stattrak;

		if ( stattrak > 0 && !this->isKnife( ) )
			*( int* ) ( ( DWORD )this + offsets::weapon::m_iEntityQuality ) = 1;
		else if ( this->isKnife( ) )
			*( int* ) ( ( DWORD )this + offsets::weapon::m_iEntityQuality ) = 3;
		else
			*( int* ) ( ( DWORD )this + offsets::weapon::m_iEntityQuality ) = 4;

		if ( name )
			sprintf_s( ( char* )this + offsets::weapon::m_szCustomName, 32, "%s", name );
	}
};

//----------------------------------------
// CLIENT INTERFACE
//----------------------------------------

class CHLClient
{
public:
	IVClientClass* getAllClasses( )
	{
		typedef IVClientClass* ( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 8 )( this );
	}
};

//----------------------------------------
// CLIENTCLASS INTERFACE
//----------------------------------------

class IVClientClass
{
public:
	const char* getName( )
	{
		return *( char** ) ( this + 0x8 );
	}
	RecvTable* getTable( )
	{
		return *( RecvTable** ) ( this + 0xC );
	}
	IVClientClass* getNextClass( )
	{
		return *( IVClientClass** ) ( this + 0x10 );
	}
	int getClassID( )
	{
		return *( int* ) ( this + 0x14 );
	}
};

//----------------------------------------
// CLIENT ENTITY LIST INTERFACE
//----------------------------------------

class IClientEntityList
{
public:
	CBaseEntity* getClientEntity( int entnum )
	{
		typedef CBaseEntity* ( __thiscall* original )( void*, int );
		return getVirtualFunction<original>( this, 3 )( this, entnum );
	}
	CBaseEntity* getClientEntityFromHandle( int ent )
	{
		typedef CBaseEntity* ( __thiscall* original )( void*, int );
		return getVirtualFunction<original>( this, 4 )( this, ent );
	}
	int getHighestEntityIndex( )
	{
		typedef int( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 6 )( this );
	}
};

//----------------------------------------
// PANEL INTERFACE
//----------------------------------------

class IPanel
{
public:
	const char* getName( unsigned int vguiPanel )
	{
		typedef const char*( __thiscall* original )( void*, unsigned int );
		return getVirtualFunction<original>( this, 36 )( this, vguiPanel );
	}
};

//----------------------------------------
// SURFACE INTERFACE
//----------------------------------------

class ISurface
{
public:
	void drawSetColor( Color color )
	{
		typedef void( __thiscall* original )( void*, Color );
		getVirtualFunction<original>( this, 14 )( this, color );
	}
	void drawFilledRect( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* original )( void*, int, int, int, int );
		getVirtualFunction<original>( this, 16 )( this, x0, y0, x1, y1 );
	}
	void drawOutlinedRect( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* original )( void*, int x0, int y0, int x1, int y1 );
		getVirtualFunction<original>( this, 18 )( this, x0, y0, x1, y1 );
	}
	void drawLine( int x0, int y0, int x1, int y1 )
	{
		typedef void( __thiscall* original )( void*, int, int, int, int );
		getVirtualFunction<original>( this, 19 )( this, x0, y0, x1, y1 );
	}
	void drawSetTextFont( unsigned long font )
	{
		typedef void( __thiscall* original )( void*, unsigned long );
		getVirtualFunction<original>( this, 23 )( this, font );
	}
	void drawSetTextColor( Color color )
	{
		typedef void( __thiscall* original )( void*, Color );
		getVirtualFunction<original>( this, 24 )( this, color );
	}
	void drawSetTextPos( int x, int y )
	{
		typedef void( __thiscall* original )( void*, int, int );
		getVirtualFunction<original>( this, 26 )( this, x, y );
	}
	void drawPrintText( const wchar_t* text, int textLen )
	{
		typedef void( __thiscall* original )( void*, const wchar_t *, int, int );
		getVirtualFunction<original>( this, 28 )( this, text, textLen, 0 );
	}
	unsigned long createFont( )
	{
		typedef unsigned int( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 71 )( this );
	}
	void setFontGlyphSet( unsigned long& font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags )
	{
		typedef void( __thiscall* original )( void*, unsigned long, const char*, int, int, int, int, int, int, int );
		getVirtualFunction<original>( this, 72 )( this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0 );
	}
	void getTextSize( unsigned long font, const wchar_t *text, int &wide, int &tall )
	{
		typedef void( __thiscall* original )( void*, unsigned long font, const wchar_t *text, int &wide, int &tall );
		getVirtualFunction<original>( this, 79 )( this, font, text, wide, tall );
	}
	void getCursorPos( int& x, int& y )
	{
		typedef void( __thiscall* original )( void*, int &, int & );
		getVirtualFunction<original>( this, 100 )( this, x, y );
	}
	const char* getFontName( unsigned long font )
	{
		typedef const char*( __thiscall* original )( void*, unsigned long );
		return getVirtualFunction<original>( this, 134 )( this, font );
	}
};

//----------------------------------------
// DEBUG OVERLAY INTERFACE
//----------------------------------------

class IVDebugOverlay
{
public:
	bool getScreenPosition( const Vector& vIn, Vector& vOut )
	{
		typedef bool( __thiscall* original )( void*, const Vector&, Vector& );
		return getVirtualFunction<original>( this, 13 )( this, vIn, vOut );
	}
};

//----------------------------------------
// ENGINE CLIENT INTERFACE
//----------------------------------------

class IEngineClient
{
public:
	void getScreenSize( int& width, int& height )
	{
		typedef void( __thiscall* original )( void*, int&, int& );
		getVirtualFunction<original>( this, 5 )( this, width, height );
	}
	bool getPlayerInfo( int iIndex, player_info_t* pInfo )
	{
		typedef bool( __thiscall* original )( void*, int, player_info_t* );
		return getVirtualFunction<original>( this, 8 )( this, iIndex, pInfo );
	}
	int getLocalPlayer( )
	{
		typedef int( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 12 )( this );
	}
	float getLastTimeStamp( )
	{
		typedef float( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 14 )( this );
	}
	void getViewAngles( QAngle& angles )
	{
		typedef void( __thiscall* original )( void*, QAngle& );
		getVirtualFunction<original>( this, 18 )( this, angles );
	}
	void setViewAngles( QAngle& angles )
	{
		typedef void( __thiscall* original )( void*, QAngle& );
		getVirtualFunction<original>( this, 19 )( this, angles );
	}
	int getMaxClients( )
	{
		typedef int( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 20 )( this );
	}
	bool inGame( )
	{
		typedef bool( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 26 )( this );
	}
	bool isConnected( )
	{
		typedef bool( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 27 )( this );
	}
	void executeClientCmd( const char* command )
	{
		typedef void( __thiscall* original )( void*, const char* );
		getVirtualFunction<original>( this, 108 )( this, command );
	}
	void clientCmd_Unrestricted( const char* command )
	{
		typedef void( __thiscall* original )( void*, const char *, char );
		getVirtualFunction<original>( this, 114 )( this, command, 0 );
	}
};

//----------------------------------------
// INPUT INTERFACE
//----------------------------------------

class CUserCmd
{
public:
	CRC32_t getChecksum( ) const
	{
		CRC32_t crc;
		CRC32_Init( &crc );
		CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
		CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
		CRC32_ProcessBuffer( &crc, &viewangles, sizeof( viewangles ) );
		CRC32_ProcessBuffer( &crc, &aimdirection, sizeof( aimdirection ) );
		CRC32_ProcessBuffer( &crc, &forwardmove, sizeof( forwardmove ) );
		CRC32_ProcessBuffer( &crc, &sidemove, sizeof( sidemove ) );
		CRC32_ProcessBuffer( &crc, &upmove, sizeof( upmove ) );
		CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
		CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );
		CRC32_ProcessBuffer( &crc, &weaponselect, sizeof( weaponselect ) );
		CRC32_ProcessBuffer( &crc, &weaponsubtype, sizeof( weaponsubtype ) );
		CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
		CRC32_ProcessBuffer( &crc, &mousedx, sizeof( mousedx ) );
		CRC32_ProcessBuffer( &crc, &mousedy, sizeof( mousedy ) );
		CRC32_Final( &crc );
		return crc;
	}

	BYTE		u1 [ 4 ];
	int			command_number;
	int			tick_count;
	QAngle		viewangles;
	Vector		aimdirection;
	float		forwardmove;
	float		sidemove;
	float		upmove;
	int			buttons;
	BYTE		impulse;
	int			weaponselect;
	int			weaponsubtype;
	int			random_seed;
	short		mousedx;
	short		mousedy;
	bool		hasbeenpredicted;
	Vector		headangles;
	Vector		headoffset;
};

class CInput
{
public:
	class CVerifiedUserCmd
	{
	public:
		CUserCmd		m_cmd;
		unsigned long	m_crc;
	};

	CUserCmd* getUserCmd( int slot, int seq )
	{
		typedef CUserCmd* ( __thiscall* original )( void*, int, int );
		return getVirtualFunction<original>( this, 8 )( this, slot, seq );
	}
};

//----------------------------------------
// ENGINE TRACE INTERFACE
//----------------------------------------

class IEngineTrace
{
public:
	enum TraceType_t
	{
		TRACE_EVERYTHING = 0,
		TRACE_WORLD_ONLY,
		TRACE_ENTITIES_ONLY,
		TRACE_EVERYTHING_FILTER_PROPS,
	};

	struct cplane_t
	{
		Vector normal;
		float dist;
		BYTE type;
		BYTE signBits;
		BYTE pad [ 2 ];
	};

	struct csurface_t
	{
		const char*		name;
		short			surfaceProps;
		unsigned short	flags;
	};

	struct Ray_t
	{
		Ray_t( ) { }

		VectorAligned	 m_start;
		VectorAligned	 m_delta;
		VectorAligned	 m_startOffset;
		VectorAligned	 m_extents;
		const matrix3x4* m_worldAxisTransform;
		bool			 m_isRay;
		bool			 m_isSwept;

		void Init( Vector start, Vector end )
		{
			m_delta = VectorAligned( end - start );
			m_isSwept = ( m_delta.LengthSqr( ) != 0 );
			m_extents.Zero( );
			m_worldAxisTransform = NULL;
			m_isRay = true;
			m_startOffset.Zero( );
			m_start = start;
		}
	};

	struct trace_t
	{
		Vector		 start;
		Vector		 endpos;
		cplane_t	 plane;
		float		 fraction;
		int			 contents;
		WORD		 dispFlags;
		bool		 allSolid;
		bool		 startSolid;
		float		 fractionLeftSolid;
		csurface_t	 surface;
		int			 hitgroup;
		short		 physicsBone;
		WORD		 worldSurfaceIndex;
		CBaseEntity* entity;
		int			 hitbox;
	};

	class ITraceFilter
	{
	public:
		virtual bool shouldHitEntity( CBaseEntity* entity, int contentsMask ) = 0;
		virtual TraceType_t getTraceType( ) const = 0;
	};
	class CTraceFilter : public ITraceFilter
	{
	public:
		bool shouldHitEntity( CBaseEntity* entityHandle, int contentsMask )
		{
			return !( entityHandle == skip );
		}
		virtual TraceType_t getTraceType( ) const
		{
			return TRACE_EVERYTHING;
		}

		void* skip;
	};

public:
	void traceRay( const Ray_t& ray, unsigned int mask, CTraceFilter* filter, trace_t* trace )
	{
		typedef void( __thiscall* original )( void*, const Ray_t&, unsigned int, CTraceFilter*, trace_t* );
		getVirtualFunction<original>( this, 5 )( this, ray, mask, filter, trace );
	}
};

//----------------------------------------
// GLOBAL VARIABLES
//----------------------------------------

class CGlobalVars
{
public:
	float     realtime;
	int       framecount;
	float     absoluteframetime;
	float     unknown;
	float     curtime;
	float     frametime;
	int       maxclients;
	int       tickcount;
	float     interval_per_tick;
	float     interpolation_amount;
};

//----------------------------------------
// MDL CACHE INTERFACE
//----------------------------------------

class IMDLCache
{
public:
};

//----------------------------------------
// MATERIAL INTERFACE
//----------------------------------------

class IMaterial
{
public:
};

//----------------------------------------
// MODEL INFO INTERFACE
//----------------------------------------

class IVModelInfo
{
public:
	int getModelIndex( const char* modelName )
	{
		typedef int( __thiscall* original )( void*, const char* );
		return getVirtualFunction<original>( this, 2 )( this, modelName );
	}
	const char* getModelName( const model_t* pModel )
	{
		typedef const char* ( __thiscall* original )( void*, const model_t* );
		return getVirtualFunction<original>( this, 3 )( this, pModel );
	}
	int getInt( const char* keyName, int defaultValue = 0 )
	{
		typedef int( __thiscall* original )( void*, const char*, int );
		return getVirtualFunction<original>( this, 6 )( this, keyName, defaultValue );
	}
	studiohdr_t* getStudioModel( const model_t* model )
	{
		typedef studiohdr_t* ( __thiscall* original )( void*, const model_t* );
		return getVirtualFunction<original>( this, 30 )( this, model );
	}
};

//----------------------------------------
// CVAR INTERFACE
//----------------------------------------

class ConVar
{
public:
	float getFloat( )
	{
		typedef float( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 12 )( this );
	}
	int getInt( )
	{
		typedef int( __thiscall* original )( void* );
		return getVirtualFunction<original>( this, 13 )( this );
	}
};

class ICVar
{
public:
	ConVar* findVar( const char *var )
	{
		typedef ConVar*( __thiscall* original )( void*, const char * );
		return getVirtualFunction<original>( this, 15 )( this, var );
	}
};

#endif
