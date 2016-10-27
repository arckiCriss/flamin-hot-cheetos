#include "Proxy.h"

Proxy proxy;

Proxy::Proxy( )
{
	modelIndexProxy = 0;
}

void Proxy::initialize( )
{
	for ( IVClientClass* clientClass = interfaces::client->getAllClasses( ); clientClass; clientClass = clientClass->getNextClass( ) )
	{
		if ( !strcmp( clientClass->getName( ), charenc( "DT_BaseViewModel" ) ) )
		{
			RecvTable* classTable = clientClass->getTable( );

			for ( int i = 0; i < classTable->m_nProps; i++ )
			{
				RecvProp* prop = &classTable->m_pProps [ i ];

				if ( !prop || strcmp( prop->m_pVarName, charenc( "m_nModelIndex" ) ) )
					continue;

				modelIndexProxy = prop->m_ProxyFn;
				prop->m_ProxyFn = ( RecvVarProxyFn ) setModelIndex;

				break;
			}

			break;
		}
	}
}

void Proxy::setModelIndex( const CRecvProxyData* proxydata, void* datastruct, void* out )
{
	CRecvProxyData* data = const_cast< CRecvProxyData* >( proxydata );

	if ( cvar::misc_knifechanger )
	{
		CBaseEntity* local = interfaces::entitylist->getClientEntity( interfaces::engine->getLocalPlayer( ) );
		if ( local && local->getLifeState( ) == LIFE_ALIVE )
		{
			if ( data->m_Value.m_Int == interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_default_ct.mdl" ) ) ||
				data->m_Value.m_Int == interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_default_t.mdl" ) ) )
			{
				if ( cvar::misc_knifechanger_model == 0 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_bayonet.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 1 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_flip.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 2 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_gut.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 3 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_karam.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 4 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_m9_bay.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 5 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_tactical.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 6 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_falchion_advanced.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 7 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_butterfly.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 8 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_push.mdl" ) );
				else if ( cvar::misc_knifechanger_model == 9 )
					data->m_Value.m_Int = interfaces::modelinfo->getModelIndex( charenc( "models/weapons/v_knife_survival_bowie.mdl" ) );
			}
		}
	}

	modelIndexProxy( data, datastruct, out );
}