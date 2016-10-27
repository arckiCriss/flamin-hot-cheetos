#ifndef PROXY_H
#define PROXY_H

#pragma once

#include "stdafx.h"

class Proxy
{
public:
	Proxy( );

	void initialize( );

private:
	void setModelIndex( const CRecvProxyData* proxydata, void* datastruct, void* out );

private:
	RecvVarProxyFn modelIndexProxy;
};

extern Proxy proxy;

#endif