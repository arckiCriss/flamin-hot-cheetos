#include "FindMDL.h"

FindMDL_t originalFindMDL;

unsigned short __stdcall FindMDL( const char* mdlrelativepath )
{
	if ( cvar::misc_knifechanger )
	{
		if ( strstr( mdlrelativepath, charenc( "v_knife_default_ct.mdl" ) ) || strstr( mdlrelativepath, charenc( "v_knife_default_t.mdl" ) ) )
		{
			switch ( cvar::misc_knifechanger_model )
			{
			case 0: return originalFindMDL( charenc( "models/weapons/v_knife_bayonet.mdl" ) );
			case 1: return originalFindMDL( charenc( "models/weapons/v_knife_flip.mdl" ) );
			case 2: return originalFindMDL( charenc( "models/weapons/v_knife_gut.mdl" ) );
			case 3: return originalFindMDL( charenc( "models/weapons/v_knife_karam.mdl" ) );
			case 4: return originalFindMDL( charenc( "models/weapons/v_knife_m9_bay.mdl" ) );
			case 5: return originalFindMDL( charenc( "models/weapons/v_knife_tactical.mdl" ) );
			case 6: return originalFindMDL( charenc( "models/weapons/v_knife_falchion_advanced.mdl" ) );
			case 7: return originalFindMDL( charenc( "models/weapons/v_knife_butterfly.mdl" ) );
			case 8: return originalFindMDL( charenc( "models/weapons/v_knife_push.mdl" ) );
			case 9: return originalFindMDL( charenc( "models/weapons/v_knife_survival_bowie.mdl" ) );
			}
		}
	}

	return originalFindMDL( mdlrelativepath );
}