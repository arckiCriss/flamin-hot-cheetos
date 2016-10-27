#ifndef PROXY_H
#define PROXY_H

#pragma once

#include "stdafx.h"

namespace proxy
{
	extern RecvVarProxyFn modelIndexProxy;

	void initialize( );
	void setModelIndex( const CRecvProxyData* proxydata, void* datastruct, void* out );
}

#endif