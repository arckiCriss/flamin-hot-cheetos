#ifndef FINDMDL_H
#define FINDMDL_H

#pragma once

#include "stdafx.h"

unsigned short __stdcall FindMDL( const char* mdlrelativepath );

typedef unsigned short( __thiscall* FindMDL_t )( IMDLCache*, const char* );
extern FindMDL_t originalFindMDL;

#endif